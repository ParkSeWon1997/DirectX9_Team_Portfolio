#include "stdafx.h"
#include "../Header/CItem.h"

#include "Export_System.h"
#include "Export_Utility.h"




CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev, L"NONE"), m_fX(0.f), m_fY(0.f), m_fSizeX(0.f), m_fSizeY(0.f), m_fPreX(0.f), m_fPreY(0.f)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_tItemInfo{}
{
	
}

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CGameObject(pGraphicDev, _strObjName)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_tItemInfo{}
{
	m_tItemInfo.ItemCount = 1;
}

CItem::CItem(const CItem& rhs)
	:CGameObject(rhs.m_pGraphicDev)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_tItemInfo(rhs.m_tItemInfo)
{
}

CItem::~CItem()
{
}

void CItem::MousePicking()
{
}

void CItem::Set_ObjState()
{
	if (m_strObjName == L"Log")
		m_Stat.strObjName = L"Åë³ª¹«";
	else if (m_strObjName == L"Berries")
		m_Stat.strObjName = L"³ª¹«¿­¸Å";
	else if (m_strObjName == L"Cooked_berries")
		m_Stat.strObjName = L"±¸¿î¿­¸Å";
	else if (m_strObjName == L"Cooked_Meat_Monster")
		m_Stat.strObjName = L"±«¹°°í±â±¸ÀÌ";
	else if (m_strObjName == L"CookedMeat")
		m_Stat.strObjName = L"°í±â±¸ÀÌ";
	else if (m_strObjName == L"CutGlass")
		m_Stat.strObjName = L"Ç® ÁÙ±â";
	else if (m_strObjName == L"CutStone")
		m_Stat.strObjName = L"Åë³ª¹«";
	else if (m_strObjName == L"Meat_Monster")
		m_Stat.strObjName = L"±«¹°°í±â";
	else if (m_strObjName == L"RawMeat")
		m_Stat.strObjName = L"³¯°í±â";
	else if (m_strObjName == L"Rocks_0")
		m_Stat.strObjName = L"µ¹¸ÍÀÌ";
	else if (m_strObjName == L"Rocks_1")
		m_Stat.strObjName = L"µ¹¸ÍÀÌ";
	else if (m_strObjName == L"Silk")
		m_Stat.strObjName = L"°Å¹ÌÁÙ";
	else if (m_strObjName == L"Twigs")
		m_Stat.strObjName = L"ÀÜ°¡Áö";
	else if (m_strObjName == L"Ax")
		m_Stat.strObjName = L"µµ³¢";
	else if (m_strObjName == L"Lance")
		m_Stat.strObjName = L"ÀüÅõÃ¢";
	else if (m_strObjName == L"Hammer")
		m_Stat.strObjName = L"¸ÁÄ¡";
	else if (m_strObjName == L"FireSton")
		m_Stat.strObjName = L"ºÎ½Ëµ¹";
	else if (m_strObjName == L"Pickaxe")
		m_Stat.strObjName = L"°î±ªÀÌ";
	else if (m_strObjName == L"Shovel")
		m_Stat.strObjName = L"»ð";
	else if (m_strObjName == L"Torch")
	{
		m_Stat.strObjName = L"È¶ºÒ";
		m_tItemInfo.Durability = 100.f;
	}
	else if (m_strObjName == L"BonFire")
		m_Stat.strObjName = L"¸ð´ÚºÒ";
	else if (m_strObjName == L"Wetgoop")
		m_Stat.strObjName = L"°ïÁ×";
	else if (m_strObjName == L"Meatballs")
		m_Stat.strObjName = L"¹ÌÆ®º¼";
}

_int CItem::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CItem::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

HRESULT CItem::Add_Component()
{
	CComponent* pComponent = nullptr;

	//VIBUFFER
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//TEXTURE
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(GetObjName().c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ GetObjName().c_str(), pComponent });

	//TransForm
	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	
	return S_OK;
}


void CItem::Free()
{
	__super::Free();
}
