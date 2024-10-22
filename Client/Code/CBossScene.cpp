#include "stdafx.h"
#include "CBossScene.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "SlotMgr.h"

#include "DynamicCamera.h"
#include "SkyBox.h"
#include "Effect.h"
#include "CInven.h"
#include "CPigHouse.h"
#include "Pig.h"
#include "BerryBush.h"
#include "MainApp.h"
//PlayerMgr
#include "PlayerMgr.h"

//Scene
#include "Layer.h"

// Monster/Resource/Object
#include "CObjectRock.h"
#include "CObjectGrass.h"
#include "CObjectTree.h"

//Component
#include "Transform.h"

// Object
// Player/Monster
#include "player.h"

#include "Monster.h"
#include "Beefalo.h"
#include "Spider.h"
#include "DeerClops.h"

//Environment

#include "Terrain.h"
//

//Resource
#include "CItem.h"
#include "CItemFood.h"
#include <BerryBush.h>


//Particle
#include "CSnow.h"
#include "CDustParticle.h"
#include "CSmoke.h"
//UI
#include "UI.h"
#include"Slot.h"
#include"SlideUI.h"
#include"CHpUI.h"
#include"CMentalUI.h"
#include"WorldUI.h"
#include "WorldTimeBody.h"
#include "WorldHand.h"
#include"CHungryUI.h"
#include"ExplainPanel.h"
#include <ItemBasic.h>
#include "Stage.h"
#include <CObjectFireFlies.h>
#include <CTeleporterWorm.h>
#include <CBossDoor.h>
#include <Tallbird.h>
#include <Mouse.h>
#include <CreateUI.h>
#include "Cook.h"

CBossScene::CBossScene(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName)
	: Engine::CScene(pGraphicDev, _strSceneName)
{
	//test
}

CBossScene::~CBossScene()
{

}

HRESULT CBossScene::Ready_Scene()
{
	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i] = CLayer::Create();
	}
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	//FAILED_CHECK_RETURN(Loading_Boss_Texture(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);
	FAILED_CHECK_RETURN(Load_Data(), E_FAIL);
	Engine::StopAll();
	
	return S_OK;
}

Engine::_int CBossScene::Update_Scene(const _float& fTimeDelta)
{
	//Change_LightInfo(fTimeDelta);

	if (KEY_TAP(DIK_Z))
	{
		CScene* pScene = CStage::Create(m_pGraphicDev, L"Stage");
		NULL_CHECK_RETURN(pScene, -1);
		scenemgr::Change_CurScene(pScene);
	}

	if (KEY_TAP(DIK_2))
	{
		//매개변수( 디바이스,텍스처이미지(basic아이템과같음),파티클개수,위치,파티클크기,파티클보이는시간,파티클속도)
		/*CGameObject* pGameObject = CDustParticle::Create(m_pGraphicDev, L"PARTICLE_SNOW", 40, m_pPlayer->GetTransForm()->Get_Pos(),10.1f, 1.f, 10.f);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::PARTICLE, pGameObject), E_FAIL);*/


		CGameObject* pGameObject = CSmoke::Create(m_pGraphicDev, L"PARTICLE_SNOW", 2, m_pPlayer->GetTransForm()->Get_Pos(), 0.1f, 5.f, 1.f);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::PARTICLE, pGameObject), E_FAIL);

	}


	return __super::Update_Scene(fTimeDelta);
}

void CBossScene::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CBossScene::Render_Scene()
{
}

HRESULT CBossScene::Loading_Boss_Texture()
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
	
	return S_OK;
}

HRESULT CBossScene::Ready_Layer_Environment()
{
	Engine::CGameObject* pGameObject = nullptr;


	//////카메라
	m_pCamera = nullptr;
	pGameObject = m_pCamera = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(64.f, 3.f, 64.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f),
		(_float)WINCX / WINCY,
		0.1f,
		1000.f);

	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::CAMERA, pGameObject), E_FAIL);

	//////////스카이박스
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::BACK_GROUND, pGameObject), E_FAIL);

	return S_OK;
}

