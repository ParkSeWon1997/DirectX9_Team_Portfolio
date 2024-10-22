#include "ToolObj.h"
#include "Export_System.h"
#include "Export_Utility.h"


CToolObj::CToolObj(LPDIRECT3DDEVICE9 pGraphicDev, wstring pName)
	: CGameObject(pGraphicDev), m_pName(pName)
{
}

CToolObj::~CToolObj()
{
}

HRESULT CToolObj::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CToolObj::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	if (m_pName == L"Proto_Player_Unarmed_idle_down")
		BillBoard();
	else
		m_pTransForm->BillBoard();

	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CToolObj::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	//Height_Obj();
}

void CToolObj::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	m_pTextureCom->Set_Texture(0);

	FAILED_CHECK_RETURN(SetUp_Material(), );

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CToolObj::BillBoard()
{
	_matrix	matWorld, matView, matBill;

	m_pTransForm->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	matBill._23 = matView._23;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	_matrix allWorld = matBill * matWorld;
	m_pTransForm->Set_WorldMatrix(&(allWorld));
}

void CToolObj::Mouse_Input()
{
	if (Engine::GetMouseState(DIM_RB) == eKEY_STATE::TAP)
	{
		
	}
}

void CToolObj::Height_Obj()
{
	_vec3 vObjPos;
	m_pTransForm->Get_Info(INFO_POS, &vObjPos);
	const auto& pTerrainBuffer = scenemgr::Get_CurScene()->GetTerrainObject()->Find_Component(ID_STATIC, L"Proto_TerrainTex");
	const _vec3* pVtxPos = dynamic_cast<CTerrainTex*> (pTerrainBuffer)->Get_VtxPos();
	_float fY = m_pCalculator->Compute_HeightOnTerrain(&vObjPos, pVtxPos);

	vObjPos.y = fY + (m_pTransForm->Get_Scale().y );
	m_pTransForm->Set_Pos(vObjPos);
}

HRESULT CToolObj::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(m_pName.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ m_pName.c_str(), pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculator = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Calculator", pComponent });

	//m_pTransForm->Set_Scale(_vec3(0.7f, 0.5f, 0.7f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	//m_pTransForm->Set_Pos(vPos.x, 0.f, vPos.z);

	return S_OK;
}

CToolObj* CToolObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring pName)
{
	CToolObj* pInstance = new CToolObj(pGraphicDev, pName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CToolObj::Free()
{
	__super::Free();
}
