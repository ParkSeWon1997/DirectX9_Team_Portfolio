#include"WorldUI.h"

#include "Export_Utility.h"
#include "Export_System.h"

#include "Scene.h"

CWorldUI::CWorldUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CStateUI(pGraphicDev)
{
}

CWorldUI::~CWorldUI()
{
}

HRESULT CWorldUI::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_fX = WINCX - 75.f;
	m_fY = 60.f;

	m_fSizeX = 40.f;
	m_fSizeY = 40.f;

	__super::Ready_GameObject();

	m_frameEnd = 0.f;

	return S_OK;
}

_int CWorldUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	//m_fWorldTime+=fTimeDelta;
	//m_fRotateAngle = 30.f * m_fWorldTime;

	//테스트용 시간 회전 1초에 1바퀴 회전// 들어온 값에 따라 회전하는 각도가 달라짐 


	return 0;
}

void CWorldUI::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CWorldUI::Render_GameObject()
{

	__super::Render_GameObject();
	scenemgr::Get_CurScene()->EndOrtho();
}

HRESULT CWorldUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_World_1"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_World_1", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CWorldUI::Check_State()
{
}


CWorldUI* CWorldUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWorldUI* pInstance = new CWorldUI(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CWorldUI::Free()
{
	__super::Free();
}
