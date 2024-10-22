#include "StateUI.h"

CStateUI::CStateUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CStateUI::~CStateUI()
{
}

HRESULT CStateUI::Ready_GameObject()
{
	__super::Ready_GameObject();

	return S_OK;
}

_int CStateUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CStateUI::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CStateUI::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_frame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CStateUI::Free()
{
	__super::Free();
}
