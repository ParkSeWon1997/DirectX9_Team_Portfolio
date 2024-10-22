#include "Export_Utility.h"


IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
{

}

CSceneMgr::~CSceneMgr()
{
	Free();
}

HRESULT CSceneMgr::Change_Scene(CScene* _pScene)
{
	Safe_Release(m_pCurScene);

	renderer::Clear_RenderGroup();

	m_pCurScene = _pScene;
	return S_OK;
}


_int CSceneMgr::Update_Scene(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pCurScene, -1)

	return m_pCurScene->Update_Scene(fTimeDelta);
}

void CSceneMgr::LateUpdate_Scene()
{
	NULL_CHECK(m_pCurScene);
	m_pCurScene->LateUpdate_Scene();
}

void CSceneMgr::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pCurScene->Render_Scene();
	renderer::Render_GameObject(pGraphicDev);
	NULL_CHECK(m_pCurScene);

	m_pCurScene->Update_Event();

}

void CSceneMgr::Free()
{
	Safe_Release(m_pCurScene);
}
