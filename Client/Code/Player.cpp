#include "stdafx.h"
#include "..\Header\Player.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "CInven.h"
#include "Monster.h"
#include "ResObject.h"
#include "Component.h"
#include "Layer.h"
#include "Scene.h"
#include "CItem.h"
#include "Ghost.h"
#include "Rebirth.h"
#include "DeerClops.h"
#include "CPigHouse.h"
//Manager
#include "SlotMgr.h"
#include <ItemBasic.h>

//TestPSW---------------------------------------------
#include <CBonfire.h>
#include<CTent.h>
#include <MainApp.h>
//TestPSW---------------------------------------------

//TestParticle
#include "CDustParticle.h"
#include "CRockSmoke.h"
#include "CTreeLeafFall.h"

//EffectAnimationUI
#include "CPlayerHitEffectUI.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_bAttack(false)
	, m_iLightNum(++CMainApp::g_iLightNum)
	, m_bTent(false)
	, m_bHit(false)
	, m_iLightRange(7.f)
{
}
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strName)
	: Engine::CGameObject(pGraphicDev, _strName)
	, m_bAttack(false)
	, m_iLightNum(++CMainApp::g_iLightNum),
	m_bTent(false)
	, m_bHit(false)
	, m_iLightRange(7.f)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: Engine::CGameObject(rhs)
	, m_bAttack(rhs.m_bAttack)
	, m_iLightNum(rhs.m_iLightNum)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_eCurState = IDLE;
	m_ePreState = STATE_END;
	m_eCurLook = LOOK_DOWN;

	m_eCurWeapon = UNARMED;
	m_ePreWeapon = WEAPON_END;
	m_bFirstStart = false;
	m_Dirchange = false;
	m_KeyLock = false;
	m_bFrameLock = false;
	m_bossAttack = false;
	m_vPlayerActing = false;
	m_bIsRoadScene - false;
	m_Ghost = nullptr;
	m_bWakeupTrig = false;
	m_TargetObject = RSOBJ_END;
	m_fFrameEnd = 22;
	m_fFrameSpeed = 0.f;
	m_fCollisionRadius = 0.5f;
	Set_Stat();
	return S_OK;
}
Engine::_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Update_State(fTimeDelta); // 플레이어 State 매 프레임마다 업데이트

	if (scenemgr::Get_CurScene()->Get_Scene_Name() == L"ROAD" && !m_bIsRoadScene)
	{
		m_bIsRoadScene = true;
		m_fCollisionRadius = 0.25f;
	}
	else if (scenemgr::Get_CurScene()->Get_Scene_Name() != L"ROAD" && m_bIsRoadScene)
	{
		m_bIsRoadScene = false;
		m_fCollisionRadius = 0.5f;
	}
	


	if (!m_bFrameLock)      //프레임 락이 걸리면 프레임이 오르지 않음
		m_fFrame += m_fFrameSpeed * fTimeDelta;
	_int iResult = Die_Check();


	

	if (m_fFrameEnd <= m_fFrame)      //프레임이 끝에 다다르면 진입
	{
		if (m_KeyLock == true && !m_Stat.bDead)         //KeyLock을 풀고 IDLE 상태로 만듦
		{
			m_KeyLock = false;
			m_eCurState = IDLE;
			m_bHit = false;
		}
		if (m_vPlayerActing)
			m_vPlayerActing = false;
		if (m_bAttack)
			m_bAttack = false;
		if (m_bossAttack)
			m_bossAttack = false;
		m_fFrame = 0.f;
	}

	if (m_eCurState == MOVE || m_eCurState == TORCH_RUN)
	{
		if (m_fFrame >= 2.f && m_fFrame < 2.2f)
			Engine::PlaySound_W(L"Player_Foot.mp3", SOUND_PLAYER, 0.2f);
		else if (m_fFrame > 5.f && m_fFrame < 5.2f)
			Engine::PlaySound_W(L"Player_Foot_2.mp3", SOUND_PLAYER, 0.2f);
	}
	
	First_Start();
	
	if (!m_KeyLock && !m_Stat.bDead && m_bWakeupTrig)         //특정 행동에는 KeyLock 을 걸어서 행동중에 다른 행동을 못하게 함
	{
		if (!m_bIsRoadScene)
			Key_Input(fTimeDelta);
		else
			Ket_Input_Road(fTimeDelta);
	}
	else if (m_Stat.bDead)
		Rebirth();
	

	Weapon_Change();
	Check_State();
	Look_Change();
	//Set_Scale();
	Fire_Light(); // 횃불 모션 시 켜짐 / 꺼짐 추가해야함
	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CPlayer::LateUpdate_GameObject()
{
	_vec3 vPos;
	m_pTransForm->BillBoard();
	__super::LateUpdate_GameObject();

	m_pTransForm->Get_Info(INFO::INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CPlayer::Render_GameObject()
{
	if (m_bTent)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pTextureCom[m_ePreLook][m_ePreState]->Set_Texture((_uint)m_fFrame);

	FAILED_CHECK_RETURN(SetUp_Material(), );

	if (m_Dirchange)
	{
		m_pReverseCom->Render_Buffer();
	}
	else if (!m_Dirchange)
	{
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CPlayer::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

#pragma region TEXCOM

	//IDLE
	pComponent = m_pTextureCom[LOOK_DOWN][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_up", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_side", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_side", pComponent });


	//RUN(UNARMED)
	pComponent = m_pTextureCom[LOOK_DOWN][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_side", pComponent });

	//Build
	pComponent = m_pTextureCom[LOOK_DOWN][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_side", pComponent });

	//Pickup
	pComponent = m_pTextureCom[LOOK_UP][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_up", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_down", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_side", pComponent });


	//Hit
	pComponent = m_pTextureCom[LOOK_DOWN][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_side", pComponent });


	//Attack
	pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_side", pComponent });

	//Torch(IDLE)
	pComponent = m_pTextureCom[LOOK_DOWN][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_side", pComponent });

	//Torch(Run)
	pComponent = m_pTextureCom[LOOK_DOWN][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_side", pComponent });

	//Axe pre
	pComponent = m_pTextureCom[LOOK_DOWN][AXE_CHOP_PRE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_preaxe_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_preaxe_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][AXE_CHOP_PRE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_preaxe_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_preaxe_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][AXE_CHOP_PRE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_preaxe_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_preaxe_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][AXE_CHOP_PRE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_preaxe_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_preaxe_side", pComponent });



	//Hammer
	pComponent = m_pTextureCom[LOOK_DOWN][HAMMERING] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_hammer_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_hammer_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][HAMMERING] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_hammer_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_hammer_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][HAMMERING] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_hammer_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_hammer_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][HAMMERING] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_hammer_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_hammer_side", pComponent });

	//Pick
	pComponent = m_pTextureCom[LOOK_DOWN][PICKING_OBJECT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_picking_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_picking_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][PICKING_OBJECT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_picking_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_picking_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][PICKING_OBJECT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_picking_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_picking_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][PICKING_OBJECT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_picking_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_picking_side", pComponent });


	//Spear
	pComponent = m_pTextureCom[LOOK_DOWN][SPEAR_ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_spear_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_spear_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][SPEAR_ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_spear_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_spear_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][SPEAR_ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_spear_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_spear_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][SPEAR_ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_spear_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_spear_side", pComponent });


	//Other
	pComponent = m_pTextureCom[LOOK_DOWN][FALLDOWN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_falldown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_falldown", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][WAKEUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_wakeup"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_wakeup", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][EAT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_eat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_eat", pComponent });



	pComponent = m_pTextureCom[LOOK_DOWN][DIALOG] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_dialog"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_dialog", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][SLEEP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_sleep"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_sleep", pComponent });


	pComponent = m_pTextureCom[LOOK_DOWN][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_die"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_die", pComponent });


	pComponent = m_pTextureCom[LOOK_DOWN][REBIRTH] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_research"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_research", pComponent });

#pragma endregion TEXCOM

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });


	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strName)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev, _strName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{

	_vec3		vDir, vRight, vCurPos;
	auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();

	CTerrainTex* pTerrainTex = dynamic_cast<CTerrainTex*>(pTerrain->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	m_pTransForm->Get_Info(INFO_LOOK, &vDir);
	m_pTransForm->Get_Info(INFO_RIGHT, &vRight);


	if (GetAsyncKeyState('W'))
	{
		D3DXVec3Normalize(&vDir, &vDir);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vDir, m_Stat.fSpeed, fTimeDelta);
		m_pTransForm->Get_Info(INFO_POS, &vCurPos);
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransForm->Move_Pos(&vDir, -m_Stat.fSpeed, fTimeDelta);

		if (m_ePreWeapon == TORCH)
		{
			m_eCurState = TORCH_RUN;
		}
		else
		{
			m_eCurState = MOVE;
		}
		m_eCurLook = LOOK_UP;
	}
	if (GetAsyncKeyState('S'))
	{ //f
		D3DXVec3Normalize(&vDir, &vDir);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vDir, -m_Stat.fSpeed, fTimeDelta);
		m_pTransForm->Get_Info(INFO_POS, &vCurPos);
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransForm->Move_Pos(&vDir, m_Stat.fSpeed, fTimeDelta);

		if (m_ePreWeapon == TORCH)
		{
			m_eCurState = TORCH_RUN;
		}
		else
		{
			m_eCurState = MOVE;
		}
		m_eCurLook = LOOK_DOWN;

	}
	if (GetAsyncKeyState('A'))
	{
		D3DXVec3Normalize(&vRight, &vRight);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vRight, -m_Stat.fSpeed, fTimeDelta);
		m_pTransForm->Get_Info(INFO_POS, &vCurPos);
		vCurPos.x += 0.5f;
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransForm->Move_Pos(&vRight, m_Stat.fSpeed, fTimeDelta);


		if (m_ePreWeapon == TORCH)
		{
			m_eCurState = TORCH_RUN;
		}
		else
		{
			m_eCurState = MOVE;
		}
		m_eCurLook = LOOK_LEFT;
		/*m_pTransForm->Rotation(ROT_Y, D3DXToRadian(90.f * fTimeDelta));*/
	}
	if (GetAsyncKeyState('D'))
	{
		D3DXVec3Normalize(&vRight, &vRight);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vRight, m_Stat.fSpeed, fTimeDelta);
		m_pTransForm->Get_Info(INFO_POS, &vCurPos);
		vCurPos.x -= 0.5f;
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransForm->Set_Pos(vCurPos);


		if (m_ePreWeapon == TORCH)
		{
			m_eCurState = TORCH_RUN;
		}
		else
		{
			m_eCurState = MOVE;
		}
		m_eCurLook = LOOK_RIGHT;

	}


	if (!GetAsyncKeyState('W') &&              //이동중이지 않을 때 IDLE로 변경
		!GetAsyncKeyState('S') &&
		!GetAsyncKeyState('A') &&
		!GetAsyncKeyState('D') && !m_vPlayerActing)
	{
		if (m_ePreWeapon == TORCH)				//횃불을 들고 있을 시에는 횃불모션으로 변경
		{
			m_eCurState = TORCH_IDLE;
		}
		else
			m_eCurState = IDLE;
	}



	if (GetAsyncKeyState('Z'))
	{
		//Find_NeerObject: 못찾았을경우 nullptr반환

		CGameObject* findObj = Find_NeerObject(m_Stat.fAggroRange, eOBJECT_GROUPTYPE::RESOURCE_OBJECT);
		if (nullptr != findObj)
		{
			_vec3 vPos;
			m_pTransForm->Get_Info(INFO_POS, &vPos);
			findObj->GetTransForm()->Get_Info(INFO_POS, &m_vTargetPos);
			m_TargetObject = dynamic_cast<CResObject*>(findObj)->Get_Resourse_ID();
			m_vTargetDir = m_vTargetPos - vPos;
			m_vTargetDir.y = 0.f;
			if (Collision_Circle(findObj))
			{
				ResObj_Mining(m_TargetObject, findObj);
			}
			else
			{
				m_eCurLook = m_pTransForm->For_Player_Direction(&m_vTargetDir, m_Stat.fSpeed, fTimeDelta);
				if (m_ePreWeapon == TORCH)
				{
					m_eCurState = TORCH_RUN;
				}
				else
					m_eCurState = MOVE;
			}


		}
	}
	if (KEY_AWAY(DIK_SPACE))// KEY_TAP(누르는시점) , KEY_AWAY (키를떼는시점), KEY_NONE(키를안누른상태), KEY_HOLD(키를누르고있는상태)
	{
		//Find_NeerObject: 못찾았을경우 nullptr반환
		CGameObject* findObj = Find_NeerObject(m_Stat.fATKRange, eOBJECT_GROUPTYPE::ITEM);

		if (nullptr != findObj)
		{
			_vec3 vSlotPos;
			if (CSlotMgr::GetInstance()->Check_AddItem(m_pGraphicDev, findObj->GetObjName(), &vSlotPos));
			{
				m_eCurState = PICKUP;
				dynamic_cast<CItemBasic*>(findObj)->Pickup_Item(vSlotPos);
				m_vPlayerActing = true;
			}
		}
	}

	if (GetAsyncKeyState('F')) // 공격
	{
		if (m_ePreWeapon == SPEAR)
		{
			m_eCurState = SPEAR_ATTACK;
		}
		else
		{
			m_eCurState = ATTACK;
		}
		CGameObject* findObj = Find_NeerObject(m_Stat.fAggroRange, eOBJECT_GROUPTYPE::MONSTER);
		if (nullptr != findObj && !findObj->IsDelete()
			&& Collision_Circle(findObj) &&!m_bAttack)
		{
			dynamic_cast<CMonster*>(findObj)->Set_Attack(m_Stat.fATK);
			m_bAttack = true;

		}

		CGameObject* boss = Find_NeerObject(m_Stat.fAggroRange, eOBJECT_GROUPTYPE::BOSS);
		if (nullptr != boss && !boss->IsDelete()
			&& Collision_Circle(boss) &&!m_bossAttack)
		{
			dynamic_cast<CDeerClops*>(boss)->Set_Attack(m_Stat.fATK);
			m_bossAttack = true;
		}


	}

