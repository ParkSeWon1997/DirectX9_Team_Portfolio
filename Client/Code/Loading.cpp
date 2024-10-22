#include "stdafx.h"
#include "..\Header\Loading.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "SlotMgr.h"
#include "UIMgr.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szLoading, sizeof(m_szLoading));
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL,	// 디폴트 보안 속성
		0,										// 디폴트 스택 사이즈(1바이트)
		Thread_Main,							// 동작 시킬 쓰레드 함수 이름(__stdcall 규약)
		this,									// 쓰레드 함수의 매개 변수로 전달할 값
		0,										// 쓰레드 생성 및 실행을 조절하기 위한 FLAG
		NULL);									// 쓰레드 ID 반환..

	m_eID = eID;

	//Engine::PlayBGM(L"Music_StartScreen.mp3", 0.5f);

	return S_OK;
}


//로딩씬 -> 메인화면씬 -> 맵생성로딩씬 -> 스테이지씬 ->로드맵로딩씬 -> 로드씬 -> 보스로딩씬 -> 보스씬

_uint CLoading::Loading_ForStage()
{

	FAILED_CHECK_RETURN(Loading_UI_Texture(), E_FAIL);
	FAILED_CHECK_RETURN(Loading_Item_Texture(), E_FAIL);
	FAILED_CHECK_RETURN(Loading_Player_Texture(), E_FAIL);
	FAILED_CHECK_RETURN(Loading_Beefalo_Texture(), E_FAIL);
	FAILED_CHECK_RETURN(Loading_TallBird_Texture(), E_FAIL);
	FAILED_CHECK_RETURN(Loading_Effect_UI(), E_FAIL);
	//FAILED_CHECK_RETURN(Loading_Anim_Texture(), E_FAIL);

	FAILED_CHECK_RETURN(SaveLoadingTexture(L"Proto_TerrainTexture", TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Terrain.png"), E_FAIL);
	FAILED_CHECK_RETURN(SaveLoadingTexture(L"Proto_BossTexture", TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/BossTerrain.png"), E_FAIL);
	FAILED_CHECK_RETURN(SaveLoadingTexture(L"Proto_HeightTerrainTexture", TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Terrain0.png"), E_FAIL);


	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_PlayerTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player%d.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_EffectTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", 90)), E_FAIL);

	Loading_Spider_Texture();
	Loading_Pig_Texture();
	/*Loading_Boss_Texture();*/
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_SkyBoxTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/Ocean.dds")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_BurgerCube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../../Client/Bin/Resource/Texture/Cube/Road.dds")), E_FAIL);
	//싱글톤 준비
	CUIMgr::GetInstance()->Ready_CreateInfo();
	CSlotMgr::GetInstance()->Add_InvenBoxList(m_pGraphicDev, INVEN, WIDTH, INVENCNT);

	
	m_bFinish = true;
	lstrcpy(m_szLoading, L"PRESS SPACE KEY");
	//test
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"PARTICLE_SNOW", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/Snow.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"PARTICLE_LEAF", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/leaf_0.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"PARTICLE_SMOKE", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/smoke.png", 1)), E_FAIL);

	return S_OK;
}


HRESULT CLoading::Loading_Player_Texture()
{
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_idle_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/idle_down/idle_down__%03d.png", 22)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_idle_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/idle_up/idle_up__%03d.png", 22)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_idle_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/idle_side/idle_side__%03d.png", 23)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_run_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/run_down/run_down__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_run_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/run_up/run_up__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_run_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/run_side/run_side__%03d.png", 6)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_build_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/build_down/build_down__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_build_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/build_up/build_up__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_build_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/build_side/build_side__%03d.png", 6)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_pickup_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/pickup_down/pickup_down__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_pickup_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/pickup_up/pickup_up__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_pickup_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/pickup_side/pickup_side__%03d.png", 6)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_hit_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/hit_std/hit_std__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_hit_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/hit_up/hit_up__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_hit_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/hit_side/hit_side__%03d.png", 7)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_punch_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/attack/punch_down/punch_down__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_punch_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/attack/punch_up/punch_up__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_punch_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/attack/punch_side/punch_side__%03d.png", 11)), E_FAIL);


	//횃불
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Torch_idle_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/torch/idle_down/torch_idle_down__%03d.png", 22)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Torch_idle_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/torch/idle_up/torch_idle_up__%03d.png", 22)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Torch_idle_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/torch/idle_side/torch_idle_side__%03d.png", 22)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Torch_run_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/torch/run_down/torch_run_down__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Torch_run_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/torch/run_up/torch_run_up__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Torch_run_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/torch/run_side/torch_run_side__%03d.png", 6)), E_FAIL);


	//도끼
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_preaxe_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/axe/chop_predown/chop_predown__%03d.png", 15)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_preaxe_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/axe/chop_preup/chop_preup__%03d.png", 15)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_preaxe_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/axe/chop_preside/chop_preside__%03d.png", 15)), E_FAIL);

	/*FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_axe_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/axe/chop_down/chop_down__%03d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_axe_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL,   L"../Bin/Resource/Texture/Player/Armed/axe/chop_up/chop_up__%03d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_axe_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/axe/chop_side/chop_side__%03d.png", 9)), E_FAIL);*/

	//해머
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_hammer_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/hammer/hammer_down/hammer_down__%03d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_hammer_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/hammer/hammer_up/hammer_up__%03d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_hammer_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/hammer/hammer_side/hammer_side__%03d.png", 9)), E_FAIL);

	//곡괭이
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_picking_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/pick/pick_down/pick_down__%03d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_picking_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/pick/pick_up/pick_up__%03d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_picking_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/pick/pick_side/pick_side__%03d.png", 9)), E_FAIL);

	//창
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_spear_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/spear/spear_down/spear_down__%03d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_spear_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/spear/spear_up/spear_up__%03d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_spear_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Armed/spear/spear_side/spear_side__%03d.png", 8)), E_FAIL);

	//별개 동작
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_falldown", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/fall_down/fall_down__%03d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_wakeup", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/wake_up/wake_up__%03d.png", 32)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_eat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/eat/eat__%03d.png", 35)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_die", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/death/death__%03d.png", 19)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_dialog", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/dial/dial__%03d.png", 17)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_sleep", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/sleep/sleep__%03d.png", 13)), E_FAIL);
	


	//유령상태
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Ghost_Appear", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Ghost/appear/appear__%03d.png", 12)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Ghost_IDLE", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Ghost/idle/idle__%03d.png", 26)), E_FAIL);


	//부활모션
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Rebirth", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/rebirth/amulet__%03d.png", 30)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_research", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/research/research__%03d.png", 17)), E_FAIL);


	return S_OK;
}

