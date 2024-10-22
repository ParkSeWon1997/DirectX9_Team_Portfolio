#include "stdafx.h"
#include "Slot.h"

#include "Export_Utility.h"
#include "Export_System.h"

#include "UIMgr.h"
#include "CItem.h"
#include "Texture.h"
#include "Scene.h"
#include <ItemTool.h>
#include "SlotMgr.h"

CSlot::CSlot(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _uint iIndex)
	:CGameObject(pGraphicDev), m_fX(vPos.x), m_fY(vPos.y), m_pItem(nullptr), m_iInvenIndex(iIndex), m_eType(BOX_END)
{
}

CSlot::CSlot(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _uint iIndex, BOX_TYPE eType)
	:CGameObject(pGraphicDev), m_fX(vPos.x), m_fY(vPos.y), m_pItem(nullptr), m_eType(eType), m_iInvenIndex(iIndex)
{
}

CSlot::CSlot(const CSlot& rhs)
	:CGameObject(rhs.m_pGraphicDev),
	m_vFontPos(rhs.m_vFontPos),
	m_fX(rhs.m_fX),
	m_fY(rhs.m_fY),
	m_pItem(rhs.m_pItem)
{
}

CSlot::~CSlot()
{
}

HRESULT CSlot::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSizeX = 15.f;
	m_fSizeY = 15.f;

	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}

_int CSlot::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CSlot::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CSlot::Free()
{
	Safe_Release(m_pItem);
	__super::Free();
}
