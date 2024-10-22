#include "stdafx.h"
#include "CMainScene.h"

#include "Export_System.h"
#include "Export_Utility.h"


#include "CBossScene.h"
#include "CRoadScene.h"
#include "CTestScene.h"
#include "LoadingScene.h"
#include "CBGLoading.h"

#include "Stage.h"
#include "Layer.h"
#include"Logo.h"

//UI Object
#include "CGenerateMap.h"
#include "CGenerateHand.h"
#include "CLoadBackGround.h"


#include"CMainUI.h"
#include"WorldUI.h"
//Camera
#include "DynamicCamera.h"



CMainScene::CMainScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev, L"LOGO")
{
}

CMainScene::~CMainScene()
{
}

HRESULT CMainScene::Ready_Scene()
{

	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
		m_arrLayer[i] = CLayer::Create();

	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	Engine::PlayBGM(L"music_Logo.mp3", 1.f);
	return S_OK;
}

Engine::_int CMainScene::Update_Scene(const _float& fTimeDelta)
{
	if (KEY_TAP(DIK_8))
	{
		ChangeScene(CLogo::Create(m_pGraphicDev));
	}

	_int	iExit = __super::Update_Scene(fTimeDelta);

	return iExit;
}

void CMainScene::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CMainScene::Render_Scene()
{
}

HRESULT CMainScene::Ready_Prototype()
{
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RvRcTex", CRvRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_LogoTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/IU.jpg")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_3_3_1_Tex", CustomizeTex::Create(m_pGraphicDev, 3, 3, 1)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Anim", CAnimator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Shader_Rect", CShader::Create(m_pGraphicDev, L"../Bin/ShaderFiles/Shader_Rect.hlsl")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Make_Button", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/UI/Make_Button_%03d.png", 2)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_UI_Main_Button", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Main/MainButton_%d.png", 2)), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_LoadingObject(), E_FAIL);

	return S_OK;
}

HRESULT CMainScene::Ready_LoadingObject()
{
	Engine::CAnimation* pAnim = nullptr;
	CGameObject* pInstance = nullptr;


	pAnim = CAnimation::Create(m_pGraphicDev, L"BG_MAIN", L"../Bin/Resource/Texture/UI/BG_Loading/Logo_Don't_Starve.png", 1, 11.f);
	proto::Ready_ProtoAnim(L"BG_MAIN", pAnim);


	
	pInstance = CBGLoading::Create(m_pGraphicDev, L"BG_MAIN", _vec3(WINCX / 2.f, WINCY / 2.f, 0.f), _vec3(WINCX * 0.5f, WINCY * 0.5f, 0.f));
	AddGameObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, pInstance);

	pInstance = CMainUI::Create(m_pGraphicDev,UI_STATIC, _vec3(540.f,600.f,0.f), _vec3(100.f, 30.f,0.f),L"Proto_UI_Main_Button");
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

CMainScene* CMainScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMainScene* pInstance = new CMainScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Main Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMainScene::Free()
{
	__super::Free();
}