HRESULT CLoading::Loading_Beefalo_Texture()
{
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Erase", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Erase/Die__%03d.png", 5)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_graze", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_graze/befalo_graze__%03d.png", 10)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_idle/beefalo_idle__%03d.png", 16)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_walk_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_run/walk_down/down__%03d.png", 20)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_walk_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_run/walk_up/up__%03d.png", 20)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_walk_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_run/walk_side/side__%03d.png", 20)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_run_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_run/run_down/run_down__%03d.png", 4)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_run_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_run/run_up/run_up__%03d.png", 4)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_run_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_run/run_side/run_side__%03d.png", 4)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_atk_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_atk_down/atk_down__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_atk_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_atk_side/atk_side__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_atk_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_atk_up/atk_up__%03d.png", 7)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_hit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_hit/hit__%03d.png", 5)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Beefalo_dead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_dead/dead__%03d.png", 12)), E_FAIL);
	return 0;
}

HRESULT CLoading::Loading_Spider_Texture()
{
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Spider_walk_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/spider/move/walk_down/walk_down__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Spider_walk_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/spider/move/walk_up/walk_up__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Spider_walk_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/spider/move/walk_side/walk_side__%03d.png", 6)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Spider_atk_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/spider/atk/atk_down/atk_down__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Spider_atk_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/spider/atk/atk_up/atk_up__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Spider_atk_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/spider/atk/atk_side/atk_side__%03d.png", 11)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Spider_hit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/spider/hit/hit__%03d.png", 6)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Spider_dead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/spider/dead/dead__%03d.png", 9)), E_FAIL);


	return 0;
}