#pragma region PSWTEST
	//PSW Test---------------------------------------------------------

	//if (GetAsyncKeyState('1')) //텐트 입장
	//{
	//	decltype(auto)	Test = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::OBJECT);
	//	for (auto& object : Test)
	//	{
	//		if(object->IsDelete())
	//			continue;
	//		if (object->Get_State().strObjName==L"텐트")
	//		{

	//			dynamic_cast<CTent*>(object)->Set_Enter();
	//			

	//		}

	//	}
	//}
	//if (GetAsyncKeyState('2')) //텐트 파괴
	//{
	//	decltype(auto)	Test = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::OBJECT);
	//	for (auto& object : Test)
	//	{
	//		if (object->IsDelete())
	//			continue;
	//		if (object->Get_State().strObjName == L"텐트")
	//		{

	//			dynamic_cast<CTent*>(object)->Set_Destroy();
	//			//해당 함수 호출하면 다시는 못지음 (Enter모션, Hit모션, Burnt모션 다 불가능)

	//		}

	//	}
	//}
	//if (GetAsyncKeyState('3')) //텐트가 맞았을 때, 이거는 다른 곳에서 해도 됨 떄린 객체에게 시켜야 할 듯
	//{
	//	decltype(auto)	Test = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::OBJECT);
	//	for (auto& object : Test)
	//	{
	//		if (object->IsDelete())
	//			continue;
	//		if (object->Get_State().strObjName == L"텐트")
	//		{

	//			dynamic_cast<CTent*>(object)->Set_Hit();
	//			

	//		}

	//	}
	//}
	//if (GetAsyncKeyState('4')) //텐트가 타버렸 을  때,  이거는 텐트가 불 탔다는 스텟을 가졌으면  텐트안에서 처리 가능
	//{
	//	decltype(auto)	Test = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::OBJECT);
	//	for (auto& object : Test)
	//	{
	//		if (object->IsDelete())
	//			continue;
	//		if (object->Get_State().strObjName == L"텐트")
	//		{

	//			dynamic_cast<CTent*>(object)->Set_Burnt();
	//			//해당 함수 호출하면 다시는 못지음 (계속 탄 상태로 남아 있음)

	//		}

	//	}
	//}






	//PSW Test---------------------------------------------------------

