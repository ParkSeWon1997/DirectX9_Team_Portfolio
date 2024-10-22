#include "ToolBossScene.h"
#include "framework.h"
#include "Engine_Define.h"
#include "Export_Utility.h"

#include "ToolMgr.h"
#include "Layer.h"
#include <ToolCamera.h>
#include <ToolSkyBox.h>
#include <ToolObj.h>
#include <ToolBossTerrain.h>

CToolBossScene::CToolBossScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev, L"SceneTool")
{
}

CToolBossScene::~CToolBossScene()
{
}

HRESULT CToolBossScene::Ready_Scene()
{
	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i] = CLayer::Create();
	}

	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Terrain0.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Tile_Grass_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Grass1/grass_%d.png", 80, 1)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Tile_Grass_2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Grass2/grass2_%d.png", 80, 1)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Tile1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Terrain.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_BossTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/BossTerrain.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_SkyBoxTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../../Client/Bin/Resource/Texture/SkyBox/Ocean.dds")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Player_Unarmed_idle_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Player/UnArmed/idle_down/idle_down__000.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Berries", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Berries.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"CookedMeat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CookedMeat.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"CutGlass", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CutGlass.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"CutStone", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CutStone.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Rocks_0", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Rocks_0.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Rocks_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Rocks_1.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Twigs", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Twigs.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"FireSton", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/FireSton.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Tree", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Tree/idle/tree_idle__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Stone", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Rock/Nomal_Rock/Nomal_Rock_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Grass", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Glass/IDLE/IDLE__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Pig_House", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/pig_house/idle/idle__0.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Berry", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/berrybush/most/most_idle__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Pig_House_Dead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/pig_house/idle/rubble.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Beefalo", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/beefalo/beefalo_graze/befalo_graze__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Spider", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/spider/move/walk_down/walk_down__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Pig", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/pig/idle_happy/happy__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Tallbird", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/tallbird/walk_down/down__000.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"FireFlies", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/FireFlies/swarm_loop/swarm_loop__008.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Teleporter", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Teleporter_worm/Idle/WormIdle__000.png")), E_FAIL);

	//Obj_BossDoor
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"BossDoor", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/BossDoor/activate/activate__000.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"TrapSpike", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Trap/Spike/IDLE/IDLE__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Capapult", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Build/Catapult/ATK_DOWN/ATK_DOWN__000.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	m_pCalculator = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));

	return S_OK;
}

_int CToolBossScene::Update_Scene(const _float& fTimeDelta)
{
	Change_LightInfo(fTimeDelta);
	Input_Mouse();

	if (CToolMgr::bObjSaveData)
	{
		Save_ObjFile();
		CToolMgr::bObjSaveData = false;
	}
	if (CToolMgr::bObjLoadData)
	{
		FAILED_CHECK_RETURN(Load_ObjFile(), E_FAIL);
		CToolMgr::bObjLoadData = false;
	}

	if (CToolMgr::bPointSaveData)
	{
		Save_PointFile();
		CToolMgr::bPointSaveData = false;
	}
	if (CToolMgr::bPointLoadData)
	{
		FAILED_CHECK_RETURN(Load_PointFile(), E_FAIL);
		CToolMgr::bPointLoadData = false;
	}

	if (CToolMgr::bLightSaveData)
	{
		Save_LightFile();
		CToolMgr::bLightSaveData = false;
	}
	if (CToolMgr::bLightLoadData)
	{
		FAILED_CHECK_RETURN(Load_LightFile(), E_FAIL);
		CToolMgr::bLightLoadData = false;
	}


	return __super::Update_Scene(fTimeDelta);
}

void CToolBossScene::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CToolBossScene::Render_Scene()
{
}