HRESULT CLoading::Loading_Pig_Texture()
{
	//Pig House
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_PigHouse_IDLE", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Resource/pig_house/idle/idle__%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_PigHouse_hit_run_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Resource/pig_house/hit/run_down/run_hit__%03d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_PigHouse_hit_std", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Resource/pig_house/hit/std/std_hit__%03d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_PigHouse_dead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Resource/pig_house/idle/rubble.png")), E_FAIL);


	//Pig
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_happy", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/idle_happy/happy__%03d.png", 17)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_angry", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/idle_angry/angry__%03d.png", 17)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_idle_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/idle_down/idle_down__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_idle_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/idle_up/idle_up__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_idle_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/idle_side/side__%03d.png", 7)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_walk_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/move/walk/down/walk_down__%03d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_walk_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/move/walk/up/walk_up__%03d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_walk_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/move/walk/side/walk_side__%03d.png", 8)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_run_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/move/run/down/run_down__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_run_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/move/run/up/run_up__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_run_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/move/run/side/run_side__%03d.png", 6)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_atk_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/attack/down/down__%03d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_atk_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/attack/up/up__%03d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_atk_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/attack/side/side__%03d.png", 10)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_dead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/dead/dead__%03d.png", 11)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_hit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/hit/hit__%03d.png", 6)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Pig_eat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/pig/eat/eat__%03d.png", 16)), E_FAIL);
	return 0;
}

HRESULT CLoading::Loading_TallBird_Texture()
{
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_walkdown", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/walk_down/down__%03d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_walkup", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/walk_up/up__%03d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_walkiside", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/walk_side/side__%03d.png", 8)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_atk_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/atk_predown/down__%03d.png", 12)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_atk_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/atk_preup/up__%03d.png", 12)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_atk_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/atk_preside/side__%03d.png", 12)), E_FAIL);

	//idle
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/idle/idle__%03d.png", 14)), E_FAIL);

	//hit
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_hit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/hit/hit__%03d.png", 6)), E_FAIL);

	//sleep loop
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_sleep", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/sleep_loop__000.png", 1)), E_FAIL);

	//wake_up
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_wakeup", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/wake_up/wake_up__%03d.png", 16)), E_FAIL);

	//taunt
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_taunt", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/taunt/taunt__%03d.png", 22)), E_FAIL);

	//dead
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_tallbird_dead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/tallbird/dead/dead__%03d.png", 10)), E_FAIL);
	return 0;
}

HRESULT CLoading::Loading_Boss_Texture()
{

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Idle_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/idle/down/down__%03d.png", 19)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Idle_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/idle/side/side__%03d.png", 19)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Idle_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/idle/up/up__%03d.png", 19)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_walk_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/walk/down/walk_down_%d.png", 16)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_walk_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/walk/side/side__%03d.png", 16)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_walk_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/walk/up/up__%03d.png", 16)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Atk_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/attack/down/non__%03d.png", 20)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Atk_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/attack/side/non__%03d.png", 20)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Atk_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/attack/up/non__%03d.png", 20)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_hit_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/hit/down/down__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_hit_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/hit/side/side__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_hit_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/hit/up/up__%03d.png", 7)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_sleep_pst", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/sleep/pst/pst__%03d.png", 18)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_sleep_loop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/sleep/loop/loop__%03d.png", 23)), E_FAIL);


	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_taunt", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/taunt/taunt__%03d.png", 17)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_dead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/dead/dead__%03d.png", 24)), E_FAIL);
	return S_OK;
}