#pragma endregion PSWTEST





}

void CPlayer::Ket_Input_Road(const _float& fTimeDelta)
{
	m_eCurWeapon = TORCH;
	m_iLightRange = 3.f;

	_vec3		vDir, vRight;
	m_pTransForm->Get_Info(INFO_LOOK, &vDir);
	m_pTransForm->Get_Info(INFO_RIGHT, &vRight);

	if (GetAsyncKeyState('W'))
	{
		D3DXVec3Normalize(&vDir, &vDir);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vDir, m_Stat.fSpeed, fTimeDelta);
		m_eCurState = TORCH_RUN;
		m_eCurLook = LOOK_UP;
	}
	if (GetAsyncKeyState('S'))
	{
		D3DXVec3Normalize(&vDir, &vDir);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vDir, -m_Stat.fSpeed, fTimeDelta);
		m_eCurState = TORCH_RUN;
		m_eCurLook = LOOK_DOWN;
	}
	if (GetAsyncKeyState('A'))
	{
		D3DXVec3Normalize(&vRight, &vRight);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vRight, -m_Stat.fSpeed, fTimeDelta);
		m_eCurState = TORCH_RUN;
		m_eCurLook = LOOK_LEFT;
	}
	if (GetAsyncKeyState('D'))
	{
		D3DXVec3Normalize(&vRight, &vRight);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vRight, m_Stat.fSpeed, fTimeDelta);
		m_eCurState = TORCH_RUN;
		m_eCurLook = LOOK_RIGHT;
	}

	if (!GetAsyncKeyState('W') &&
		!GetAsyncKeyState('S') &&
		!GetAsyncKeyState('A') &&
		!GetAsyncKeyState('D'))
	{
		m_eCurState = TORCH_IDLE;
	}

}

