#include "stdafx.h"
#include "..\Header\Logo.h"

#include "Export_System.h"
#include "Export_Utility.h"


#include "CBossScene.h"
#include "CRoadScene.h"
#include "CTestScene.h"
#include "LoadingScene.h"
#include "CMainScene.h"


#include "Stage.h"
#include "Layer.h"


//UI Object

#include "CGenerateMap.h"
#include "CGenerateHand.h"
#include "CLoadBackGround.h"



//Camera
#include "DynamicCamera.h"



CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev, L"LOGO")
	, m_pLoading(nullptr)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	Engine::StopAll();
	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i] = CLayer::Create();
	}
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	//Engine::PlaySound_NonStop(L"music_Logo.mp3", SOUND_MAIN_BGM, 0.8f);
	//Engine::PlayBGM(L"music_Logo.mp3", 0.7f);
	Engine::PlayBGM(L"GenerateMap.mp3", 0.3f);
	//m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_ROAD);

	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		if (KEY_TAP(DIK_Q))
		{
			Engine::CScene* pScene = nullptr;
			
			pScene = CStage::Create(m_pGraphicDev, L"STAGE");
			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(scenemgr::Change_CurScene(pScene), E_FAIL);

			return 0;
		}
		else if (KEY_TAP(DIK_W))
		{
			Engine::CScene* pScene = nullptr;

			pScene = CLoadingScene::Create(m_pGraphicDev, CLoading::LOADING_ROAD);
			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(scenemgr::Change_CurScene(pScene), E_FAIL);

			return 0;

		}
		else if (KEY_TAP(DIK_E))
		{
			Engine::CScene* pScene = nullptr;
			
			pScene = CLoadingScene::Create(m_pGraphicDev,CLoading::LOADING_BOSS);
			NULL_CHECK_RETURN(pScene, -1);
			FAILED_CHECK_RETURN(scenemgr::Change_CurScene(pScene), E_FAIL);

			return 0;

		}
		else if (KEY_TAP(DIK_T))
		{
			Engine::CScene* pScene = nullptr;
			
			pScene = CMainScene::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, -1);
			FAILED_CHECK_RETURN(scenemgr::Change_CurScene(pScene), E_FAIL);

			return 0;

		}
	}

	return iExit;
}

void CLogo::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CLogo::Render_Scene()
{
	Engine::Render_Font(L"Loading_Key1", m_pLoading->Get_String(), &_vec2(WINCX * 0.5f - 80.f, 300.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CLogo::Ready_Prototype()
{
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RvRcTex", CRvRcTex::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_LogoTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/IU.jpg")), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_3_3_1_Tex", CustomizeTex::Create(m_pGraphicDev, 3, 3, 1)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Anim", CAnimator::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Shader_Rect", CShader::Create(m_pGraphicDev, L"../Bin/ShaderFiles/Shader_Rect.hlsl")), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_LoadingObject(), E_FAIL);

	return S_OK;
}

HRESULT CLogo::Ready_LoadingObject()
{


	//GenerateMap LoadingUI // 로딩쓰레드 전에 미리 만들어야함
	Engine::CAnimation* pAnim = nullptr;
	pAnim = CAnimation::Create(m_pGraphicDev, L"GENERATE_WORLD", L"../Bin/Resource/Texture/UI/GenerateWorld/GenarateWorld__%03d.png", 193, 0.045f);
	proto::Ready_ProtoAnim(L"GENERATE_WORLD", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"GENERATE_HAND", L"../Bin/Resource/Texture/UI/GenerateHand/GenerateHand__%03d.png", 69, 0.05f);
	proto::Ready_ProtoAnim(L"GENERATE_HAND", pAnim);

	pAnim = CAnimation::Create(m_pGraphicDev, L"LOAD_BACK_GROUND", L"../Bin/Resource/Texture/UI/UI/GenerateBackGround.png", 1, 10.f);
	proto::Ready_ProtoAnim(L"LOAD_BACK_GROUND", pAnim);


	CGameObject* pInstance = nullptr;


	pInstance = CLoadBackGround::Create(m_pGraphicDev, L"LOAD_BACK_GROUND", _vec3(WINCX/2.f,WINCY/2.f,0.f),_vec3(WINCX*0.5f, WINCY*0.5f,0.f));
	AddGameObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, pInstance);

	pInstance = CGenerateMap::Create(m_pGraphicDev, L"GENERATE_MAP", _vec3(WINCX / 2 + 50.f, WINCY, 0.f), _vec3(325.f * 0.8f, 374.f * 0.8f, 0.f));
	AddGameObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, pInstance);


	pInstance = CGenerateHand::Create(m_pGraphicDev, L"GENERATE_HAND", _vec3(WINCX - 250.f, WINCY - 100.f, 0.f), _vec3(220.f * 0.7f, 228.f * 0.7f, 0.f), false);
	AddGameObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, pInstance);

	pInstance = CGenerateHand::Create(m_pGraphicDev, L"GENERATE_HAND", _vec3(250.f, WINCY - 100.f, 0.f), _vec3(220.f * 0.7f, 228.f * 0.7f, 0.f), true);
	AddGameObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, pInstance);

	m_pCamera = nullptr;
	pInstance = m_pCamera = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(64.f, 3.f, 64.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f),
		(_float)WINCX / WINCY,
		0.1f,
		1000.f);

	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::CAMERA, pInstance), E_FAIL);


	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo* pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLogo::Free()
{
	Safe_Release(m_pLoading);

	__super::Free();
}
