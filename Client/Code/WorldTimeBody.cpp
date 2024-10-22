#include "WorldTimeBody.h"

CWorldTimeBody::CWorldTimeBody(LPDIRECT3DDEVICE9 pGraphicDev)
	: CStateUI(pGraphicDev)
{
}

CWorldTimeBody::~CWorldTimeBody()
{
}

HRESULT CWorldTimeBody::Ready_GameObject()
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

_int CWorldTimeBody::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CWorldTimeBody::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CWorldTimeBody::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CWorldTimeBody::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_World_2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_World_2", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CWorldTimeBody::Check_State()
{
}

CWorldTimeBody* CWorldTimeBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWorldTimeBody* pInstance = new CWorldTimeBody(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CWorldTimeBody::Free()
{
	__super::Free();
}
