#include "stdafx.h"
#include "CTent.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include <Mouse.h>
#include "SlotMgr.h"
#include <Player.h>

CTent::CTent(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall)
	: CResObject(pGraphicDev), m_bInstall(bInstall)
{
}

CTent::CTent(const CTent& rhs)
	: CResObject(rhs.m_pGraphicDev)
{

}

CTent::~CTent()
{
}

HRESULT CTent::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eObject_id = RESOBJID::TENT;
	m_eTentCurState = CTENT_DEFAULT;
	_vec3 vPos;
	m_pTransForm->Set_Scale(_vec3(3.7f, 3.7f, 3.7f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.7f, vPos.z);

	m_Stat.strObjName = L"텐트";
	m_fFrame = 0.0f;
	return S_OK;
}

_int CTent::Update_GameObject(const _float& fTimeDelta)
{

	Install_Obj();
	

	//예시 코드

	//if (GetAsyncKeyState('1')) // 횃불
	//{
	//
	//	Set_Enter();
	//}
	//if (GetAsyncKeyState('2')) // 횃불
	//{
	//
	//	Set_Hit();
	//}
	if (GetAsyncKeyState('3')) // 횃불
	{
		Set_Destroy();
	
	}
	//if (GetAsyncKeyState('4')) // 횃불
	//{
	//	Set_Burnt();
	//}


	if (!m_bIsFrameStop)
		m_fFrame += m_fFrameEnd * fTimeDelta;



	Change_Frame_Event();

	if (Engine::GetMouseState(DIM_RB) == eKEY_STATE::TAP) // 플레이어와 상호작용
	{
		//auto& vecUI = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI);
		auto vecMouse = scenemgr::Get_CurScene()->GetMouseObject();
		CMouse* pMouse = dynamic_cast<CMouse*>(vecMouse);
		_vec3 vPos;
		m_pTransForm->Get_Info(INFO_POS, &vPos);
		if (Engine::Collision_Mouse_Object(pMouse->Get_MouseRayPos(), pMouse->Get_MouseRayDir(), vPos, m_pTransForm->Get_Scale()))
		{
			//충돌처리
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(scenemgr::Get_CurScene()->GetPlayerObject());
			pPlayer->Set_Tent();
			Set_Enter();
		}
	}

	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CTent::LateUpdate_GameObject()
{



	Check_FrameState();




	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	__super::LateUpdate_GameObject();
}

void CTent::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_bShader || m_bInstall)
	{
		m_pTentTexCom[m_eTentCurState]->Set_Texture(m_pShaderCom, "g_Texture", m_fFrame);
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

		m_pTentTexCom[m_eTentCurState]->Set_Texture((_uint)m_fFrame);
		FAILED_CHECK_RETURN(SetUp_Material(), );
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}




