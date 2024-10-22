#include "stdafx.h"
#include "CCookingPot.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include <Mouse.h>
#include "SlotMgr.h"
#include <Cook.h>
#include <ItemBasic.h>

CCookingPot::CCookingPot(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall)
	: CResObject(pGraphicDev), m_bInstall(bInstall)
{
}

CCookingPot::CCookingPot(const CCookingPot& rhs)
	: CResObject(rhs.m_pGraphicDev), m_bInstall(rhs.m_bInstall)
{

}

CCookingPot::~CCookingPot()
{
}

HRESULT CCookingPot::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos;

	m_pTransForm->Set_Scale(_vec3(2.f, 2.f, 2.f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.f, vPos.z);


	m_eObject_id = COOKING_POT;
	m_eCookingpotCurState = COOKINGPOT_DEFAULT;
	
	m_fFrame = 0.0f;

	m_Stat.strObjName = L"요리 솥";
	return S_OK;
}

_int CCookingPot::Update_GameObject(const _float& fTimeDelta)
{
	Install_Obj();

	if (GetAsyncKeyState('6')) // 횃불
	{
		Set_Cooking(false);
	}

	if (GetAsyncKeyState('7')) // 횃불
	{
		Set_Cooking(true);
	}



	//if (GetAsyncKeyState('8')) // 횃불
	//{
	//	Set_Hit();
	//	Set_Empty();
	//}
	//if (GetAsyncKeyState('9')) // 횃불
	//{
	//	Set_Hit();
	//	Set_Full();
	//}

	//if (GetAsyncKeyState('0')) // 횃불
	//{
	//	Set_Burnt();
	//}

	if (!m_bIsFrameStop)
	{
		m_fFrame += m_fFrameEnd * fTimeDelta;
	}

	if (m_bIsCooking)
	{

		m_fTimeChek += fTimeDelta;
	}


	if (m_fTimeChek >= m_MaxfTimeChek)
	{
		
		_vec3 vSlot;
		//요리끝
		if (CSlotMgr::GetInstance()->Check_AddItem(m_pGraphicDev, m_bSuccess ? L"Meatballs" : L"Wetgoop", &vSlot))
		{
			Engine::PlaySound_W(L"Obj_Cookingpot_Finish.mp3", SOUND_EFFECT, 0.3f);
			
			_vec3 vPos;
			m_pTransForm->Get_Info(INFO_POS, &vPos);
			CItem* pItem = CItemBasic::Create(m_pGraphicDev, m_bSuccess ? L"Meatballs" : L"Wetgoop");
			CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::ITEM, pItem);
			pItem->GetTransForm()->Set_Pos(vPos);
			dynamic_cast<CItemBasic*>(pItem)->Pickup_Item(vSlot);
		}
		
		m_bIsCooking=false;
		m_fTimeChek = 0.f;
	}



	Change_Frame_Event();

	if (Engine::GetMouseState(DIM_RB) == eKEY_STATE::TAP) // 요리도구 UI 열기
	{
		auto& vecUI = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI);
		for (auto& iter : vecUI)
		{
			auto vecMouse = scenemgr::Get_CurScene()->GetMouseObject();
			CMouse* pMouse = dynamic_cast<CMouse*>(vecMouse);
			_vec3 vPos;
			m_pTransForm->Get_Info(INFO_POS, &vPos);
			if (Engine::Collision_Mouse_Object(pMouse->Get_MouseRayPos(), pMouse->Get_MouseRayDir(), vPos, m_pTransForm->Get_Scale()))
			{
				if (iter->Get_State().strObjName == L"요리 도구")
				{
					CCook* pCook = dynamic_cast<CCook*>(iter);
					pCook->IsShow(true);
				}
			}
		}
	}



	//Engine::Update_Sound(_vec3{ 1,1,1 }, get<0>(Get_Info_vec()), get<1>(Get_Info_vec()), get<2>(Get_Info_vec()), get<3>(Get_Info_vec()), SOUND_EFFECT, 1);
	Engine::Update_Sound(_vec3{ 1,1,1 }, get<0>(Get_Info_vec()), get<1>(Get_Info_vec()), get<2>(Get_Info_vec()), get<3>(Get_Info_vec()), SOUND_EFFECT_CONTINUE_CH1, 1);
	Engine::Update_Sound(_vec3{ 1,1,1 }, get<0>(Get_Info_vec()), get<1>(Get_Info_vec()), get<2>(Get_Info_vec()), get<3>(Get_Info_vec()), SOUND_EFFECT_CONTINUE_CH2, 1);
	Engine::Update_Sound(_vec3{ 1,1,1 }, get<0>(Get_Info_vec()), get<1>(Get_Info_vec()), get<2>(Get_Info_vec()), get<3>(Get_Info_vec()), SOUND_EFFECT_CONTINUE_CH3, 1);

	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CCookingPot::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();

	Check_FrameState();
	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CCookingPot::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	if (m_bShader || m_bInstall)
	{
		m_pCookingpotTexCom[m_eCookingpotCurState]->Set_Texture(m_pShaderCom, "g_Texture", m_fFrame);
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

		m_pCookingpotTexCom[m_eCookingpotCurState]->Set_Texture((_uint)m_fFrame);

		FAILED_CHECK_RETURN(SetUp_Material(), );
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}