HRESULT CToolBossScene::Input_Mouse()
{
	//test
	if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
	{
		if (CToolMgr::bMonsterAdd)
		{
			_vec3	vPickPos = Picking_Terrain();
			_vec3 vScale = _vec3(CToolMgr::fMonsterScale[0], CToolMgr::fMonsterScale[1], CToolMgr::fMonsterScale[2]);
			switch (CToolMgr::iItemCurrentMonsterIdx)
			{
			case 0:
				Create_Object(L"Spider", vPickPos, vScale);
				break;
			case 1:
				Create_Object(L"Pig", vPickPos, vScale);
				break;
			case 2:
				Create_Object(L"Beefalo", vPickPos, vScale);
				break;
			case 3:
				//Create_Object(L"Boss", vPickPos);
				break;
			case 4:
				Create_Object(L"Tallbird", vPickPos, vScale);
				break;
			default:
				break;
			}
			CToolMgr::bMonsterAdd = false;
		}
		else if (CToolMgr::bObjectAdd)
		{
			_vec3	vPickPos = Picking_Terrain();
			_vec3 vScale = _vec3(CToolMgr::fMonsterScale[0], CToolMgr::fMonsterScale[1], CToolMgr::fMonsterScale[2]);
			switch (CToolMgr::iItemCurrentEtcIdx)
			{
			case 0:
				Create_Object(L"Tree", vPickPos, vScale);
				break;
			case 1:
				Create_Object(L"Stone", vPickPos, vScale);
				break;
			case 2:
				Create_Object(L"Grass", vPickPos, vScale);
				break;
			case 3:
				Create_Object(L"Pig_House", vPickPos, vScale);
				break;
			case 4:
				Create_Object(L"Pig_House_Dead", vPickPos, vScale);
				break;
			case 5:
				Create_Object(L"Berry", vPickPos, vScale);
				break;
			case 6:
				Create_Object(L"FireFlies", vPickPos, vScale);
				break;
			case 7:
				Create_Object(L"Teleporter", vPickPos, vScale);
				break;
			case 8:
				Create_Object(L"BossDoor", vPickPos, vScale);
				break;
			case 9:
				Create_Object(L"TrapSpike", vPickPos, vScale);
				break;
			case 10:
				Create_Object(L"Capapult", vPickPos, vScale);
				break;
			default:
				break;
			}
			CToolMgr::bObjectAdd = false;
		}

		else if (CToolMgr::bItemAdd)
		{
			_vec3	vPickPos = Picking_Terrain();
			_vec3 vScale = _vec3(CToolMgr::fMonsterScale[0], CToolMgr::fMonsterScale[1], CToolMgr::fMonsterScale[2]);
			switch (CToolMgr::iItemCurrentItemIdx)
			{
			case 0:
				Create_Object(L"Twigs", vPickPos, vScale); break;
			case 1:
				Create_Object(L"Rocks_0", vPickPos, vScale); break;
			case 2:
				Create_Object(L"FireSton", vPickPos, vScale); break;
			case 3:
				Create_Object(L"CutGlass", vPickPos, vScale); break;
			default:
				break;
			}
			CToolMgr::bItemAdd = false;
		}
	}
	//"Stick",
	//	"Stone",
	//	"Firestone", //부싯돌
	//	"Grass_Cut",

	return S_OK;
}

_vec3 CToolBossScene::Picking_Terrain()
{
	auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();

	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(pTerrain->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform* pTerrainTransCom = dynamic_cast<CTransform*>(pTerrain->GetTransForm());
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());

	return m_pCalculator->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

HRESULT CToolBossScene::Ready_Layer_Environment()
{

	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT], E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	m_pCamera = nullptr;
	_vec3 vEye = { 0.f, 10.f, -5.f };
	_vec3 vAt = { 0.f, 0.f, 1.f };
	_vec3 vUp = { 0.f, 1.f, 0.f };

	pGameObject = m_pCamera = CToolCamera::Create(m_pGraphicDev,
		&vEye,
		&vAt,
		&vUp,
		D3DXToRadian(60.f),
		(_float)WINCX / WINCY,
		0.1f,
		1000.f);

	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::CAMERA, pGameObject), E_FAIL);

	pGameObject = CToolSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::BACK_GROUND, pGameObject), E_FAIL);


	return S_OK;
}

HRESULT CToolBossScene::Ready_Layer_GameLogic()
{
	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC], E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = m_pTerrain = CToolBossTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TILE, pGameObject), E_FAIL);

	return S_OK;
}