HRESULT CBossScene::Ready_Layer_GameLogic()
{
	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC], E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = m_pTerrain = CTerrain::Create(m_pGraphicDev, L"Proto_BossTexture");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TILE, pGameObject), E_FAIL);

	if (CPlayerMgr::GetInstance()->Get_Player_Pointer() == nullptr)
	{
		pGameObject = m_pPlayer = CPlayer::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::PLAYER, pGameObject), E_FAIL);
		CPlayerMgr::GetInstance()->Set_Player_Pointer(m_pPlayer);
	}
	else
	{
		m_pPlayer = CPlayerMgr::GetInstance()->Get_Player_Pointer();
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::PLAYER, m_pPlayer), E_FAIL);
	}

	m_pPlayer->GetTransForm()->Set_Pos(_vec3(64.f, 1.15f, 64.f));
	m_pPlayer->GetTransForm()->Set_Scale(_vec3(2.f, 2.f, 2.f));


	pGameObject = CDeerClops::Create(m_pGraphicDev, _vec3(64.f, 2.5f, 64.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::BOSS, pGameObject), E_FAIL);
	dynamic_cast<CDeerClops*>(pGameObject)->Set_Phase(true, true, false, false, false, false);
	pGameObject->GetTransForm()->Set_Scale({ 4.5f,4.5f,4.5f });


	dynamic_cast<CDynamicCamera*>(m_pCamera)->SetTarget(m_pPlayer);

	///TEST
	pGameObject = CSnow::Create(m_pGraphicDev, L"PARTICLE_SNOW", 660, _vec3(0.f, 0.f, 0.f), _vec3(64.f, 10.f, 64.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::PARTICLE, pGameObject), E_FAIL);
	pGameObject->GetTransForm()->Set_Pos(_vec3(32.f, 0.f, 32.f));

	return S_OK;
}

HRESULT CBossScene::Ready_Layer_UI()
{

	auto& uiLayer = m_arrLayer[(int)eLAYER_TYPE::FORE_GROUND];

	NULL_CHECK_RETURN(uiLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CCreateUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//Cook
	pGameObject = CCook::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	pGameObject = CInven::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//HpUI
	pGameObject = CHpUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//Mental
	pGameObject = CMentalUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);
	//Hunger
	pGameObject = CHungryUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);


	//WorldUI
	pGameObject = CWorldUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//WorldUI
	pGameObject = CWorldTimeBody::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//WorldUIClock
	pGameObject = CWorldHand::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//////////마우스
	pGameObject = m_pMouse = CMouse::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::MOUSE, pGameObject), E_FAIL);

	return S_OK;
}


HRESULT CBossScene::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(light::Ready_Light(m_pGraphicDev, &tLightInfo, ++CMainApp::g_iLightNum), E_FAIL);

	return S_OK;
}

