#include "stdafx.h"
#include "CPigHouse.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "ResObject.h"
#include "Monster.h"
#include "MainApp.h"
#include "Pig.h"

CPigHouse::CPigHouse(LPDIRECT3DDEVICE9 pGraphicDev)
	:CResObject(pGraphicDev), m_eHouseState(STANDARD), m_iPointNum(++CMainApp::g_iLightNum)
{
}

CPigHouse::CPigHouse(const CPigHouse& rhs)
	:CResObject(rhs.m_pGraphicDev), m_eHouseState(rhs.m_eHouseState)
{
}

CPigHouse::~CPigHouse()
{
}

HRESULT CPigHouse::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Set_Scale(_vec3(2.5f, 2.5f, 2.5f));

	m_eCurState = RES_IDLE;
	m_eObject_id = PIG_HOUSE;
	m_fFrame = 0.f;
	Ready_Stat();
	Ready_Light();

	return S_OK;
}

_int CPigHouse::Update_GameObject(const _float& fTimeDelta)
{
	Change_Light();

	if (m_ePreState != RES_IDLE && m_ePreState != RES_DEAD)
		m_fFrame += m_fFrameSpeed * fTimeDelta;

	if (m_fFrameEnd < m_fFrame)
	{
		if (m_ePreState == RES_HIT_1 || m_ePreState == RES_HIT_2) // 피격 모션이 끝난 후 IDLE로 돌아감
		{
			m_eCurState = RES_IDLE;
		}
		if (m_bHit)
			m_bHit = false;
		m_fFrame = 0.f;
	}

	CGameObject::Update_GameObject(fTimeDelta);

	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CPigHouse::LateUpdate_GameObject()
{

	__super::LateUpdate_GameObject();

	Change_Frame_Event(); // 피격 혹은 특정 이벤트 발생시 집의 형태 변환
	Check_FrameState();
	_vec3 vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

}

void CPigHouse::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom[m_eCurState]->Set_Texture((int)m_fFrame);
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CPigHouse::Set_Pig_Angry()
{
	_vec3 vThispos;
	vThispos = m_pTransForm->Get_Pos();
	auto& vecObj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::MONSTER);
	for (auto& obj : vecObj)
	{
		_vec3  vTargetPos;
		vTargetPos = obj->GetTransForm()->Get_Pos();
		if (D3DXVec3Length(&(vTargetPos - vThispos)) <= 10.f)
		{
			if (obj->IsDelete() || obj == nullptr || dynamic_cast<CMonster*>(obj)->Get_Name() != L"돼지")
				continue;
			else
				dynamic_cast<CPig*>(obj)->Set_AttackState();
		}

	}
}

HRESULT CPigHouse::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

#pragma region TEXCOM
	pComponent = m_pTextureCom[RES_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_PigHouse_IDLE"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PigHouse_IDLE", pComponent });
	
	pComponent = m_pTextureCom[RES_HIT_1] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_PigHouse_hit_std"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PigHouse_hit_std", pComponent });

	pComponent = m_pTextureCom[RES_HIT_2] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_PigHouse_hit_run_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PigHouse_hit_run_down", pComponent });

	pComponent = m_pTextureCom[RES_DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_PigHouse_dead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PigHouse_dead", pComponent });

	pComponent = m_pTextureCom[RES_DEAD2] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Erase"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Erase", pComponent });
#pragma endregion TEXCOM

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CPigHouse::Change_Frame_Event()
{
	//시간에 따른 텍스쳐 프레임값 변경 
	TIME_STATE  eTimeState = light::Get_TimeIndex();
	//IDLE일때의 상태값
	if (m_ePreState == RES_IDLE && eTimeState != MORNING && m_Stat.fHP == m_Stat.fMxHP)
		m_fFrame = 1.f; // 점조명 추가필요
	

	//HIT 시 상태값
	if (m_bHit && m_Stat.fHP <= 4)
		m_eCurState = RES_HIT_2;

	else if (m_bHit && m_Stat.fHP >= 5)
		m_eCurState = RES_HIT_1;

	else if (!m_bHit)
		m_eCurState = RES_IDLE;

	if (m_Stat.fHP <= 0) // 체력이 0일때
	{
		if (m_bIsDropItem == false)
		{
			
			CreateItem(L"Woodplank", this, this->m_pGraphicDev);
			
		}
		m_bIsDropItem = true;
		m_eCurState = RES_DEAD;
		
		

	}
}


void CPigHouse::Check_FrameState()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case RES_IDLE:
		m_fFrameEnd = 3;
		m_fFrameSpeed = 0.f;
		m_fFrame = 0.f;
		if (m_Stat.fHP <= 4)
			m_fFrame = 2.f;
		break;
	case RES_HIT_1:
		m_fFrameEnd = 5;
		m_fFrameSpeed = 12.f;
		m_fFrame = 0.f;
		break;
	case RES_HIT_2:
		m_fFrameEnd = 5;
		m_fFrameSpeed = 12.f;
		m_fFrame = 0.f;
		break;
	case RES_DEAD:
		m_fFrameEnd = 0;
		m_fFrameSpeed = 0.f;
		m_fFrame = 0.f;
		break;
	case RES_DEAD2:
		m_fFrameEnd = 5;
		m_fFrameSpeed = 8.f;
		m_fFrame = 0.f;
		break;
	
	}
	m_ePreState = m_eCurState;
}

void CPigHouse::Ready_Stat()
{
	m_Stat.strObjName = L"돼지 집";
	m_Stat.fHP = 10.f;
	m_Stat.fMxHP = 10.f;
	m_Stat.fSpeed = 1.f;
	m_Stat.bDead = false;
}

HRESULT CPigHouse::Ready_Light()
{
	//점광원
// 최초 생성 후 플레이어 횟불 사용 시에만 켜지도록 
	D3DLIGHT9 tPointLightInfo;
	ZeroMemory(&tPointLightInfo, sizeof(D3DLIGHT9));

	tPointLightInfo.Type = D3DLIGHT_POINT;

	tPointLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tPointLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tPointLightInfo.Attenuation0 = 3.f;

	tPointLightInfo.Range = 5.f;
	tPointLightInfo.Position = { 0.f, 0.f, 0.f };

	FAILED_CHECK_RETURN(light::Ready_Light(m_pGraphicDev, &tPointLightInfo, m_iPointNum), E_FAIL);
	light::Get_Light(m_iPointNum)->Close_Light();

	return S_OK;
}

void CPigHouse::Change_Light()
{
	//오전을 제외한 모든 시간에 
	TIME_STATE eTimeState = light::Get_TimeIndex();
	if (eTimeState == MORNING|| m_eCurState==DEAD)
	{
		light::Get_Light(m_iPointNum)->Close_Light();
		return;
	}

	D3DLIGHT9* tPointLightInfo = light::Get_Light(m_iPointNum)->Get_Light();
	//ZeroMemory(&tPointLightInfo, sizeof(D3DLIGHT9));

	tPointLightInfo->Type = D3DLIGHT_POINT;

	tPointLightInfo->Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tPointLightInfo->Attenuation0 = 3.f;

	tPointLightInfo->Range = 3.f;

	_vec3 pPigHousePos;
	m_pTransForm->Get_Info(INFO_POS, &pPigHousePos); // player pos 값 설정
	tPointLightInfo->Position = pPigHousePos;

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1), E_FAIL);
	light::Get_Light(m_iPointNum)->Update_Light();
}


CResObject* CPigHouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CPigHouse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CPigHouse::Free()
{
	CGameObject::Free();
}

