#include "InvenSlot.h"
#include "Export_System.h"
#include "SlotMgr.h"
#include <ItemTool.h>

CInvenSlot::CInvenSlot(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _uint iIndex, BOX_TYPE eType)
	: CSlot(pGraphicDev, vPos, iIndex, eType)
{
}

CInvenSlot::~CInvenSlot()
{
}

HRESULT CInvenSlot::Ready_GameObject()
{
	__super::Ready_GameObject();

	return S_OK;
}

_int CInvenSlot::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	if (m_pItem) m_pItem->Update_GameObject(fTimeDelta);
	return 0;
}

void CInvenSlot::LateUpdate_GameObject()
{
	if (m_pItem) m_pItem->LateUpdate_GameObject();
	__super::LateUpdate_GameObject();
}

void CInvenSlot::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_bool bFood(false);
	if (!m_pItem)
		bFood = CSlotMgr::GetInstance()->Get_InvenItem(m_iInvenIndex) ? dynamic_cast<CItemTool*>(CSlotMgr::GetInstance()->Get_InvenItem(m_iInvenIndex))->Get_FoodItem() : 0;

	if (m_eType == COOK)
		bFood = false;

	m_pTextureCom->Set_Texture(bFood);

	m_pBufferCom->Render_Buffer();


	//if (m_pItem != nullptr)
	//{
	//	m_pItem->Render_GameObject();

	//	if (m_eType == INVEN)
	//	{
	//		_itow_s(m_pItem->Get_ItemCount(), strItemCount, 10);
	//		Engine::Render_Font(L"Panel_Info", strItemCount, &_vec2(m_fX + 8.f, m_fY + 5.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	//	}
	//}

	//if (m_eType == INVEN && CSlotMgr::GetInstance()->Get_InvenItem(m_iInvenIndex) != nullptr)
	//{

	//}
	if (m_pItem)
		m_pItem->Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CInvenSlot::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Item_Inven_Slot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Item_Inven_Slot", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CSlot* CInvenSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _uint iIndex, BOX_TYPE eType)
{
	CSlot* pInstance = new CInvenSlot(pGraphicDev, vPos, iIndex, eType);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInvenSlot::Free()
{
	__super::Free();
}