HRESULT CPlayer::SetUp_Material()
{
	D3DMATERIAL9			tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tMtrl.Power = 0.f;
	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}


void CPlayer::Check_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_fFrameSpeed = 22.f;
			m_fFrameEnd = 22;
			break;
		case HIT:
			Hit_Sound();
			m_fFrameEnd = 7;
			m_fFrameSpeed = 15.f;
			m_KeyLock = true;
			break;
		case BUILD:
			m_fFrameSpeed = 11.f;
			m_fFrameEnd = 6;
			break;
		case ATTACK:
			m_fFrameSpeed = 20.f;
			m_fFrameEnd = 11;
			m_KeyLock = true;
			break;
		case FALLDOWN:
			m_fFrameSpeed = 15.f;
			m_fFrameEnd = 8;
			m_KeyLock = true;
			m_eCurLook = LOOK_DOWN;
			break;
		case PICKUP:
			m_fFrameSpeed = 20.f;
			m_KeyLock = true;
			m_fFrameEnd = 6;
			break;
		case EAT:
			m_fFrameSpeed = 14.f;
			m_fFrameEnd = 35;
			m_KeyLock = true;
			m_eCurLook = LOOK_DOWN;
			break;
		case MOVE:
			m_fFrameSpeed = 9.f;
			m_fFrameEnd = 6;
			break;
		case DIALOG:
			m_KeyLock = true;
			Dialog_Sound();
			m_eCurLook = LOOK_DOWN;
			m_fFrameEnd = 17;
			m_fFrameSpeed = 17.f;
			break;
		case TORCH_IDLE:
			m_fFrameSpeed = 22.f;
			m_fFrameEnd = 22;
			break;
		case TORCH_RUN:
			m_fFrameSpeed = 7.f;
			m_fFrameEnd = 6;
			break;
		case PICKING_OBJECT:
			m_fFrameSpeed = 14.f;
			m_fFrameEnd = 9;
			break;
		case AXE_CHOP_PRE:
			m_fFrameSpeed = 20.f;
			m_fFrameEnd = 15;
			break;
		case HAMMERING:
			m_fFrameSpeed = 14.f;
			m_fFrameEnd = 9;
			break;
		case SPEAR_ATTACK:
			m_KeyLock = true;
			m_fFrameSpeed = 20.f;
			m_fFrameEnd = 8;
			break;
		case SLEEP:
			m_fFrameSpeed = 10.f;
			m_fFrameEnd = 13;
			m_eCurLook = LOOK_DOWN;
			break;
		case WAKEUP:
			m_fFrameSpeed = 12.f;
			m_fFrameEnd = 32;
			m_eCurLook = LOOK_DOWN;
			break;
		case DEAD:
			m_fFrameSpeed = 19.f;
			Engine::PlaySound_W(L"wilson_Vocie_Death.mp3", SOUND_PLAYER_VOICE, 0.3f);
			m_fFrameEnd = 19;
			m_KeyLock = true;
			m_eCurLook = LOOK_DOWN;
			break;
		case REBIRTH:
			m_fFrameSpeed = 17.f;
			m_fFrameEnd = 17;
			m_KeyLock = true;
			m_eCurLook = LOOK_DOWN;
			break;
		}
		m_ePreState = m_eCurState;
		m_fFrame = 0.f;
	}
	else
		return;

}

