#include "ArmorSlot.h"
#include "Export_System.h"
#include "SlotMgr.h"

CArmorSlot::CArmorSlot(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _uint iIndex, ARMOR_SLOT_TYPE eType)
	: CSlot(pGraphicDev, vPos, iIndex), m_eArmorType(eType)
{
}

CArmorSlot::~CArmorSlot()
{
}

HRESULT CArmorSlot::Ready_GameObject()
{
	return __super::Ready_GameObject();
}

_int CArmorSlot::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CArmorSlot::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CArmorSlot::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_iInvenIndex - 15);

	m_pBufferCom->Render_Buffer();

	/*_tchar strItemCount[32];
	_itow_s(CInvenBoxMgr::GetInstance()->Get_InvenItem(m_iInvenIndex)->Get_ItemCount(), strItemCount, 10);
	Engine::Render_Font(L"Panel_Info", strItemCount, &_vec2(m_fX + 8.f, m_fY + 5.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));*/

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CArmorSlot::Add_Component()
{
	CComponent* pComponent = nullptr;

	//VIBUFFER
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Armor_Slot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Armor_Slot", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CSlot* CArmorSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _uint iIndex, ARMOR_SLOT_TYPE eType)
{
	CSlot* pInstance = new CArmorSlot(pGraphicDev, vPos, iIndex, eType);

	if (FAILED(dynamic_cast<CArmorSlot*>(pInstance)->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CArmorSlot::Free()
{
	__super::Free();
}