HRESULT CLoading::Loading_Anim_Texture()
{
	Engine::CAnimation* pAnim = nullptr;


	//NONE



	pAnim = CAnimation::Create(m_pGraphicDev, L"NONE", L"../Bin/Resource/Texture/NONE.png", 1, 10.f);
	proto::Ready_ProtoAnim(L"NONE", pAnim);

	////////
	//Trap//
	////////

	//SPIKE
	pAnim = CAnimation::Create(m_pGraphicDev, L"TRAP_SPIKE_IDLE", L"../Bin/Resource/Texture/Build/Trap/Spike/IDLE/IDLE__%03d.png", 30, 0.03f);
	proto::Ready_ProtoAnim(L"TRAP_SPIKE_IDLE", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"TRAP_SPIKE_GROW", L"../Bin/Resource/Texture/Build/Trap/Spike/GROW/GROW__%03d.png", 31, 0.03f);
	proto::Ready_ProtoAnim(L"TRAP_SPIKE_GROW", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"TRAP_SPIKE_HIDE", L"../Bin/Resource/Texture/Build/Trap/Spike/PICKING/PICKING__%03d.png", 31, 0.03f);
	proto::Ready_ProtoAnim(L"TRAP_SPIKE_HIDE", pAnim);


	//TOOTH_TRAP
	pAnim = CAnimation::Create(m_pGraphicDev, L"TRAP_TOOTH_HIDE", L"../Bin/Resource/Texture/Build/Trap/ToothTrap/IDLE/IDLE__%03d.png", 9, 0.1f);
	proto::Ready_ProtoAnim(L"TRAP_TOOTH_HIDE", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"TRAP_TOOTH_GROW", L"../Bin/Resource/Texture/Build/Trap/ToothTrap/TRAP/TRAP__%03d.png", 11, 0.04f);
	proto::Ready_ProtoAnim(L"TRAP_TOOTH_GROW", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"TRAP_TOOTH_IDLE", L"../Bin/Resource/Texture/Build/Trap/ToothTrap/TRAP_IDLE/TRAP_IDLE__%03d.png", 5, 10.f);
	proto::Ready_ProtoAnim(L"TRAP_TOOTH_IDLE", pAnim);

	//TumbleWeed
	pAnim = CAnimation::Create(m_pGraphicDev, L"TRAP_TUMBLE_BREAK", L"../Bin/Resource/Texture/Build/Trap/TumbleWeed/BREAK/BREAK__%03d.png", 12, 0.1f);
	proto::Ready_ProtoAnim(L"TRAP_TUMBLE_BREAK", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"TRAP_TUMBLE_MOVE_DOWN", L"../Bin/Resource/Texture/Build/Trap/TumbleWeed/MOVE_DOWN/MOVE_DOWN__%03d.png", 17, 0.04f);
	proto::Ready_ProtoAnim(L"TRAP_TUMBLE_MOVE_DOWN", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"TRAP_TUMBLE_MOVE_UP", L"../Bin/Resource/Texture/Build/Trap/TumbleWeed/MOVE_UP/MOVE_UP__%03d.png", 5, 10.f);
	proto::Ready_ProtoAnim(L"TRAP_TUMBLE_MOVE_UP", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"TRAP_TUMBLE_MOVE_SIDE", L"../Bin/Resource/Texture/Build/Trap/TumbleWeed/MOVE_SIDE/MOVE_SIDE__%03d.png", 5, 10.f);
	proto::Ready_ProtoAnim(L"TRAP_TUMBLE_MOVE_SIDE", pAnim);


	//Catapult
	pAnim = CAnimation::Create(m_pGraphicDev, L"CATAPULT_ATK_DOWN", L"../Bin/Resource/Texture/Build/Catapult/ATK_DOWN/ATK_DOWN__%03d.png", 31, 0.05f);
	proto::Ready_ProtoAnim(L"CATAPULT_ATK_DOWN", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"CATAPULT_DEATH", L"../Bin/Resource/Texture/Build/Catapult/DEATH/DEATH__%03d.png", 31, 0.05f);
	proto::Ready_ProtoAnim(L"CATAPULT_DEATH", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"CATAPULT_IDLE_DOWN", L"../Bin/Resource/Texture/Build/Catapult/IDLE_DOWN/IDLE_DOWN__%03d.png", 51, 0.05f);
	proto::Ready_ProtoAnim(L"CATAPULT_IDLE_DOWN", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"CATAPULT_PLACE", L"../Bin/Resource/Texture/Build/Catapult/PLACE/PLACE__%03d.png", 31, 0.05f);
	proto::Ready_ProtoAnim(L"CATAPULT_PLACE", pAnim);


	//CatapultProjectile
	pAnim = CAnimation::Create(m_pGraphicDev, L"PROJ_CATAPULT_AIR", L"../Bin/Resource/Texture/Projectile/CataProj/AIR/AIR__%03d.png", 63, 0.01f);
	proto::Ready_ProtoAnim(L"PROJ_CATAPULT_AIR", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"PROJ_CATAPULT_HIT", L"../Bin/Resource/Texture/Projectile/CataProj/HIT/HIT__%03d.png", 48, 0.01f);
	proto::Ready_ProtoAnim(L"PROJ_CATAPULT_HIT", pAnim);

	//Geyser
	pAnim = CAnimation::Create(m_pGraphicDev, L"GEYSER_IDLE", L"../Bin/Resource/Texture/Build/Trap/Geyser/idle/idle__%03d.png", 77, 0.05f);
	proto::Ready_ProtoAnim(L"GEYSER_IDLE", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"GEYSER_LOOP", L"../Bin/Resource/Texture/Build/Trap/Geyser/loop/active_loop__%03d.png", 61, 0.05f);
	proto::Ready_ProtoAnim(L"GEYSER_LOOP", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"GEYSER_OPEN", L"../Bin/Resource/Texture/Build/Trap/Geyser/pre/active_pre__%03d.png", 43, 0.08f);
	proto::Ready_ProtoAnim(L"GEYSER_OPEN", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"GEYSER_CLOSE", L"../Bin/Resource/Texture/Build/Trap/Geyser/pst/active_pst__%03d.png", 118, 0.02f);
	proto::Ready_ProtoAnim(L"GEYSER_CLOSE", pAnim);



	return S_OK;
}