void CPlayer::Set_Stat()
{
	m_Stat.fHP = 200.f;
	m_Stat.fMxHP = 200.f;
	m_Stat.fSpeed = 4.f;
	m_Stat.fATK = 1.f;
	m_Stat.fATKRange = 1.f;
	m_Stat.fAggroRange = 5.f;
	m_Stat.fHungry = 200.f;
	m_Stat.fMaxHungry = 200.f;
	m_Stat.fMental = 200.f;
	m_Stat.fMaxMental = 200.f;
	m_Stat.bDead = false;

}

void CPlayer::Set_Attack(int _Atk)
{
	if (!m_Stat.bDead && m_Stat.fHP > 0 && !m_bHit)
	{
		m_Stat.fHP -= _Atk;
		m_eCurState = HIT;
		m_KeyLock = true;
		m_bHit = true;
		CGameObject* pEffectObj = CPlayerHitEffectUI::Create(m_pGraphicDev, L"PLAYER_HIT_EFFECT", _vec3(WINCX / 2.f, WINCY / 2.f, 0.f), _vec3(WINCX * 0.5f, WINCY * 0.5f, 0.f));
		CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::EFFECT, pEffectObj);
	}
}

void CPlayer::Weapon_Change()
{
	if (m_ePreWeapon != m_eCurWeapon)
	{
		switch (m_eCurWeapon)
		{
		case UNARMED:
			m_Stat.fATK = 20;
			m_Stat.fATKRange = 1.f;
			break;
		case AXE:
			m_Stat.fATK = 20;
			m_Stat.fATKRange = 1.f;
			break;
		case TORCH:
			Engine::PlayTorch(L"Torch.mp3", 0.2f);
			if (m_eCurState = IDLE)
			{
				m_eCurState = TORCH_IDLE;
			}
			m_Stat.fATK = 20;
			m_Stat.fATKRange = 1.f;
			break;
		case HAMMER:
			m_Stat.fATK = 20;
			m_Stat.fATKRange = 1.f;
			break;
		case PICK:
			m_Stat.fATK = 20;
			m_Stat.fATKRange = 1.f;
			break;
		case SPEAR:
			m_Stat.fATK = 50;
			m_Stat.fATKRange = 2.f;
			break;
		}
		if (m_ePreWeapon == TORCH) Engine::StopSound(SOUND_TORCH);

		m_ePreWeapon = m_eCurWeapon;
	}

}

void CPlayer::Look_Change()
{
	if (m_ePreLook != m_eCurLook)
	{
		if (m_eCurLook == LOOK_LEFT)
		{
			m_Dirchange = true;
		}
		else
		{
			m_Dirchange = false;
		}

		m_ePreLook = m_eCurLook;
	}

}

