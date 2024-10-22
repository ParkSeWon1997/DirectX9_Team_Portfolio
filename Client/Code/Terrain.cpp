#include "stdafx.h"
#include "..\Header\Terrain.h"

#include "Export_Utility.h"
#include "Export_System.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev, wstring pKey)
	: Engine::CGameObject(pGraphicDev),
	m_pKey(pKey)
{
}

CTerrain::CTerrain(const CTerrain& rhs)
	: Engine::CGameObject(rhs),
	m_pKey(rhs.m_pKey)
{

}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//Test z버퍼 땅뚫림 테스트


	return S_OK;
}

Engine::_int CTerrain::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	
	_vec3 vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);

	vPos.y = 60000;
	Compute_ViewZ(&vPos);
	//Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CTerrain::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CTerrain::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	//z버퍼 사용안함 ()
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	FAILED_CHECK_RETURN(SetUp_Material(), );

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CTerrain::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(proto::Clone_Proto(L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(m_pKey.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ m_pKey.c_str(), pComponent});

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring pKey)
{
	CTerrain* pInstance = new CTerrain(pGraphicDev, pKey);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();
}


