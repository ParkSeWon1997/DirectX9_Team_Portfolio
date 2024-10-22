#include "stdafx.h"
#include "LoadingScene.h"

#include "Export_System.h"
#include "Export_Utility.h"


#include "CBossScene.h"
#include "CRoadScene.h"
#include "CTestScene.h"

#include "Stage.h"
#include "Layer.h"
//
#include "CBGLoading.h"

//Camera
#include "DynamicCamera.h"



CLoadingScene::CLoadingScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev, L"BG_LOADING_SCENE")
	, m_pLoading(nullptr)
{

}

CLoadingScene::CLoadingScene(LPDIRECT3DDEVICE9 pGraphicDev, CLoading::LOADINGID m_eLoadingID)
	: Engine::CScene(pGraphicDev, L"BG_LOADING_SCENE")
	, m_pLoading(nullptr)
	,m_eLoadingID(m_eLoadingID)
{
}

CLoadingScene::~CLoadingScene()
{
}

HRESULT CLoadingScene::Ready_Scene()
{

	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i] = CLayer::Create();
	}

	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	m_pLoading = CLoading::Create(m_pGraphicDev, m_eLoadingID);

	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

Engine::_int CLoadingScene::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		if (KEY_TAP(DIK_SPACE))
		{
			CScene* pScene = nullptr;

			switch (m_eLoadingID)
			{
			case CLoading::LOADING_ROAD:
				pScene = CRoadScene::Create(m_pGraphicDev, L"ROAD");
				break;
			case CLoading::LOADING_BOSS:
				pScene = CBossScene::Create(m_pGraphicDev, L"BOSS");
				break;
			default:
				break;
			}
			NULL_CHECK_RETURN(pScene, -1);
			FAILED_CHECK_RETURN(scenemgr::Change_CurScene(pScene), E_FAIL);

			return 0;
		}
		

	}

	return iExit;
}

void CLoadingScene::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CLoadingScene::Render_Scene()
{
	if (m_pLoading->Get_Finish())
	{
		Engine::Render_Font(L"Loading_BOSS", m_pLoading->Get_String(), &_vec2(WINCX * 0.5f - 80.f, 500.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else
	{
		Engine::Render_Font(L"Loading_BOSS", L"LOADING...", &_vec2(0.f, 570.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
}

HRESULT CLoadingScene::Ready_Prototype()
{
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RvRcTex", CRvRcTex::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_3_3_1_Tex", CustomizeTex::Create(m_pGraphicDev, 3, 3, 1)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Anim", CAnimator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LoadingObject(), E_FAIL);

	return S_OK;
}

HRESULT CLoadingScene::Ready_LoadingObject()
{


	//GenerateMap LoadingUI // 로딩쓰레드 전에 미리 만들어야함
	Engine::CAnimation* pAnim = nullptr;
	pAnim = CAnimation::Create(m_pGraphicDev, L"BG_LOADING", L"../Bin/Resource/Texture/UI/BG_Loading/BG_Loading__%03d.png", 3, 4.f);
	proto::Ready_ProtoAnim(L"BG_LOADING", pAnim);

	CGameObject* pInstance = nullptr;
	pInstance = CBGLoading::Create(m_pGraphicDev, L"BG_LOADING", _vec3(WINCX * 0.5f, WINCY * 0.5f, 0.f), _vec3(WINCX * 0.5f, WINCY * 0.5f, 0.f));
	AddGameObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, pInstance);

	m_pCamera = nullptr;
	pInstance = m_pCamera = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 0.f, 0.f),
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

CLoadingScene* CLoadingScene::Create(LPDIRECT3DDEVICE9 pGraphicDev, CLoading::LOADINGID _eLoadingID)
{
	CLoadingScene* pInstance = new CLoadingScene(pGraphicDev, _eLoadingID);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("LOADING Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLoadingScene::Free()
{
	Safe_Release(m_pLoading);

	__super::Free();
}