HRESULT CCookingPot::Add_Component()
{
	CComponent* pComponent = nullptr;


	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	
	pComponent = m_pCookingpotTexCom[COOKINGPOT_IDLE_EMPTY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Idle_empty"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Idle_empty", pComponent });


	pComponent = m_pCookingpotTexCom[COOKINGPOT_IDLE_FULL] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Idle_full"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Idle_full", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_COOKING_LOOP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_loop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_loop", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_BURNT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Burnt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Burnt", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_PLACE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Place"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Place", pComponent });


	pComponent = m_pCookingpotTexCom[COOKINGPOT_HIT_EMPTY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Hit_Empty"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Hit_Empty", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_DEFAULT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Default"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Default", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_HIT_COOKING] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Hit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Hit", pComponent });

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

void CCookingPot::Check_FrameState()
{
	if (m_eCookingpotCurState != m_eCookingpotPrevState)
	{


		switch (m_eCookingpotCurState)
		{
		case CCookingPot::COOKINGPOT_IDLE_EMPTY:
			
			m_fFrameEnd = 0.0f;
			break;
		case CCookingPot::COOKINGPOT_IDLE_FULL:
			m_fFrameEnd = 0.0f;
			break;
		case CCookingPot::COOKINGPOT_COOKING_LOOP:
			//Engine::PlayBGM(L"Obj_Cookingpot_Boil.mp3", SOUND_EFFECT_CONTINUE_CH1);
			Engine::SpatialPlay_Sound(L"Obj_Cookingpot_Boil.mp3", SOUND_EFFECT_CONTINUE_CH1);
			Engine::SpatialPlay_Sound(L"Obj_Cookingpot_Rattle_1.mp3", SOUND_EFFECT_CONTINUE_CH2);
			Engine::SpatialPlay_Sound(L"Obj_Cookingpot_Rattle_2.mp3", SOUND_EFFECT_CONTINUE_CH3);
	
			m_fFrameEnd = 6.0f;
			break;
		case CCookingPot::COOKINGPOT_BURNT:
			m_fFrameEnd=0.0f;
			break;
		case CCookingPot::COOKINGPOT_PLACE:
			
			m_fFrameEnd = 8.0f;
			break;
		case CCookingPot::COOKINGPOT_HIT_EMPTY:
			m_fFrameEnd = 9.0f;
			break;
		case CCookingPot::COOKINGPOT_HIT_COOKING:
			m_fFrameEnd = 9.0f;
			break;
		case CCookingPot::COOKINGPOT_DEFAULT:
			m_fFrameEnd = 0.0f;
			break;
		default:
			break;
		}
		m_eCookingpotPrevState = m_eCookingpotCurState;
		m_fFrame = 0.0f;
	}
}