void CPlayer::ResObj_Mining(RESOBJID _ObjID, CGameObject* _Obj)
{
	switch (_ObjID)
	{
	case ROCK:

		if (m_ePreWeapon == PICK)
		{
			if ((m_fFrameEnd - 1) < m_fFrame && !m_vPlayerActing)
			{
				Engine::PlaySound_W(L"wilson_Hit_Rock_by_Axe.mp3", SOUND_ROCK, 0.2f);
				Rock_Sound();
				dynamic_cast<CResObject*>(_Obj)->Set_Attack();
				m_vPlayerActing = true;
				CGameObject* pGameObject = CRockSmoke::Create(m_pGraphicDev, L"Rocks_0", 4, _Obj->GetTransForm()->Get_Pos(), 0.09f, 0.16f, 15.f);
				CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::PARTICLE, pGameObject);
			}
			m_eCurState = PICKING_OBJECT;
		}
		else
		{
			m_eCurState = DIALOG;		//곡괭이가 없으면 안된다는 대사 추가
			m_vPlayerActing = true;
		}
		break;
	case TREE:
		if (m_ePreWeapon == AXE)
		{
			if (7.f < m_fFrame && !m_vPlayerActing)
			{
				Engine::PlaySound_W(L"wilson_Gather_Wood_1.mp3", SOUND_TREE, 0.2f);
				Engine::PlaySound_W(L"wilson_Gather_Wood_2.mp3", SOUND_TREE, 0.2f);
				Tree_Sound();
				dynamic_cast<CResObject*>(_Obj)->Set_Attack();
				dynamic_cast<CResObject*>(_Obj)->Set_Attack_State(true);
				//Test임시
				CGameObject* pGameObject = CTreeLeafFall::Create(m_pGraphicDev, L"PARTICLE_LEAF", 3, _Obj->GetTransForm()->Get_Pos(),_vec3(-0.7f,-2.f,-0.7f), _vec3(0.7f, 0.8f, 0.7f));
				CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::PARTICLE, pGameObject);
				m_vPlayerActing = true;
			}
			m_eCurState = AXE_CHOP_PRE;
		}
		else
		{
			m_eCurState = DIALOG;  //도끼가 없으면 안된다는 대사 추가
			m_vPlayerActing = true;
		}
		break;
	case PIG_HOUSE:
		if (m_ePreWeapon == HAMMER)
		{
			if ((m_fFrameEnd - 1) < m_fFrame && !m_vPlayerActing)
			{
				Tree_Sound();
				dynamic_cast<CResObject*>(_Obj)->Set_Attack();
				dynamic_cast<CResObject*>(_Obj)->Set_Attack_State(true);
				dynamic_cast<CPigHouse*>(_Obj)->Set_Pig_Angry();
				m_vPlayerActing = true;
			}
			m_eCurState = HAMMERING;
		}
		else
		{
			m_eCurState = DIALOG;		//망치가 없으면 안된다는 대사 추가
			m_vPlayerActing = true;
		}
		break;
	case GRASS:
		if ((m_fFrameEnd - 1) < m_fFrame && !m_vPlayerActing)
		{
			dynamic_cast<CResObject*>(_Obj)->Set_Attack();
			Grass_Sound();
			m_vPlayerActing = true;
		}
		m_eCurState = BUILD;
		break;
	case BERRY_BUSH:
		if ((m_fFrameEnd - 1) < m_fFrame && !m_vPlayerActing)
		{
			dynamic_cast<CResObject*>(_Obj)->Set_Attack();
			Grass_Sound();
			m_vPlayerActing = true;
		}
		m_eCurState = BUILD;
		break;
	}
}

_int CPlayer::Die_Check()
{
	if (m_Stat.fHP <= 0 && m_ePreState != DEAD)
	{
		m_Stat.fHP = 0;
		m_eCurState = DEAD;
		m_Stat.bDead = true;
		m_fFrame = 0.f;
	}
	else if (m_ePreState == DEAD)
	{
		if (m_fFrameEnd - 1 < m_fFrame)
		{
			m_fFrame = m_fFrameEnd - 1;
			m_bFrameLock = true;
			//여기에 고스트 소환하는 거
			_vec3 pPlayerPos;
			m_pTransForm->Get_Info(INFO_POS, &pPlayerPos);
			Engine::PlaySound_W(L"wilson_Ghost_Spawn.mp3", SOUND_PLAYER, 0.3f);
			m_Ghost = CGhost::Create(m_pGraphicDev, pPlayerPos);
			NULL_CHECK_RETURN(m_Ghost, E_FAIL);
			FAILED_CHECK_RETURN(scenemgr::Get_CurScene()->GetLayer(eLAYER_TYPE::GAME_LOGIC)->AddGameObject(eOBJECT_GROUPTYPE::EFFECT, m_Ghost), E_FAIL);
		}
	}


	return 0;

}

