#include "stdafx.h"
#include "UI.h"

#include "Export_Utility.h"
#include "Export_System.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_fX(0.f), m_fY(0.f), m_fSizeX(0.f), m_fSizeY(0.f)
{
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs.m_pGraphicDev),
	m_fX(rhs.m_fX), m_fY(rhs.m_fY), m_fSizeX(rhs.m_fSizeX), m_fSizeY(rhs.m_fSizeY)
{
}

CUI::~CUI()
{
}

HRESULT CUI::Ready_GameObject()
{
	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}

Engine::_int CUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	renderer::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CUI::LateUpdate_GameObject()
{

	__super::LateUpdate_GameObject();
}

void CUI::Render_GameObject()
{
	scenemgr::Get_CurScene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pBufferCom->Render_Buffer();


	scenemgr::Get_CurScene()->EndOrtho();
}


void CUI::Free()
{	
	__super::Free();
}


