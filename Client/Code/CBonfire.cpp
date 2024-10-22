#include "stdafx.h"
#include "CBonfire.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "SlotMgr.h"
#include "MainApp.h"

#include "CFire.h"
#include <Mouse.h>

CBonfire::CBonfire(LPDIRECT3DDEVICE9 pGraphicDev , _bool bInstall)
	: CResObject(pGraphicDev), m_bInstall(bInstall)

{
}

CBonfire::CBonfire(const CBonfire& rhs)
	: CResObject(rhs.m_pGraphicDev)
	, m_pFire(rhs.m_pFire)

{

}

CBonfire::~CBonfire()
{
}

HRESULT CBonfire::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	_vec3 vPos;
	m_pTransForm->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 0.5f, vPos.z);



	m_eObject_id = BON_FIRE;
	m_eBonfireCurState = BONFIRE_IDLE;
	m_fFrame = 0.0f;

	m_Stat.strObjName = L"모닥불";
	return S_OK;
}

static bool teskey = false;

_int CBonfire::Update_GameObject(const _float& fTimeDelta)
{
	Install_Obj();
	
	//
	//Test 외부에서 똑같이 동작확인을 위한 코드 ()
	//if (GetAsyncKeyState('1')& 0x8000) // 횃불
	//{
	//	if (!teskey)
	//	{
	//		teskey = true;
	//	}
	//}
	//else
	//{
	//	if (teskey)
	//	{
	//		AddFIre(1);
	//
	//		teskey = false;
	//	}
	//		
	//}
	
	if (m_eBonfireCurState == BONFIRE_DROP)
		{
			if (!m_bIsDropEnd)
			{
				m_fFrame += m_fFrameEnd * fTimeDelta;
				if (m_fFrame >= m_fFrameEnd)
				{
					m_fFrame = m_fFrameEnd;
					m_bIsDropEnd = true;
				
				}
			}
		}
	
	if (m_bIsDrop)
	{
		m_eBonfireCurState = BONFIRE_DROP;

	}

	if (m_pFire)
	{
		//불이 꺼지면 장작도 IDLE상태로
		if (m_pFire->Get_IsOff())
		{
			m_eBonfireCurState = BONFIRE_IDLE;
	
		}

	}
	//드롭이 끝나면 해당 이미지idle로 변경
	if (m_bIsDropEnd)
	{
		m_eBonfireCurState = BONFIRE_IDLE;
	}

	
	Check_FrameState();

	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);


	//드롭이 끝나면 해당 위치에 불 업데이트
	if (m_bIsDropEnd)
	{
		
		if (m_pFire)
		{
			_vec3	vPos;
			m_pTransForm->Get_Info(INFO_POS, &vPos);
			m_pFire->Set_Pos(vPos);
			m_pFire->Update_GameObject(fTimeDelta);

		}


	}


	return 0;
}

void CBonfire::LateUpdate_GameObject()
{


	

	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	__super::LateUpdate_GameObject();

	if (m_pFire)
	{
		m_pFire->LateUpdate_GameObject();
	}

}

void CBonfire::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_bShader || m_bInstall)
	{
		m_pBonfireTexCom[m_eBonfireCurState]->Set_Texture(m_pShaderCom, "g_Texture", m_fFrame);
		_matrix maxView, maxProj;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &maxView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &maxProj);

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransForm->Get_WorldMatrix())))
			return;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &maxView)))
			return;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &maxProj)))
			return;

		if (FAILED(m_pShaderCom->Begin_Shader(m_bInstall ? 1 : 0)))
			return;

		m_pBufferCom->Render_Buffer();

		if (FAILED(m_pShaderCom->End_Shader()))
			return;
	}
	else
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
		m_pBonfireTexCom[m_eBonfireCurState]->Set_Texture((_uint)m_fFrame);
		FAILED_CHECK_RETURN(SetUp_Material(), );
		m_pBufferCom->Render_Buffer();
	}


	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (m_pFire)
	{
		m_pFire->Set_Shader(m_bShader);
		m_pFire->Render_GameObject();
	}
}

void CBonfire::Install_Obj() // 설치시 마우스 포인터를 따라옴 (쉐이더 써야함)
{
	if (!m_bInstall) return;

	//auto& vecMouse = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::ENVIRONMENT, eOBJECT_GROUPTYPE::MOUSE)[0];
	//CMouse* pMouse = dynamic_cast<CMouse*>(vecMouse);

	//_vec3 vMouseRayPos = pMouse->Get_MouseRayPos(); // 마우스 월드 좌표값
	//vMouseRayPos.y = 1.f;
	//m_pTransForm->Set_Pos(vMouseRayPos);
	auto& vecTerrain = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::TILE)[0];
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(scenemgr::Get_CurScene()->GetTerrainObject()->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	_vec3 vPos = m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, vecTerrain->GetTransForm());

	vPos.y = 0.5f;
	m_pTransForm->Set_Pos(vPos);

	if (Engine::GetMouseState(DIM_LB) == eKEY_STATE::TAP) // 설치 완료
	{
		m_bInstall = false;
		m_bIsDrop = true;
		auto vecMouse = scenemgr::Get_CurScene()->GetMouseObject();
		CMouse* pMouse = dynamic_cast<CMouse*>(vecMouse);
		pMouse->Set_Install(false);

		CSlotMgr::GetInstance()->Remove_InvenItem(m_iSlotNum);
		Set_DropBonfire();
	}
}

void CBonfire::AddFIre(int _Value)
{
	m_fFireWoodCount += _Value;


	if (m_pFire)
	{
		if (m_pFire->Get_CurState() == 1&& m_pFire->Get_IsOff())
		{
			//Engine::SpatialPlay_Sound(L"Obj_Campfire_Lv1.mp3", SOUND_EFFECT_CONTINUE_CH1);
			Engine::PlayEffectContinue(L"Obj_Campfire_Lv1.mp3", 0.8, STEREO_BGM);
			m_pFire->Set_IsOff(false);
		}


		if (m_fFireWoodCount >= 5)
		{
			m_pFire->Set_NextLevel();
			//
			Engine::PlaySound_W(L"Obj_Campfire_Addfuel.mp3", SOUND_EFFECT, 0.2f);
			m_fFireWoodCount = 0;

		}

		//연료를 추가 할 때마다 항상 시간이 증가해야함 
		m_pFire->Set_MoreTime();

	}

}


HRESULT CBonfire::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pBonfireTexCom[BONFIRE_DROP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_BonFIre_Drop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_BonFIre_Drop", pComponent });


	pComponent = m_pBonfireTexCom[BONFIRE_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Bonfire_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Bonfire_Idle", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(proto::Clone_Proto(L"Proto_Shader_Rect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Shader_Rect", pComponent });




	return S_OK;
}

void CBonfire::Check_FrameState()
{


	if (m_eBonfireCurState != m_eBonfirerPrevState)
	{

		switch (m_eBonfireCurState)
		{
		case CBonfire::BONFIRE_IDLE:
		{
			m_pTransForm->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
			m_fFrameEnd = 0.0f;
			break;
		}
		case CBonfire::BONFIRE_DROP:
		{
			m_pTransForm->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
			m_fFrameEnd = 6.0f;
			m_pFire = CFire::Create(m_pGraphicDev);


			break;
		}
		default:
			break;
		}

		m_eBonfirerPrevState = m_eBonfireCurState;
		m_fFrame = 0.0f;
	}


}

CResObject* CBonfire::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall)
{
	CResObject* pInstance = new CBonfire(pGraphicDev, bInstall);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBonfire::Free()
{
	Safe_Release(m_pFire);
	CGameObject::Free();
}


