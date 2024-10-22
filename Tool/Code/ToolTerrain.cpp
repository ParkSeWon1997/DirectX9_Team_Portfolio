#include "ToolTerrain.h"
#include "framework.h"
#include "Export_Utility.h"
#include "ToolMgr.h"

CToolTerrain::CToolTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CToolTerrain::CToolTerrain(const CToolTerrain& rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CToolTerrain::~CToolTerrain()
{
}

HRESULT CToolTerrain::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CToolTerrain::Update_GameObject(const _float& fTimeDelta)
{
	Input_Mouse();
	if (CToolMgr::bTerrainWireFrame)
		renderer::Add_RenderGroup(RENDER_NONALPHA, this);
	else
		renderer::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CToolTerrain::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CToolTerrain::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	if (CToolMgr::bTerrainWireFrame) m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pTextureCom->Set_Texture(0);
	
	FAILED_CHECK_RETURN(SetUp_Material(), );

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (CToolMgr::bTerrainWireFrame) m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_FORCE_DWORD);
}


HRESULT CToolTerrain::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(proto::Clone_Proto(L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Tile1"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Tile1", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculator = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}

void CToolTerrain::Input_Mouse()
{
	if (Engine::Get_DIMouseState(DIM_LB) & 0x80 && CToolMgr::bTerrainWireFrame)
	{
		Picking_OnTerrain();
	}
}

HRESULT CToolTerrain::Picking_OnTerrain()
{
	_ulong i = m_pCalculator->Picking_OnTerrain_Tool(g_hWnd,
		m_pBufferCom,
		m_pTransForm);

	CToolMgr::iPickingIndex = i;

	//씬에 있는 타일 저장하는 map에 tile 오브젝트 추가해주기.
	auto iTemp = find_if(CToolMgr::vecPickingIdex.begin(), CToolMgr::vecPickingIdex.end(), [&](_int Dst) {
			return !(CToolMgr::iPickingIndex != Dst);
	});

	if (iTemp == CToolMgr::vecPickingIdex.end())
		CToolMgr::vecPickingIdex.push_back(i);

	return S_OK;
}

CToolTerrain* CToolTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolTerrain* pInstance = new CToolTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CToolTerrain::Free()
{
	__super::Free();
}
