#include "ToolSkyBox.h"

CToolSkyBox::CToolSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CToolSkyBox::CToolSkyBox(const CToolSkyBox& rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CToolSkyBox::~CToolSkyBox()
{
}

HRESULT CToolSkyBox::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransForm->m_vScale = { 40.f, 40.f, 40.f };

	return S_OK;
}

_int CToolSkyBox::Update_GameObject(const _float& fTimeDelta)
{
	renderer::Add_RenderGroup(RENDER_PRIORITY, this);

	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CToolSkyBox::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_matrix		matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	m_pTransForm->Set_Pos(matCamWorld._41, matCamWorld._42 + 3.f, matCamWorld._43);

}

void CToolSkyBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CToolSkyBox::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(proto::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_SkyBoxTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_SkyBoxTexture", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CToolSkyBox* CToolSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolSkyBox* pInstance = new CToolSkyBox(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CToolSkyBox::Free()
{
	__super::Free();
}