void CCookingPot::Change_Frame_Event()
{
	if (m_bIsDrop)
	{
		if (m_eCookingpotCurState == COOKINGPOT_DEFAULT)
		{
			//공간 음향
			//Engine::SpatialPlay_Sound(L"Obj_Cookingpot_Craft.mp3", SOUND_EFFECT);
			Engine::PlaySound_W(L"Obj_Cookingpot_Craft.mp3", SOUND_EFFECT, 0.3f);
			
			m_eCookingpotCurState = COOKINGPOT_PLACE;
		}

		//솥이 타버리면 복구 불가
		if (m_bIsBurnt)
		{
			m_eCookingpotCurState = COOKINGPOT_BURNT;
			return;

		}

		//솥이 떨어지는 모션이 끝나면 프레임 고정
		if (m_eCookingpotCurState== COOKINGPOT_PLACE&&m_fFrame>m_fFrameEnd)
		{
			
			m_bIsFrameStop = true;
			m_eCookingpotCurState = COOKINGPOT_COOKING_LOOP;
		}
		
		//솥이 떨어지는 모션이 끝나고 그리고 요리가 시작 되었을 때 프레임 반복
		else if (m_eCookingpotCurState == COOKINGPOT_COOKING_LOOP&& m_bIsCooking)
		{
			
			m_bIsFrameStop= false;
			if (m_fFrame > m_fFrameEnd)
			{
				m_fFrame=0.0f;
			}
		}

		//솥이 떨어지는 모션이 끝나고 요리가 시작되지 않았을 때 Empty로 돌아감
		else if(m_eCookingpotCurState == COOKINGPOT_COOKING_LOOP && !m_bIsCooking)
		{
			
			Engine::StopSound(SOUND_EFFECT_CONTINUE_CH1);
			Engine::StopSound(SOUND_EFFECT_CONTINUE_CH2);
			Engine::StopSound(SOUND_EFFECT_CONTINUE_CH3);
			m_bIsFrameStop = true;
			m_eCookingpotCurState = COOKINGPOT_IDLE_EMPTY;
		}


		//요리를 다시 시작하려 할 때
		if (m_eCookingpotCurState == COOKINGPOT_IDLE_EMPTY && m_bIsCooking)
		{
			//공간 음향
			//Engine::SpatialPlay_Sound(L"Obj_Cookingpot_Boil.mp3", SOUND_EFFECT_CONTINUE_CH1);
			//Engine::SpatialPlay_Sound(L"Obj_Cookingpot_Rattle_1.mp3", SOUND_EFFECT_CONTINUE_CH2);
			//Engine::SpatialPlay_Sound(L"Obj_Cookingpot_Rattle_2.mp3", SOUND_EFFECT_CONTINUE_CH3);
			
			m_eCookingpotCurState = COOKINGPOT_COOKING_LOOP;
			
		}

		if (m_eCookingpotCurState == COOKINGPOT_IDLE_EMPTY && !m_bIsCooking || m_eCookingpotCurState == COOKINGPOT_HIT_EMPTY)
		{
			//솥에 요리가 비어있고, 요리 중이 아닐 때에만 피해 모션		또는 //프레임이 한번 씹혔을 때의 방지 다시 초기화를 위한
			if (m_bIsHit && m_bIsEmpty  || m_eCookingpotCurState == COOKINGPOT_HIT_EMPTY)
			{

				m_eCookingpotCurState = COOKINGPOT_HIT_EMPTY;
				m_bIsFrameStop = false;
				if (m_fFrame > m_fFrameEnd)
				{
					m_fFrame = 0.0f;
					m_bIsHit = false;
					m_eCookingpotCurState = COOKINGPOT_IDLE_EMPTY;
				}
			}

		}

		
		//솥에 요리가 차있고, 요리 중일 때에만 피해 모션				또는 //프레임이 한번 씹혔을 때의 다시 초기화를 위한 방지
		else if (m_bIsHit && m_bIsFull&& m_bIsCooking|| m_eCookingpotCurState== COOKINGPOT_HIT_COOKING)
		{
			
			m_eCookingpotCurState= COOKINGPOT_HIT_COOKING;
			m_bIsFrameStop = false;
			if (m_fFrame > m_fFrameEnd)
			{
				m_fFrame = 0.0f;
				m_bIsHit = false;
				m_eCookingpotCurState = COOKINGPOT_COOKING_LOOP;
		
			}

			
		}






	}


}

void CCookingPot::Install_Obj()
{
	if (!m_bInstall) return;

	auto& vecTerrain = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::TILE)[0];
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(scenemgr::Get_CurScene()->GetTerrainObject()->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	_vec3 vPos = m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, vecTerrain->GetTransForm());

	vPos.y = 1.f;
	m_pTransForm->Set_Pos(vPos);

	if (Engine::GetMouseState(DIM_LB) == eKEY_STATE::TAP) // 설치 완료
	{
		m_bInstall = false;
		m_bIsDrop= true;
		auto vecMouse = scenemgr::Get_CurScene()->GetMouseObject();;
		CMouse* pMouse = dynamic_cast<CMouse*>(vecMouse);
		pMouse->Set_Install(false);

		CSlotMgr::GetInstance()->Remove_InvenItem(m_iSlotNum);
		
	}
}




CCookingPot* CCookingPot::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall)
{
	CCookingPot* pInstance = new CCookingPot(pGraphicDev, bInstall);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCookingPot::Free()
{
	CGameObject::Free();
}


