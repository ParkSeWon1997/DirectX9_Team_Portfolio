#include "Cook.h"
#include "SlotMgr.h"
CCook::CCook(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev), m_bShow(false), pButton(nullptr)
{
}

CCook::~CCook()
{
}

HRESULT CCook::Ready_GameObject()
{
	CSlotMgr::GetInstance()->Add_InvenBoxList(m_pGraphicDev, COOK, HEIGHT, 4);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSizeX = 40.f;
	m_fSizeY = 110.f;

	m_fX = 450.f;
	m_fY = 300.f;

	__super::Ready_GameObject();

	pButton = CButtonUI::Create(m_pGraphicDev, _vec3{ m_fX, m_fY + 70.f, 0.f }, _vec2{ 20.f, 10.f });

	m_Stat.strObjName = L"요리 도구";

	return S_OK;
}

_int CCook::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bShow) return 0;

	__super::Update_GameObject(fTimeDelta);

	CSlotMgr::GetInstance()->Update_InvenBoxMgr(fTimeDelta, COOK);
	pButton->Update_GameObject(fTimeDelta);

	return 0;
}

void CCook::LateUpdate_GameObject()
{
	if (!m_bShow) return;

	CSlotMgr::GetInstance()->LateUpdate_InvenBoxMgr(COOK);
	pButton->LateUpdate_GameObject();

	__super::LateUpdate_GameObject();
}

void CCook::Render_GameObject()
{
	if (!m_bShow) return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	CSlotMgr::GetInstance()->Render_InvenBoxMgr(COOK);
	pButton->Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CCook::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Cook_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Cook_UI", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CCook* CCook::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCook* pInstance = new CCook(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CCook::Free()
{
	Safe_Release(pButton);
	__super::Free();
}