void CPlayer::Rebirth()
{
	if (KEY_HOLD(DIK_M))
	{
		_vec3 pPlayerPos;
		m_pTransForm->Get_Info(INFO_POS, &pPlayerPos);
		CGameObject* amulet = CRebirth::Create(m_pGraphicDev, pPlayerPos);
		NULL_CHECK_RETURN(amulet, );
		FAILED_CHECK_RETURN(scenemgr::Get_CurScene()->GetLayer(eLAYER_TYPE::GAME_LOGIC)->AddGameObject(eOBJECT_GROUPTYPE::EFFECT, amulet), );
		Engine::PlaySound_W(L"ghost[6].mp3", SOUND_PLAYER, 0.3f);
		
		m_eCurState = REBIRTH;
		m_bFrameLock = false;
		m_Stat.bDead = false;
		m_Stat.fHP = m_Stat.fMxHP;

		DeleteObject(m_Ghost);
		m_Ghost = nullptr;
	}
}

void CPlayer::First_Start()
{
	if (scenemgr::Get_CurScene()->Get_Scene_Name() == L"STAGE"
		&& !m_bFirstStart)
	{
		m_eCurState = SLEEP;
		m_bFirstStart = true;
	}
	else if (scenemgr::Get_CurScene()->Get_Scene_Name() != L"STAGE")
	{
		m_bWakeupTrig = true;
	}

	else if (m_ePreState == SLEEP)
	{
		if (m_fFrameEnd-1 < m_fFrame)
		{
			m_eCurState = WAKEUP;
		}
	}
	else if (m_ePreState == WAKEUP)
	{
		if (m_fFrameEnd-1 < m_fFrame)
		{
			m_eCurState = IDLE;
			m_bWakeupTrig = true;
		}
	}

}



HRESULT CPlayer::Ready_Light()
{
	//플레이어 조명
	D3DLIGHT9 tPointLightInfo;
	ZeroMemory(&tPointLightInfo, sizeof(D3DLIGHT9));

	tPointLightInfo.Type = D3DLIGHT_POINT;

	tPointLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tPointLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tPointLightInfo.Attenuation0 = 3.f;
	tPointLightInfo.Range = m_iLightRange;
	tPointLightInfo.Position = { 0.f, 0.f, 0.f };

	FAILED_CHECK_RETURN(light::Ready_Light(m_pGraphicDev, &tPointLightInfo, m_iLightNum), E_FAIL);
	light::Get_Light(m_iLightNum)->Close_Light();

	return S_OK;
}

void CPlayer::Fire_Light()
{
	if (m_ePreWeapon != TORCH || m_bTent)
	{
		light::Get_Light(m_iLightNum)->Close_Light();

		return;
	}


	D3DLIGHT9* tPointLightInfo = light::Get_Light(m_iLightNum)->Get_Light();

	tPointLightInfo->Type = D3DLIGHT_POINT;

	tPointLightInfo->Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tPointLightInfo->Attenuation0 = 3.f;
	tPointLightInfo->Range = m_iLightRange;

	_vec3 pPlayerPos;
	m_pTransForm->Get_Info(INFO_POS, &pPlayerPos); // player pos 값 설정
	tPointLightInfo->Position = pPlayerPos;

	light::Get_Light(m_iLightNum)->Update_Light();
}

void CPlayer::Update_State(const _float& fTimeDelta)
{
	if (m_bTent)
	{
		//텐트 안에 있을때 플레이어 상태값 변화

		if (m_Stat.fHP + fTimeDelta * 0.4f >= m_Stat.fMxHP)
			m_Stat.fHP = m_Stat.fMxHP;
		else
			m_Stat.fHP += fTimeDelta * 0.4f;

		if (m_Stat.fHungry - fTimeDelta <= 0.f)
			m_Stat.fHungry = 0.f;
		else
			m_Stat.fHungry -= fTimeDelta;

		if (m_Stat.fMental + fTimeDelta * 0.2f >= m_Stat.fMaxMental)
			m_Stat.fMental = m_Stat.fMaxMental;
		else
			m_Stat.fMental += fTimeDelta * 0.2f;
	}
	else
	{
		//일반 상태값 
		if (m_Stat.fHungry - (fTimeDelta * 0.3) < 0.f) m_Stat.fHungry = 0.f; else m_Stat.fHungry -= fTimeDelta * 0.3;
		if (m_Stat.fMental - (fTimeDelta * 0.3) < 0.f) m_Stat.fMental = 0.f; else m_Stat.fMental -= fTimeDelta * 0.1;
	}

}