HRESULT CToolBossScene::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	//tLightInfo.Diffuse = D3DXCOLOR(CToolMgr::m_fDirectionDiffuseColor[0], CToolMgr::m_fDirectionDiffuseColor[1], CToolMgr::m_fDirectionDiffuseColor[2], 1.f);
	//tLightInfo.Specular = D3DXCOLOR(CToolMgr::m_fDirectionAmbientColor[0], CToolMgr::m_fDirectionAmbientColor[1], CToolMgr::m_fDirectionAmbientColor[2], 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(CToolMgr::m_fDirectionSpecularColor[0], CToolMgr::m_fDirectionSpecularColor[1], CToolMgr::m_fDirectionSpecularColor[2], 1.f);

	tLightInfo.Diffuse = D3DXCOLOR(CToolMgr::m_fDirectionDiffuseColor[CToolMgr::iTimeLight].x,
		CToolMgr::m_fDirectionDiffuseColor[CToolMgr::iTimeLight].y,
		CToolMgr::m_fDirectionDiffuseColor[CToolMgr::iTimeLight].z, 1.f);
	tLightInfo.Specular = D3DXCOLOR(CToolMgr::m_fDirectionAmbientColor[CToolMgr::iTimeLight].x,
		CToolMgr::m_fDirectionAmbientColor[CToolMgr::iTimeLight].y,
		CToolMgr::m_fDirectionAmbientColor[CToolMgr::iTimeLight].z, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(CToolMgr::m_fDirectionSpecularColor[CToolMgr::iTimeLight].x,
		CToolMgr::m_fDirectionSpecularColor[CToolMgr::iTimeLight].y,
		CToolMgr::m_fDirectionSpecularColor[CToolMgr::iTimeLight].z, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(light::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}


void CToolBossScene::Save_ObjFile()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/BossMap_Obj.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	_vec3 vPos{}, vScale{};
	_int iCount(0);
	DWORD	dwByte(0), dwStrByte(0);
	string pName;

	auto list = GetLayer(eLAYER_TYPE::GAME_LOGIC)->GetGroupObject(eOBJECT_GROUPTYPE::OBJECT);

	iCount = list.size();
	WriteFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (auto& objectIter : list)
	{
		dwStrByte = sizeof(TCHAR) * (objectIter->GetObjName().size() + 1);

		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, objectIter->GetObjName().c_str(), dwStrByte, &dwByte, nullptr);

		objectIter->GetTransForm()->Get_Info(INFO_POS, &vPos);
		WriteFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);

		vScale = objectIter->GetTransForm()->Get_Scale();
		WriteFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
	}

	MessageBox(g_hWnd, L"Object Save", L"성공", MB_OK);
	CloseHandle(hFile);
}

HRESULT CToolBossScene::Load_ObjFile()
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

	MessageBox(g_hWnd, L"Object Load", L"성공", MB_OK);

	return S_OK;
}

void CToolBossScene::Save_PointFile()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/BossMap_Point.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	_vec3 vPos{};
	_int iCount(0);
	DWORD	dwByte(0), dwStrByte(0);

	iCount = CToolMgr::vecPickingIdex.size();
	WriteFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (int i = 0; i < CToolMgr::vecPickingIdex.size(); ++i)
		WriteFile(hFile, &CToolMgr::vecPickingIdex[i], sizeof(_int), &dwByte, nullptr);

	MessageBox(g_hWnd, L"Point Save", L"성공", MB_OK);
	CloseHandle(hFile);
}

HRESULT CToolBossScene::Load_PointFile()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/BossMap_Point.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	_int iCount(0);
	DWORD	dwByte(0);

	ReadFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (int i = 0; i < iCount; ++i)
	{
		int iTemp = 0;
		ReadFile(hFile, &iTemp, sizeof(_int), &dwByte, nullptr);
		CToolMgr::vecPickingIdex.push_back(iTemp);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Point Load", L"성공", MB_OK);

	return S_OK;
}

void CToolBossScene::Save_LightFile()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/BossMap_Light.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	_vec3 vPos{};
	DWORD	dwByte(0);

	for (int i = 0; i < 3; ++i)
	{
		WriteFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].x, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].y, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].z, sizeof(_float), &dwByte, nullptr);

		WriteFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].x, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].y, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].z, sizeof(_float), &dwByte, nullptr);

		WriteFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].x, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].y, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].z, sizeof(_float), &dwByte, nullptr);
	}

	MessageBox(g_hWnd, L"Light Save", L"성공", MB_OK);
	CloseHandle(hFile);
}