HRESULT CLoading::Loading_Effect_UI()
{
	Engine::CAnimation* pAnim = nullptr;
	pAnim = CAnimation::Create(m_pGraphicDev, L"PLAYER_HIT_EFFECT", L"../Bin/Resource/Texture/UI/Hit/NomalHit/NomalHit__%03d.png", 21, 0.05f);
	proto::Ready_ProtoAnim(L"PLAYER_HIT_EFFECT", pAnim);





	return S_OK;
}


//로드 씬 로딩
_uint CLoading::Loading_ForRoad()
{
	Loading_Anim_Texture();
	Loading_ForBoss();

	m_bFinish = true;
	lstrcpy(m_szLoading, L"PRESS SPACE KEY");
	return S_OK;
}

// 보스 씬 로딩
_uint CLoading::Loading_ForBoss()
{
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Idle_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/idle/down/down__%03d.png", 19)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Idle_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/idle/side/side__%03d.png", 19)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Idle_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/idle/up/up__%03d.png", 19)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_walk_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/walk/down/walk_down_%d.png", 16)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_walk_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/walk/side/side__%03d.png", 16)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_walk_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/walk/up/up__%03d.png", 16)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Atk_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/attack/down/non__%03d.png", 20)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Atk_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/attack/side/non__%03d.png", 20)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Atk_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/attack/up/non__%03d.png", 20)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_hit_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/hit/down/down__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_hit_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/hit/side/side__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_hit_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/hit/up/up__%03d.png", 7)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_sleep_pst", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/sleep/pst/pst__%03d.png", 18)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_sleep_loop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/sleep/loop/loop__%03d.png", 23)), E_FAIL);


	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_taunt", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/taunt/taunt__%03d.png", 17)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_long_taunt", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/long_taunt/taunt__%03d.png", 33)), E_FAIL);

	//Effect
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Atk_Effect", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/effect/standard/ice__%03d.png", 11)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Circle_Effect", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/circle/circle__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Rock_Effect", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/effect/effect__%03d.png", 13)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Snow_Effect", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/snow/snow__%03d.png", 13)), E_FAIL);


	//Ice Missile
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Ice_missile_left", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/Fall_Left.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Ice_missile_right", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/Fall_Right.png")), E_FAIL);

	//Ice Spike
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Ice_spike_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/ice_spike_1/none__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Ice_spike_2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/ice_spike_2/none__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Ice_spike_3", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/ice_spike_3/none__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Ice_spike_4", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/ice_spike_4/none__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Ice_spike_5", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/ice_spike_5/none__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Ice_spike_6", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/ice_spike_6/none__%03d.png", 11)), E_FAIL);

	//Fall Mark
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Ice_mark_pre", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/Fall_Mark/Pre/Pre__%03d.png", 30)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Ice_mark_pst", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Boss_Effect/Fall_Mark/Pst/Pst__%03d.png", 5)), E_FAIL);


	//Fall Down
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Fall_Down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/falldown/falldown__%03d.png", 37)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Wake_Up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/wakeup/wakeup__%03d.png", 11)), E_FAIL);

	//Pattern
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Pattern_Pre", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/PatternPre/pre__%03d.png", 12)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_Pattern_Loop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/PatternLoop/loop__%03d.png", 13)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Deer_dead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/new_deerclops/dead/dead__%03d.png", 24)), E_FAIL);


	//Particle







	m_bFinish = true;
	lstrcpy(m_szLoading, L"PRESS SPACE KEY");
	return S_OK;
}