//못찾았을경우 nullptr반환
CGameObject* CPlayer::Find_NeerObject(float _fRange, eOBJECT_GROUPTYPE _findTarget)
{
	auto& vecObj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, _findTarget);

	_vec3 vPlayerPos, vTargetPos, vLook;
	_float fNeerDistance = FLT_MAX;
	_float fFindDistance = 0.f;
	CGameObject* pFindObject = nullptr;

	ZeroMemory(vPlayerPos, sizeof(_vec3));
	ZeroMemory(vTargetPos, sizeof(_vec3));
	ZeroMemory(vLook, sizeof(_vec3));

	m_pTransForm->Get_Info(INFO_POS, &vPlayerPos);//플레이어좌표 구하기
	vPlayerPos.y = 0.f;

	//반복문돌면서 모든 오브젝트그룹 순회하기
	for (auto& obj : vecObj)
	{
		if (obj->IsDelete() || obj == nullptr)//해당오브젝트가 삭제될예정이면 무시
			continue;

		obj->GetTransForm()->Get_Info(INFO_POS, &vTargetPos);
		vTargetPos.y = 0.f;

		vLook = vTargetPos - vPlayerPos;

		fFindDistance = D3DXVec3Length(&vLook);
		if (_fRange < fFindDistance)//현재 찾은거리가 범위보다 클경우
			continue;

		if (fNeerDistance > fFindDistance)//현재 찾은 거리가 이전에찾았던 가장가까운거리보다 작을경우
		{
			pFindObject = obj;
			fNeerDistance = fFindDistance;
		}


	}

	return pFindObject;
}


void CPlayer::BillBoard()
{
	_matrix	matWorld, matView, matBill;

	m_pTransForm->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	matBill._23 = matView._23;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransForm->Set_WorldMatrix(&(matBill * matWorld));
}

void CPlayer::Hit_Sound()
{
	int randomvalue = rand() % 3;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"wilson_Voice_Hurt_1.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	case 1:
		Engine::PlaySound_W(L"wilson_Voice_Hurt_2.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	case 2:
		Engine::PlaySound_W(L"wilson_Voice_Hurt_3.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	}

}

void CPlayer::Eat_Sound()
{
	int randomvalue = rand() % 3;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"wilson_Eat_1.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	case 1:
		Engine::PlaySound_W(L"wilson_Eat_2.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	case 2:
		Engine::PlaySound_W(L"wilson_Eat_3.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	}
}

void CPlayer::Dialog_Sound()
{
	int randomvalue = rand() % 6;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"wilson_Voice_Generic_1.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	case 1:
		Engine::PlaySound_W(L"wilson_Vocie_Generic_2.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	case 2:
		Engine::PlaySound_W(L"wilson_Vocie_Generic_3.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	case 3:
		Engine::PlaySound_W(L"wilson_Vocie_Generic_4.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	case 4:
		Engine::PlaySound_W(L"wilson_Vocie_Generic_5.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	case 5:
		Engine::PlaySound_W(L"wilson_Vocie_Generic_6.mp3", SOUND_PLAYER_VOICE, 0.2f);
		break;
	}
}

void CPlayer::Rock_Sound()
{
	int randomvalue = rand() % 3;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"Obj_Rock_Hurt_1.mp3", SOUND_ROCK, 0.2f);
		break;
	case 1:
		Engine::PlaySound_W(L"Obj_Rock_Hurt_2.mp3", SOUND_ROCK, 0.2f);
		break;
	case 2:
		Engine::PlaySound_W(L"Obj_Rock_Hurt_3.mp3", SOUND_ROCK, 0.2f);
		break;
	}

}

void CPlayer::Tree_Sound()
{
	int randomvalue = rand() % 5;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"Obj_Tree_Impact_1.mp3", SOUND_TREE, 0.2f);
		break;
	case 1:
		Engine::PlaySound_W(L"Obj_Tree_Impact_2.mp3", SOUND_TREE, 0.2f);
		break;
	case 2:
		Engine::PlaySound_W(L"Obj_Tree_Impact_3.mp3", SOUND_TREE, 0.2f);
		break;
	case 3:
		Engine::PlaySound_W(L"Obj_Tree_Impact_4.mp3", SOUND_TREE, 0.2f);
		break;
	case 4:
		Engine::PlaySound_W(L"Obj_Tree_Impact_5.mp3", SOUND_TREE, 0.2f);
		break;
	}

}

void CPlayer::Grass_Sound()
{
	int randomvalue = rand() % 3;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"wilson_Gather_Reeds_1.mp3", SOUND_EFFECT, 0.2f);
		break;
	case 1:
		Engine::PlaySound_W(L"wilson_Gather_Reeds_2.mp3", SOUND_EFFECT, 0.2f);
		break;
	case 2:
		Engine::PlaySound_W(L"wilson_Gather_Reeds_3.mp3", SOUND_EFFECT, 0.2f);
		break;
	}
}



void CPlayer::Free()
{
	__super::Free();
}

//DeleteObject(삭제할게임오브젝트포인터 (GameObject*));
//CreateObject(레이어타입,오브젝트타입,생성한 오브젝트포인터);