HRESULT CTent::Add_Component()
{
	CComponent* pComponent = nullptr;




	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTentTexCom[CTENT_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Idle", pComponent });


	pComponent = m_pTentTexCom[CTENT_HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Hit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Hit", pComponent });

	pComponent = m_pTentTexCom[CTENT_ENTER] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Enter"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Enter", pComponent });

	pComponent = m_pTentTexCom[CTENT_DESTROY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Destroy"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Destroy", pComponent });

	pComponent = m_pTentTexCom[CTENT_BURNT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Burnt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Burnt", pComponent });


	pComponent = m_pTentTexCom[CTENT_DEFAULT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Defalut_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Defalut_Idle", pComponent });

	pComponent = m_pTentTexCom[CTENT_PLACE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Place"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Place", pComponent });


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

void CTent::Check_FrameState()
{
	if (m_eTentCurState != m_eTentPrevState)
	{
		switch (m_eTentCurState)
		{
		case CTent::CTENT_IDLE:
			m_fFrameEnd = 18.0f;
			break;
		case CTent::CTENT_HIT:
			m_fFrameEnd = 3.0f;
			break;
		case CTent::CTENT_ENTER:
			m_fFrameEnd = 7.0f;
			Engine::PlaySound_W(L"Obj_Tent_Enter.mp3", SOUND_EFFECT, 0.2f);
			break;
		case CTent::CTENT_DESTROY:
			Engine::PlaySound_W(L"Obj_Tent_Destroy.mp3", SOUND_EFFECT, 0.2f);
			m_fFrameEnd = 16.0f;
			break;
		case CTent::CTENT_BURNT:
			m_fFrameEnd = 0.0f;
			break;
		case CTent::CTENT_PLACE:
			Engine::PlaySound_W(L"Obj_Tent_Craft.mp3", SOUND_EFFECT, 0.2f);
			m_fFrameEnd = 7.0f;
			break;
		case CTent::CTENT_DEFAULT:
			m_fFrameEnd = 0.0f;
			break;
		default:
			break;
		}
		m_eTentPrevState = m_eTentCurState;
		m_fFrame = 0.0f;
	}



}

void CTent::Change_Frame_Event()
{
	if (m_bIsDrop)
	{
		if (m_eTentCurState == CTENT_DEFAULT)
		{
			
			m_eTentCurState = CTENT_PLACE;
		}

		if (m_bIsBurnt && !m_bIsDestroy)	//파괴되지 않은 상태에서만 탈 수 있음
		{
			m_eTentCurState = CTENT_BURNT;
			return;
		}

		if (m_bIsDestroy)
		{
			
			//Obj_Tent_Destroy.mp3
			m_eTentCurState = CTENT_DESTROY;
			if (m_fFrame > m_fFrameEnd)
			{
				
				m_bIsFrameStop = true;
				//회수(파괴)시 다시는 못 지음 새로 텐트를 생성해야함
				return;
			}
		}

		if (m_eTentCurState == CTENT_PLACE && m_fFrame > m_fFrameEnd)
		{
			m_bIsFrameStop = true;
			m_eTentCurState = CTENT_IDLE;
		}

		else if (m_eTentCurState == CTENT_IDLE)
		{
			m_bIsFrameStop = false;
			if (m_fFrame > m_fFrameEnd)
			{
				m_fFrame = 0.0f;
			}
		}

		if (m_bIsEnter&&!m_bIsDestroy)
		{
			m_eTentCurState = CTENT_ENTER;
			if (m_fFrame > m_fFrameEnd)
			{
				m_eTentCurState = CTENT_IDLE;
				m_bIsEnter = false;
				m_fFrame = 0.0f;
			}
		}
		if (m_bIsHit&&!m_bIsDestroy&&!m_bIsEnter)
		{

			m_eTentCurState = CTENT_HIT;
			if (m_fFrame > m_fFrameEnd)
			{
				m_eTentCurState = CTENT_IDLE;
				m_bIsHit = false;
				m_fFrame = 0.0f;
			}
		}





	}




}
void CTent::Install_Obj()
{
	if (!m_bInstall) return;

	auto& vecTerrain = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::TILE)[0];
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(scenemgr::Get_CurScene()->GetTerrainObject()->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	_vec3 vPos = m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, vecTerrain->GetTransForm());

	vPos.y = 1.7f;
	m_pTransForm->Set_Pos(vPos);

	if (Engine::GetMouseState(DIM_LB) == eKEY_STATE::TAP) // 설치 완료
	{
		m_bInstall = false;
		m_bIsDrop = true;
		auto vecMouse = scenemgr::Get_CurScene()->GetMouseObject();
		CMouse* pMouse = dynamic_cast<CMouse*>(vecMouse);
		pMouse->Set_Install(false);
		Engine::PlaySound_W(L"Obj_Tent_Craft.mp3", SOUND_EFFECT, 0.2f);
		CSlotMgr::GetInstance()->Remove_InvenItem(m_iSlotNum);
	}
}

CTent* CTent::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall)
{
	CTent* pInstance = new CTent(pGraphicDev, bInstall);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTent::Free()
{
	CGameObject::Free();
}