HRESULT CLoading::Loading_UI_Texture()
{
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Left_Panel", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Left_Panel.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Item_Inven_Slot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/InvenSlot/Item_Inven_Slot_%03d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Weapon", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Weapon.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Light", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Light.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Alive", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Alive.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Processing", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Processing.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Science", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Science.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Equipment", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Equipment.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Cloth", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Cloth.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Armor_Slot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/InvenSlot/ArmorSlot/Armor_%03d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Cook_UI", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Cook_UI/idle__000.png")), E_FAIL);

	//Right_Top UI
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_HP", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/HP/HP__%03d.png", 20)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Hungry", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Hungry/Hunger__%03d.png", 20)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Mental", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Mental/Sanity__%03d.png", 20)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_World_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/World_1.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_World_2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/World_2.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_World_10", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/World_10.png")), E_FAIL);



	//Pop_Up UI
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Linked_Panel", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Linked_Panel.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Item_Panel", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Item_Panel_%03d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Explain_PopUp_Panel", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Explain_PopUp_Panel.png")), E_FAIL);

	//Pop_Up UI Botton
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Left_Panel_Button", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Left_Panel_Button__%03d.png", 3)), E_FAIL);
	//Make_Button
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Make_Button", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/UI/Make_Button_%03d.png", 2)), E_FAIL);





	return S_OK;
}

