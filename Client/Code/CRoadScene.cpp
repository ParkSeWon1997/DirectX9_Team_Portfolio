#include "stdafx.h"
#include "CRoadScene.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "SlotMgr.h"

#include "CRoadSceneCamera.h"
#include "SkyBox.h"
#include "Effect.h"
#include "CInven.h"
#include "CPigHouse.h"
#include "Pig.h"
#include "BerryBush.h"
#include "MainApp.h"
// PlayerMgr
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


//Trap//Object
#include "CTrap.h"
#include "CSpike.h"
#include "CToothTrap.h"
#include "CTumbleWeed.h"
#include "CCatapult.h"
#include "CGeyserTrap.h"


//Particle

#include "CParticle.h"
#include "CSnow.h"





//Environment



#include "RunTerrain.h"
//

//Resource
#include "CItem.h"
#include "CItemFood.h"
#include <BerryBush.h>


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
#include <CBossScene.h>
#include <CObjectFireFlies.h>
#include <CTeleporterWorm.h>
#include <CBossDoor.h>
#include <Tallbird.h>
#include <CreateUI.h>
#include "Cook.h"
#include <Mouse.h>

CRoadScene::CRoadScene(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName)
	: Engine::CScene(pGraphicDev, _strSceneName)
{
	//test
}

CRoadScene::~CRoadScene()
{

}

HRESULT CRoadScene::Ready_Scene()
{
	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i] = CLayer::Create();
	}

	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);
	FAILED_CHECK_RETURN(Load_Data(), E_FAIL);

	Engine::StopAll();
	
	return S_OK;
}

Engine::_int CRoadScene::Update_Scene(const _float& fTimeDelta)
{
	__super::Update_Scene(fTimeDelta);

	Change_LightInfo(fTimeDelta);

	if (KEY_TAP(DIK_Z))
	{
		CScene* pScene = CBossScene::Create(m_pGraphicDev, L"BosScene");
		NULL_CHECK_RETURN(pScene, -1);
		scenemgr::Change_CurScene(pScene);
	}

	return 0;
}

void CRoadScene::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CRoadScene::Render_Scene()
{
}

HRESULT CRoadScene::Ready_Layer_Environment()
{
	Engine::CGameObject* pGameObject = nullptr;


	//////카메라
	m_pCamera = nullptr;
	pGameObject = m_pCamera = CRoadSceneCamera::Create(m_pGraphicDev,
		&_vec3(10.f, 6.f, 3.5f),
		&_vec3(10.f, 1.6f, 3.5f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(70.f),
		(_float)WINCX / WINCY,
		1.f,
		1000.f);

	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::CAMERA, pGameObject), E_FAIL);



	//////////스카이박스
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::BACK_GROUND, pGameObject), E_FAIL);




	return S_OK;
}

HRESULT CRoadScene::Ready_Layer_GameLogic()
{
	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC], E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = m_pTerrain = CRunTerrain::Create(m_pGraphicDev);
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
	m_pPlayer->GetTransForm()->Set_Pos(_vec3(10.f, 1.6f, 3.5f));
	m_pPlayer->GetTransForm()->Set_Scale(_vec3(1.f, 1.f, 1.f));
	dynamic_cast<CRoadSceneCamera*>(m_pCamera)->SetTarget(m_pPlayer);

	pGameObject = CDeerClops::Create(m_pGraphicDev, { 2.5f ,3.3f, 3.5f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::BOSS, pGameObject), E_FAIL);
	pGameObject->GetTransForm()->Set_Scale({ 3.5f,3.5f,3.5f });
	srand(Engine::Get_TimeDelta(L"Timer_FPS60"));


	//TRAP_OBJECT_SPIKE_BUSH

	//for (size_t i = 0; i < 40; i++)
	//{
	//	pGameObject = CSpike::Create(m_pGraphicDev, L"TRAP_SPIKE", _vec3(4.f + 4.f * i, 1.5f, (rand() % 3 + 2)));
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TRAP, pGameObject), E_FAIL);
	//}

	////TRAP_OBJECT_TOOTH
	//for (size_t i = 0; i < 40; i++)
	//{
	//	pGameObject = CToothTrap::Create(m_pGraphicDev, L"TRAP_TOOTH", _vec3(3.f + 4.f * i, 1.5f, (rand() % 3 + 2)));
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TRAP, pGameObject), E_FAIL);
	//}

	//for (size_t i = 0; i < 10; i++)
	//{
	//	pGameObject = CTumbleWeed::Create(m_pGraphicDev, L"TRAP_TUMBLE", _vec3(10.f + 5.f * i, 1.5f, (rand() % 3 + 2.5f) ));
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TRAP, pGameObject), E_FAIL);
	//}

	//pGameObject = CCatapult::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	//pGameObject->GetTransForm()->Set_Pos(_vec3(10.f, 1.5f, 3.5f));



	return S_OK;
}

