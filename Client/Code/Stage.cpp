#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "SlotMgr.h"

#include "Mouse.h"
#include "player.h"
#include "Monster.h"
#include "Beefalo.h"
#include "Spider.h"
#include "DeerClops.h"
#include "Terrain.h"
#include "DynamicCamera.h"
#include "SkyBox.h"
#include "Effect.h"
#include "CInven.h"
#include "CPigHouse.h"
#include "Pig.h"
#include "Tallbird.h"
#include "BerryBush.h"


#include "Transform.h"

//PlayerMgr
#include "PlayerMgr.h"

//Scene
#include "Layer.h"
#include "CTestScene.h"
#include "CRoadScene.h"

// Monster/Resource/Object
#include "CObjectRock.h"
#include "CObjectGrass.h"
#include "CObjectTree.h"
#include"CObjectFireFlies.h"
#include"CTeleporterWorm.h"
#include"CBossDoor.h"
#include"CBonfire.h"
#include"CFIre.h"
#include"CCookingPot.h"
#include "CTent.h"

// Item/Object
#include "CItem.h"
#include "CItemFood.h"
#include "UIMgr.h"
#include "BerryBush.h"
#include "CCatapult.h"

//Trap//Object
#include "CTrap.h"
#include "CSpike.h"
#include "CToothTrap.h"
#include "CTumbleWeed.h"

//Particle
#include "CSnow.h"
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
#include <CreateUI.h>
#include "Cook.h"
#include "MainApp.h"



CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName)
	: Engine::CScene(pGraphicDev, _strSceneName)
{
	//test
}

CStage::~CStage()
{

}

HRESULT CStage::Ready_Scene()
{
	Engine::StopAll();
	srand(unsigned(time(NULL)));
	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i] = CLayer::Create();
	}
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);
	FAILED_CHECK_RETURN(Load_Data(), E_FAIL);

	
	//Engine::Update_System();
	//Engine::PlaySound_NonStop(L"Common_Forest.mp3", SOUND_BGM, 0.8f);		?????
	//Engine::PlayBGM(L"Common_Forest.mp3", 0.7f);

	//Engine::PlayBGM(L"Common_Wind.mp3", 0.7f);
	//Engine::PlayBGM(L"Common_Forest.mp3", 0.7f);
	return S_OK;
}

Engine::_int CStage::Update_Scene(const _float& fTimeDelta)
{

	if (KEY_TAP(DIK_2))
	{
		CGameObject* pGameObject = CSmoke::Create(m_pGraphicDev, L"PARTICLE_SMOKE", 2, m_pPlayer->GetTransForm()->Get_Pos(), 0.2f, 1.05f, 1.f);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::PARTICLE, pGameObject), E_FAIL);
	}
	Engine::Update_System();
	__super::Update_Scene(fTimeDelta);
	Change_LightInfo(fTimeDelta);

	return 0;
}

void CStage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CStage::Render_Scene()
{

}

HRESULT CStage::Ready_Layer_Environment()
{
	Engine::CGameObject* pGameObject = nullptr;

	//////카메라
	m_pCamera = nullptr;
	pGameObject = m_pCamera = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(64.f, 3.f, 64.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(50.f),
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

HRESULT CStage::Ready_Layer_GameLogic()
{
	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC], E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = m_pTerrain = CTerrain::Create(m_pGraphicDev, L"Proto_TerrainTexture");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TILE, pGameObject), E_FAIL);

	pGameObject = m_pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::PLAYER, pGameObject), E_FAIL);
	m_pPlayer->GetTransForm()->Set_Pos(_vec3(64.f, 1.15f, 64.f));
	m_pPlayer->GetTransForm()->Set_Scale(_vec3(2.f, 2.f, 2.f));
	dynamic_cast<CDynamicCamera*>(m_pCamera)->SetTarget(pGameObject);
	CPlayerMgr::GetInstance()->Set_Player_Pointer(m_pPlayer);

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI()
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


HRESULT CStage::Ready_LightInfo()
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

