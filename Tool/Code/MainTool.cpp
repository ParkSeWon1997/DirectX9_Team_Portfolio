#include "framework.h"
#include "MainTool.h"
#include "Export_System.h"
#include "Export_Utility.h"

#include "Engine_Function.h"

#include "ToolTerrain.h"
#include "SubScene.h"

#include "TerrainScene.h"
#include <ToolBossScene.h>


CMainTool::CMainTool() : m_pToolMgr(nullptr)
{
}

CMainTool::~CMainTool()
{
}

HRESULT CMainTool::Ready_MainTool()
{
	FAILED_CHECK_RETURN(SetUp_Setting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	m_pToolMgr = CToolMgr::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pToolMgr, E_FAIL);

	return S_OK;
}

void CMainTool::Update_MainTool(const _float& fTimeDelta)
{
	Engine::Update_InputDev();

	m_pManagementClass->Update_Scene(fTimeDelta);
	m_pToolMgr->Update_ToolMgr();
}

void CMainTool::LateUpdate_MainTool()
{
	m_pManagementClass->LateUpdate_Scene();
}

void CMainTool::Render_MainTool()
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);
	m_pToolMgr->Render_ToolMgr();

	Engine::Render_End();

	eventmgr::EventUpdate();
}

HRESULT CMainTool::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CSceneMgr** ppManagement)
{
	Engine::CScene* pScene = nullptr;

	pScene = CSubScene::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(scenemgr::Create_SceneMgr(pGraphicDev, ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	FAILED_CHECK_RETURN((*ppManagement)->Change_Scene(pScene), E_FAIL);

	return S_OK;
}

HRESULT CMainTool::SetUp_Setting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, TOOL_WINCX, TOOL_WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);

	//(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		  // Z버퍼에 깊이 값을 기록은 하지만 자동 정렬을 수행할지 말지 결정
	//(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Z버퍼에 픽셀의 깊이 값을 저장할지 말지 결정

	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Default", L"바탕체", 30, 30, FW_HEAVY), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

CMainTool* CMainTool::Create()
{
	CMainTool* pInstance = new CMainTool;

	if (FAILED(pInstance->Ready_MainTool()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMainTool::Free()
{
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagementClass);
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pToolMgr);
	Engine::Release_Utility();
	Engine::Release_System();
}