HRESULT CRoadScene::Ready_Layer_UI()
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


HRESULT CRoadScene::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(light::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CRoadScene::Load_Data()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/RoadMap_Obj.dat",
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
		L"../../Data/RoadMap_Point.dat",
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
		L"../../Data/RoadMap_Light.dat",
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

HRESULT CRoadScene::Create_Object(const _tchar* pName, _vec3 vPos, _vec3 vScale)
{
	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC], E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	if (!_tcscmp(L"Tree", pName))
	{
		pGameObject = CObjectTree::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
		//pGameObject->GetTransForm()->Set_Scale(vScale);
		vPos.y = 1.f;
	}
	else if (!_tcscmp(L"Stone", pName))
	{
		pGameObject = CObjectRock::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Grass", pName))
	{
		pGameObject = CObjectGrass::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Pig_House", pName))
	{
		pGameObject = CPigHouse::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Berry", pName))
	{
		pGameObject = CBerryBush::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
		vPos.y = 0.5f;
	}
	else if (!_tcscmp(L"CutGlass", pName))
	{
		pGameObject = CItemBasic::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Rocks_0", pName))
	{
		pGameObject = CItemBasic::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Rocks_1", pName))
	{
		pGameObject = CItemBasic::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Twigs", pName))
	{
		pGameObject = CItemBasic::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
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
	else if (!_tcscmp(L"TrapSpike", pName))
	{
		pGameObject = CSpike::Create(m_pGraphicDev, L"TRAP_SPIKE", vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TRAP, pGameObject), E_FAIL);
		vPos.y = 1.8f;
	}
	else if (!_tcscmp(L"ToothTrap", pName))
	{
		pGameObject = pGameObject = CToothTrap::Create(m_pGraphicDev, L"TRAP_TOOTH", vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TRAP, pGameObject), E_FAIL);
		vPos.y = 2.f;
	}
	else if (!_tcscmp(L"Capapult", pName))
	{
		pGameObject = CCatapult::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TRAP, pGameObject), E_FAIL);
		vPos.y = 1.8f;
	}
	else if (!_tcscmp(L"TrapTooth", pName))
	{

		pGameObject = CGeyserTrap::Create(m_pGraphicDev, L"GEYSER", _vec3(12.f, 1.6f, 3.5f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TRAP, pGameObject), E_FAIL);
		vPos.y = 2.34f;
	}

	if (nullptr != pGameObject)
		pGameObject->GetTransForm()->Set_Pos(vPos);
}

HRESULT CRoadScene::Change_LightInfo(const _float& fTimeDelta)
{
	D3DLIGHT9* tLightInfo = light::Get_Light(0)->Get_Light();

	static _vec3 vLight[3] = {
		{m_vDirectionDiffuseColor[0].x, m_vDirectionDiffuseColor[0].y, m_vDirectionDiffuseColor[0].z },
		{m_vDirectionAmbientColor[0].x, m_vDirectionAmbientColor[0].y, m_vDirectionAmbientColor[0].z },
		{m_vDirectionSpecularColor[0].x, m_vDirectionSpecularColor[0].y, m_vDirectionSpecularColor[0].z } };

	tLightInfo->Type = D3DLIGHT_DIRECTIONAL;

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

CRoadScene* CRoadScene::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName)
{
	CRoadScene* pInstance = new CRoadScene(pGraphicDev, _strSceneName);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TestScene Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CRoadScene::Free()
{
	CSlotMgr::GetInstance()->Box_Release(CREATE);
	CSlotMgr::GetInstance()->Box_Release(COOK);
	__super::Free();
}