HRESULT CBossScene::Load_Data()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/BossMap_Obj.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	_vec3 vPos{}, vScale{};
	_int iCount(0);
	DWORD	dwByte(0), dwStrByte(0);


	ReadFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (int i = 0; i < iCount; ++i)
	{
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		TCHAR* pName = new TCHAR[dwStrByte];

		ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);

		dwStrByte = 0;

		Create_Object(pName, vPos, vScale);
		delete[] pName;
	}
	CloseHandle(hFile);

	hFile = CreateFile(
		L"../../Data/BossMap_Point.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	iCount = 0;
	dwByte = 0;

	ReadFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (int i = 0; i < iCount; ++i)
	{
		int iTemp = 0;
		ReadFile(hFile, &iTemp, sizeof(_int), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	hFile = CreateFile(
		L"../../Data/BossMap_Light.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	dwByte = 0;

	for (int i = 0; i < 3; ++i)
	{
		ReadFile(hFile, &m_vDirectionDiffuseColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionDiffuseColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionDiffuseColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &m_vDirectionAmbientColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionAmbientColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionAmbientColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &m_vDirectionSpecularColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionSpecularColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionSpecularColor[i].z, sizeof(_float), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CBossScene::Create_Object(const _tchar* pName, _vec3 vPos, _vec3 vScale)
{
	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC], E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;
	vPos.y = 1.f;

	if (!_tcscmp(L"Tree", pName))
	{
		pGameObject = CObjectTree::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
		vPos.y = 2.f;
	}
	else if (!_tcscmp(L"Stone", pName))
	{
		pGameObject = CObjectRock::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
		vPos.y = 0.86;
	}
	else if (!_tcscmp(L"Grass", pName))
	{
		pGameObject = CObjectGrass::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
		vPos.y = 1.15f;
	}
	else if (!_tcscmp(L"Pig_House", pName))
	{
		pGameObject = CPigHouse::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
		vPos.y = 2.f;
	}
	else if (!_tcscmp(L"Berry", pName))
	{
		pGameObject = CBerryBush::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
		vPos.y = 0.87;
	}
	else if (!_tcscmp(L"CutGlass", pName))
	{
		pGameObject = CItemBasic::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
		vPos.y = 0.f;
	}
	else if (!_tcscmp(L"Rocks_0", pName))
	{
		pGameObject = CItemBasic::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
		vPos.y = 0.f;
	}
	else if (!_tcscmp(L"Rocks_1", pName))
	{
		pGameObject = CItemBasic::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
		vPos.y = 0.f;
	}
	else if (!_tcscmp(L"Twigs", pName))
	{
		pGameObject = CItemBasic::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
		vPos.y = 0.f;
	}
	else if (!_tcscmp(L"Beefalo", pName))
	{
		pGameObject = CBeefalo::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::MONSTER, pGameObject), E_FAIL);
		vPos.y = 1.5f;
	}
	else if (!_tcscmp(L"FireSton", pName))
	{
		pGameObject = CItemBasic::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Spider", pName))
	{
		pGameObject = CSpider::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::MONSTER, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Pig", pName))
	{
		pGameObject = CPig::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::MONSTER, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"FireFlies", pName))
	{
		pGameObject = CObjectFireFlies::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Teleporter", pName))
	{
		pGameObject = CTeleporterWorm::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"BossDoor", pName))
	{
		pGameObject = CBossDoor::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 3.f;
	}
	else if (!_tcscmp(L"Tallbird", pName))
	{
		pGameObject = CTallbird::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::MONSTER, pGameObject), E_FAIL);
		vPos.y = 1.4f;
	}
	if (nullptr != pGameObject)
		pGameObject->GetTransForm()->Set_Pos(vPos);
}

HRESULT CBossScene::Change_LightInfo(const _float& fTimeDelta)
{
	D3DLIGHT9* tLightInfo = light::Get_Light(0)->Get_Light();

	static _vec3 vLight[3] = {
		{m_vDirectionDiffuseColor[0].x, m_vDirectionDiffuseColor[0].y, m_vDirectionDiffuseColor[0].z },
		{m_vDirectionAmbientColor[0].x, m_vDirectionAmbientColor[0].y, m_vDirectionAmbientColor[0].z },
		{m_vDirectionSpecularColor[0].x, m_vDirectionSpecularColor[0].y, m_vDirectionSpecularColor[0].z } };

	tLightInfo->Type = D3DLIGHT_DIRECTIONAL;

	int iIndex(0);

	iIndex = light::Change_Light(fTimeDelta, 0);

	vLight[0].x += (m_vDirectionDiffuseColor[iIndex].x - vLight[0].x) * fTimeDelta;
	vLight[0].y += (m_vDirectionDiffuseColor[iIndex].y - vLight[0].y) * fTimeDelta;
	vLight[0].z += (m_vDirectionDiffuseColor[iIndex].z - vLight[0].z) * fTimeDelta;

	vLight[1].x += (m_vDirectionAmbientColor[iIndex].x - vLight[1].x) * fTimeDelta;
	vLight[1].y += (m_vDirectionAmbientColor[iIndex].y - vLight[1].y) * fTimeDelta;
	vLight[1].z += (m_vDirectionAmbientColor[iIndex].z - vLight[1].z) * fTimeDelta;

	vLight[2].x += (m_vDirectionSpecularColor[iIndex].x - vLight[2].x) * fTimeDelta;
	vLight[2].y += (m_vDirectionSpecularColor[iIndex].y - vLight[2].y) * fTimeDelta;
	vLight[2].z += (m_vDirectionSpecularColor[iIndex].z - vLight[2].z) * fTimeDelta;

	tLightInfo->Diffuse = D3DXCOLOR(vLight[0].x,
		vLight[0].y,
		vLight[0].z,
		1.f);
	tLightInfo->Specular = D3DXCOLOR(vLight[1].x,
		vLight[1].y,
		vLight[1].z,
		1.f);
	tLightInfo->Ambient = D3DXCOLOR(vLight[2].x,
		vLight[2].y,
		vLight[2].z,
		1.f);

	tLightInfo->Direction = _vec3(1.f, -1.f, 1.f);

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);
	light::Get_Light(0)->Update_Light();

	return S_OK;
}

CBossScene* CBossScene::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName)
{
	CBossScene* pInstance = new CBossScene(pGraphicDev, _strSceneName);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TestScene Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBossScene::Free()
{
	CSlotMgr::GetInstance()->Box_Release(CREATE);
	CSlotMgr::GetInstance()->Box_Release(COOK);
	__super::Free();
}