HRESULT CStage::Load_Data()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/MainMap_Obj.dat",
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

	HANDLE hPointFile = CreateFile(
		L"../../Data/MainMap_Point.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hPointFile)
		return E_FAIL;

	iCount = 0;
	dwByte = 0;
	vector<_int> vecInt;
	ReadFile(hPointFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (int i = 0; i < iCount; ++i)
	{
		int iTemp = 0;
		ReadFile(hPointFile, &iTemp, sizeof(_int), &dwByte, nullptr);
		vecInt.push_back(iTemp);
	}
	dynamic_cast<CTerrain*>(m_pTerrain)->Get_Buffer()->Set_VecPos(vecInt);

	CloseHandle(hPointFile);

	HANDLE hLightFile = CreateFile(
		L"../../Data/MainMap_Light.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hLightFile)
		return E_FAIL;

	dwByte = 0;

	for (int i = 0; i < 3; ++i)
	{
		ReadFile(hLightFile, &m_vDirectionDiffuseColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hLightFile, &m_vDirectionDiffuseColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hLightFile, &m_vDirectionDiffuseColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hLightFile, &m_vDirectionAmbientColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hLightFile, &m_vDirectionAmbientColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hLightFile, &m_vDirectionAmbientColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hLightFile, &m_vDirectionSpecularColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hLightFile, &m_vDirectionSpecularColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hLightFile, &m_vDirectionSpecularColor[i].z, sizeof(_float), &dwByte, nullptr);
	}

	CloseHandle(hLightFile);

	return S_OK;
}

HRESULT CStage::Create_Object(const _tchar* pName, _vec3 vPos, _vec3 vScale)
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
	else if (!_tcscmp(L"FireSton", pName))
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
	else if (!_tcscmp(L"Spider", pName))
	{
		pGameObject = CSpider::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::MONSTER, pGameObject), E_FAIL);
		vPos.y = 0.3f;
	}
	else if (!_tcscmp(L"Pig", pName))
	{
		pGameObject = CPig::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::MONSTER, pGameObject), E_FAIL);
		vPos.y = 1.05f;
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
		vPos.y = 0.f;
	}
	else if (!_tcscmp(L"BossDoor", pName))
	{
		pGameObject = CBossDoor::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 5.3f;
	}

	if (nullptr != pGameObject)
		pGameObject->GetTransForm()->Set_Pos(vPos);
}

HRESULT CStage::Change_LightInfo(const _float& fTimeDelta)
{
	D3DLIGHT9* tLightInfo = light::Get_Light(0)->Get_Light();

	static _vec3 vLight[3] = {
		{ m_vDirectionDiffuseColor[0].x, m_vDirectionDiffuseColor[0].y, m_vDirectionDiffuseColor[0].z },
		{ m_vDirectionAmbientColor[0].x, m_vDirectionAmbientColor[0].y, m_vDirectionAmbientColor[0].z },
		{ m_vDirectionSpecularColor[0].x, m_vDirectionSpecularColor[0].y, m_vDirectionSpecularColor[0].z },
	};

	tLightInfo->Type = D3DLIGHT_DIRECTIONAL;
	
	int iIndex(0);

	iIndex = light::Change_Light(fTimeDelta, 0);

	if (light::IsChangeTime())
	{
		if (iIndex == AFTERNOON)
			Engine::PlaySound_W(L"Afternoon.mp3", SOUND_BGM, 1.f);
		else if (iIndex == MORNING)
			Engine::PlaySound_W(L"music [11].mp3", SOUND_BGM, 1.f);
	}

	float fSpeed = 0.7;

	vLight[0].x += (m_vDirectionDiffuseColor[iIndex].x - vLight[0].x) * fTimeDelta * fSpeed;
	vLight[0].y += (m_vDirectionDiffuseColor[iIndex].y - vLight[0].y) * fTimeDelta * fSpeed;
	vLight[0].z += (m_vDirectionDiffuseColor[iIndex].z - vLight[0].z) * fTimeDelta * fSpeed;

	vLight[1].x += (m_vDirectionAmbientColor[iIndex].x - vLight[1].x) * fTimeDelta * fSpeed;
	vLight[1].y += (m_vDirectionAmbientColor[iIndex].y - vLight[1].y) * fTimeDelta * fSpeed;
	vLight[1].z += (m_vDirectionAmbientColor[iIndex].z - vLight[1].z) * fTimeDelta * fSpeed;

	vLight[2].x += (m_vDirectionSpecularColor[iIndex].x - vLight[2].x) * fTimeDelta * fSpeed;
	vLight[2].y += (m_vDirectionSpecularColor[iIndex].y - vLight[2].y) * fTimeDelta * fSpeed;
	vLight[2].z += (m_vDirectionSpecularColor[iIndex].z - vLight[2].z) * fTimeDelta * fSpeed;

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

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName)
{
	CStage* pInstance = new CStage(pGraphicDev, _strSceneName);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Stage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CStage::Free()
{
	CSlotMgr::GetInstance()->Box_Release(CREATE);
	CSlotMgr::GetInstance()->Box_Release(COOK);
	__super::Free();
}