HRESULT CToolBossScene::Load_LightFile()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/BossMap_Light.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte(0);

	for (int i = 0; i < 3; ++i)
	{
		ReadFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].z, sizeof(_float), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Light Load", L"성공", MB_OK);

	return S_OK;
}

HRESULT CToolBossScene::Create_Object(const _tchar* pName, _vec3 vPos, _vec3 vScale)
{
	Engine::CLayer* pLayer = scenemgr::Get_CurScene()->GetLayer(eLAYER_TYPE::GAME_LOGIC);

	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CToolObj::Create(m_pGraphicDev, pName);
	pGameObject->SetObjName(pName);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);

	vPos.y = CToolMgr::fMonsterY;
	pGameObject->GetTransForm()->Set_Pos(vPos);

	//_vec3 vScale = _vec3(CToolMgr::fMonsterScale[0], CToolMgr::fMonsterScale[1], CToolMgr::fMonsterScale[2]);
	pGameObject->GetTransForm()->Set_Scale(vScale);

	CToolMgr::m_vecObj.push_back(pGameObject);

	return S_OK;
}

HRESULT CToolBossScene::Change_LightInfo(const _float& fTimeDelta)
{
	D3DLIGHT9* tLightInfo = light::Get_Light(0)->Get_Light();
	_float fSpeed = 1;
	//ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo->Type = D3DLIGHT_DIRECTIONAL;

	int iIndex = 0;
	if (CToolMgr::iAUtoTime == 0)
	{
		iIndex = light::Change_Light(fTimeDelta, 0);
	}
	else
		iIndex = CToolMgr::iTimeLight;

	// 낮, 오후, 밤 시간 별 조명 값이 다름.
	// 시간이 변경될 때 마다 조명이 같아질 수 있도록 값을 시간에 따라 변경해줘야 함.

	if (CToolMgr::iAUtoTime == 0)
	{
		vLight[0].x += (CToolMgr::m_fDirectionDiffuseColor[iIndex].x - vLight[0].x) * fTimeDelta;
		vLight[0].y += (CToolMgr::m_fDirectionDiffuseColor[iIndex].y - vLight[0].y) * fTimeDelta;
		vLight[0].z += (CToolMgr::m_fDirectionDiffuseColor[iIndex].z - vLight[0].z) * fTimeDelta;

		vLight[1].x += (CToolMgr::m_fDirectionAmbientColor[iIndex].x - vLight[1].x) * fTimeDelta;
		vLight[1].y += (CToolMgr::m_fDirectionAmbientColor[iIndex].y - vLight[1].y) * fTimeDelta;
		vLight[1].z += (CToolMgr::m_fDirectionAmbientColor[iIndex].z - vLight[1].z) * fTimeDelta;

		vLight[2].x += (CToolMgr::m_fDirectionSpecularColor[iIndex].x - vLight[2].x) * fTimeDelta;
		vLight[2].y += (CToolMgr::m_fDirectionSpecularColor[iIndex].y - vLight[2].y) * fTimeDelta;
		vLight[2].z += (CToolMgr::m_fDirectionSpecularColor[iIndex].z - vLight[2].z) * fTimeDelta;
	}
	else
	{
		vLight[0].x = CToolMgr::m_fDirectionDiffuseColor[iIndex].x;
		vLight[0].y = CToolMgr::m_fDirectionDiffuseColor[iIndex].y;
		vLight[0].z = CToolMgr::m_fDirectionDiffuseColor[iIndex].z;

		vLight[1].x = CToolMgr::m_fDirectionAmbientColor[iIndex].x;
		vLight[1].y = CToolMgr::m_fDirectionAmbientColor[iIndex].y;
		vLight[1].z = CToolMgr::m_fDirectionAmbientColor[iIndex].z;

		vLight[2].x = CToolMgr::m_fDirectionSpecularColor[iIndex].x;
		vLight[2].y = CToolMgr::m_fDirectionSpecularColor[iIndex].y;
		vLight[2].z = CToolMgr::m_fDirectionSpecularColor[iIndex].z;
	}

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

CToolBossScene* CToolBossScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolBossScene* pInstance = new CToolBossScene(pGraphicDev);

	if (pInstance->Ready_Scene())
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CToolBossScene::Free()
{
	__super::Free();
}