HRESULT CLoading::Loading_Item_Texture()
{
	//Objects Resource
	//Obj_Tree
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Tree_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Tree/idle/tree_idle__%03d.png", 27)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Tree_Hit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Tree/chop/chop__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Tree_Left_Fall", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Tree/fall_left/fall_left__%03d.png", 13)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Tree_Right_Fall", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Tree/fall_right/fall_right__%03d.png", 13)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Tree_Final", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Tree/stump__000.png")), E_FAIL);

	//Obj_Rock
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Nomal_Rock", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Rock/Nomal_Rock/Nomal_Rock_%d.png", 4)), E_FAIL);
	//Obj_Grass
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Grass_idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/grass_new/idle/grass_idle__%03d.png", 28)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Grass_dead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/grass_new/picking/picking__%03d.png", 7)), E_FAIL);
	//Obj_Bush
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_most_idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/berrybush/most/most_idle__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_most_pick", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/berrybush/most_picked/most_picked__%03d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_empty_idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/berrybush/empty_idle/empty_idle__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_empty_pick", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/berrybush/empty_picked/empty_picked__%03d.png", 5)), E_FAIL);
	//Obj_FireFlies
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_FireFlies_Pre", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/FireFlies/swarm_pre/swarm_pre__%03d.png", 17)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_FireFlies_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/FireFlies/swarm_loop/swarm_loop__%03d.png", 34)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_FireFlies_Pst", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/FireFlies/swarm_pst/swarm_pst__%03d.png", 18)), E_FAIL);

	//Obj_Teleporter
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Teleporter_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Teleporter_worm/Idle/WormIdle__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Teleporter_Open", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Teleporter_worm/Open/WormOpen__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Teleporter_Close", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Teleporter_worm/Close/WormClose__%03d.png", 6)), E_FAIL);

	//Obj_BossDoor
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_BossDoor_Open", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/BossDoor/activate/activate__%03d.png", 19)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_BossDoor_Close", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/BossDoor/deactive/deactive__%03d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_BossDoor_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/BossDoor/idle_loop_on/idle_loop_on__%03d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_BossDoor_Off", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/BossDoor/idle_off__000.png")), E_FAIL);

	//Obj_BonFIre
	//Drop_state
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_BonFIre_Drop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Campfire/place/place__%03d.png", 7)), E_FAIL);
	//Idle_state
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Bonfire_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Campfire/new_idle__000.png")), E_FAIL);


	//FIre
	//Level_0
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_FIre_lev_0", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Campfire/None_Fire.png")), E_FAIL);
	//Level_1
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_FIre_lev_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Campfire/lev1/lev1__%03d.png", 6)), E_FAIL);
	//Level_2
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_FIre_lev_2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Campfire/lev2/lev2__%03d.png", 6)), E_FAIL);
	//Level_3
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_FIre_lev_3", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Campfire/lev3/lev3__%03d.png", 6)), E_FAIL);
	//Level_4
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_FIre_lev_4", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Campfire/lev4/lev4__%03d.png", 6)), E_FAIL);

	//Obj_CookingPot
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_CookingPot_Burnt", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Cooking_pot/Burnt/Burnt__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_CookingPot_loop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Cooking_pot/Cooking_loop/Cooking_loop__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_CookingPot_Hit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Cooking_pot/Hit_Cooking/Hit_Cooking__%03d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_CookingPot_Hit_Empty", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Cooking_pot/Hit_Empty/Hit_Empty__%03d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_CookingPot_Idle_empty", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Cooking_pot/Idle_empty/Idle_empty__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_CookingPot_Idle_full", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Cooking_pot/Idle_full/Idle_full__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_CookingPot_Default", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Cooking_pot/Idle_Defalut__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_CookingPot_Place", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Cooking_pot/Place/Place__%03d.png", 9)), E_FAIL);


	//Tent
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Tent_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Tent/Idle/Tent_Idle__%03d.png", 19)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Tent_Burnt", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Tent/Burnt/Tent_Burnt__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Tent_Defalut_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Tent/Tent_Defalut_Idle__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Tent_Destroy", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Tent/Destroy/Tent_Destroy__%03d.png", 17)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Tent_Enter", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Tent/Enter/Tent_Enter__%03d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Tent_Hit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Tent/Hit/Tent_Hit__%03d.png", 4)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Tent_Place", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Tent/Place/Tent_Place__%03d.png", 8)), E_FAIL);




	//Item Object
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Log", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Log.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Berries", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Berries.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Cooked_berries", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Cooked_berries.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Cooked_Meat_Monster", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Cooked_Meat_Monster.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"CookedMeat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CookedMeat.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"CutGlass", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CutGlass.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"CutStone", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CutStone.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Meat_Monster", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Meat_Monster.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"RawMeat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/RawMeat.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Rocks_0", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Rocks_0.png")), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Rocks_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Rocks_1.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Silk", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Silk.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Twigs", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Twigs.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Ax", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Ax.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Lance", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Lance.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Hammer", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Hammer.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"FireSton", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/FireSton.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Pickaxe", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Pickaxe.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Shovel", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Shovel.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Torch", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Torch.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"BonFire", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/BonFire.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Woodplank", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Woodplank.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"PigTail", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/PigTail.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Rope", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Rope.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Charcoal", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Charcoal.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"LogSuit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/LogSuit.png")), E_FAIL);

	// Food
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Wetgoop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/wetgoop.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Meatballs", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/meatballs.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Armor", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Armor.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Cook", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Cook.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Tent", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Tant.png")), E_FAIL);
	return S_OK;
}


HRESULT CLoading::SaveLoadingTexture(const _tchar* _key, TEXTUREID _textureTag, const _tchar* _path, int num)
{
	FAILED_CHECK_RETURN(proto::Ready_Proto(_key, CTexture::Create(m_pGraphicDev, _textureTag, _path)), E_FAIL);

	//Save_TexturePath(L"Scene_STAGE", _key, _textureTag, _path, num);

	return S_OK;
}

unsigned int CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint iFlag(0);

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;
	case LOADING_ROAD:
		iFlag = pLoading->Loading_ForRoad();
		break;
	case LOADING_BOSS:
		iFlag = pLoading->Loading_ForBoss();
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	//_endthreadex(0);

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading* pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Loading Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free()
{
	Engine::StopAll();
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
