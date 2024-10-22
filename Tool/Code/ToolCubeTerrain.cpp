#include "ToolCubeTerrain.h"
#include "Export_Utility.h"

CToolCubeTerrain::CToolCubeTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_pGraphicDev(pGraphicDev)
{
}

CToolCubeTerrain::~CToolCubeTerrain()
{
}

HRESULT CToolCubeTerrain::Ready_GameObject()
{
	m_pCubeCom.reserve(129 * 5 * 3);

	_ulong iIndex(0);
	_float fX(0), fY(0), fZ(0);

	for (int y = 0; y < 5; ++y)
	{
		for (int i = 0; i < 5; ++i)
		{

			for (int j = 0; j < VTXCNTX; ++j)
			{
				if (y > 0 && i != 0 && i != 4 && j != 0 && j != VTXCNTX - 1)
					continue;
				//iIndex = i * VTXCNTX + j + (y * 5 * VTXCNTX);
				CGameObject* pTile = Engine::CTile::Create(m_pGraphicDev);

				m_pCubeCom.push_back(pTile);
				//FAILED_CHECK_RETURN(pTile, E_FAIL);

				fX = j * 2;
				fZ = i * 2;
				fY = y * 2;

				pTile->GetTransForm()->Set_Pos(fX, fY, fZ);
			}
		}
	}

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CToolCubeTerrain::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	for (auto& iter : m_pCubeCom)
	{
		iter->Update_GameObject(fTimeDelta);
	}
	renderer::Add_RenderGroup(RENDER_NONALPHA, this);
	//m_pCubeCom[0]->Update_GameObject(fTimeDelta);
	return 0;
}

void CToolCubeTerrain::LateUpdate_GameObject()
{
	for (auto& iter : m_pCubeCom)
	{
		iter->LateUpdate_GameObject();
	}
	//m_pCubeCom[0]->LateUpdate_GameObject();
	__super::LateUpdate_GameObject();
}

void CToolCubeTerrain::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	for (auto& iter : m_pCubeCom)
	{
		iter->Render_GameObject();
	}
	//m_pCubeCom[0]->Render_GameObject();

}

HRESULT CToolCubeTerrain::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}

void CToolCubeTerrain::Input_Mouse()
{
}

HRESULT CToolCubeTerrain::Picking_OnTerrain()
{
	return S_OK;
}

CToolCubeTerrain* CToolCubeTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolCubeTerrain* pInstance = new CToolCubeTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		
		return nullptr;
	}

	return pInstance;
}

void CToolCubeTerrain::Free()
{
	for (int i = 0; i < m_pCubeCom.size(); ++i)
	{
		Safe_Release(m_pCubeCom[i]);
	}

	__super::Free();
}
