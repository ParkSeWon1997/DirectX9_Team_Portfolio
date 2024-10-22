#include "CInven.h"
#include "SlotMgr.h"
CInven::CInven(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CInven::~CInven()
{
}

HRESULT CInven::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = WINCX * 0.5;
	m_fY = WINCY - 20.f;

	m_fSizeX = 20.f;
	m_fSizeY = 340.f;

	//직교투영
	m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(90.f));
	__super::Ready_GameObject();

	return S_OK;
}

_int CInven::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	CSlotMgr::GetInstance()->Update_InvenBoxMgr(fTimeDelta, INVEN);
	return 0;
}

void CInven::LateUpdate_GameObject()
{
	CSlotMgr::GetInstance()->LateUpdate_InvenBoxMgr(INVEN);

	__super::LateUpdate_GameObject();
}

void CInven::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);
	FAILED_CHECK_RETURN(SetUp_Material(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)),)
	m_pBufferCom->Render_Buffer();

	CSlotMgr::GetInstance()->Render_InvenBoxMgr(INVEN);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CInven::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Left_Panel"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Left_Panel", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CInven* CInven::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInven* pInstance = new CInven(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInven::Free()
{
	__super::Free();
}
