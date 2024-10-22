#include "SlotMgr.h"
#include "Engine_Enum.h"
#include "ArmorSlot.h"
#include <ItemTool.h>
#include "InvenSlot.h"

IMPLEMENT_SINGLETON(CSlotMgr)

CSlotMgr::CSlotMgr()
{
	for (int i = 0; i < INVENCNT; ++i)
		m_pItemArr[i] = nullptr;

	for (int i = 0; i < 3; ++i)
		m_pArmorArr[i] = nullptr;

	for (int i = 0; i < 4; ++i)
		m_pCookArr[i] = nullptr;
}

CSlotMgr::~CSlotMgr()
{
	Free();
}

//아이템 습득실패시 false반환
_bool CSlotMgr::AddItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring strItemKey, _vec3* vSlotPos)
{
	vector<CSlot*> vecBox = Get_BoxList(INVEN);

	_vec3 vPos;

	for (int i = 0; i < INVENCNT; ++i) // 인벤토리 빈칸 있는지 없는지 체크
	{
		if (m_pItemArr[i] != nullptr && m_pItemArr[i]->GetObjName() == strItemKey) // 같은 아이템이 있다면 아이템 카운트 증가
		{
			vPos.x = vecBox[i]->Get_fX();
			vPos.y = vecBox[i]->Get_fY();

			m_pItemArr[i]->AddItemCount(1);
			(*vSlotPos) = vPos;
			return true;
		}
	}

	for (int i = 0; i < INVENCNT; ++i) 
	{
		if (m_pItemArr[i] == nullptr) // 아이템이 없다면 인벤에 아이템 생성
		{
			vPos.x = vecBox[i]->Get_fX();
			vPos.y = vecBox[i]->Get_fY();

			_bool bFood = strItemKey == L"Cooked_berries" || strItemKey == L"Cooked_Meat_Monster" || strItemKey == L"CookedMeat" || strItemKey == L"Meat_Monster" || strItemKey == L"RawMeat" || strItemKey == L"Berries" || strItemKey == L"Meatballs" || strItemKey == L"Wetgoop";

			ARMOR_SLOT_TYPE eArmor(ARMOR_SLOT_END);
			if (strItemKey == L"Ax" || strItemKey == L"Lance" || strItemKey == L"Hammer" || strItemKey == L"Pickaxe" || strItemKey == L"Torch")
				eArmor = HAND;
			if (strItemKey == L"Armor" || strItemKey == L"LogSuit")
				eArmor = ARMOR;

			CItem* pItem = CItemTool::Create(pGraphicDev, strItemKey, vPos, UI_ITEM_INVEN, bFood);
			if (eArmor != ARMOR_SLOT_END) dynamic_cast<CItemTool*>(pItem)->Set_ArmorSlotType(eArmor);
			dynamic_cast<CItemTool*>(pItem)->Set_BoxIndex(i);
			m_pItemArr[i] = pItem;
			(*vSlotPos) = vPos;
			return true;
		}
	}

	return false;
}

_bool CSlotMgr::Check_AddItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring strItemKey, _vec3* vSlotPos)
{
	vector<CSlot*> vecBox = Get_BoxList(INVEN);

	_vec3 vPos;

	for (int i = 0; i < INVENCNT; ++i) // 인벤토리 빈칸 있는지 없는지 체크
	{
		if (m_pItemArr[i] != nullptr && m_pItemArr[i]->GetObjName() == strItemKey) // 같은 아이템이 있다면 아이템 카운트 증가
		{
			vPos.x = vecBox[i]->Get_fX();
			vPos.y = vecBox[i]->Get_fY();
			(*vSlotPos) = vPos;
			return true;
		}
	}

	for (int i = 0; i < INVENCNT; ++i)
	{
		if (m_pItemArr[i] == nullptr) // 아이템이 없다면 인벤에 아이템 생성
		{
			vPos.x = vecBox[i]->Get_fX();
			vPos.y = vecBox[i]->Get_fY();
			(*vSlotPos) = vPos;
			return true;
		}
	}

	return false;
}

void CSlotMgr::Set_Create_Menu(int _iNum, CItem* pItem)
{
	if (3 < _iNum || pItem == nullptr)
		return;

	vector<CSlot*> pCreateBox = m_mapBox.find(CREATE)->second;

	pCreateBox[_iNum]->Set_Item(pItem);
}

void CSlotMgr::Remove_InvenItem(_uint _iItemNum, _uint _iItemCount)
{
	CItem* pItme = m_pItemArr[_iItemNum];

	pItme->MinusItemCount(_iItemCount);

	if (pItme->Get_ItemCount() <= 0)
	{
		Safe_Release(pItme);
		m_pItemArr[_iItemNum] = nullptr;
	}	
}

void CSlotMgr::Set_ArmorItem(ARMOR_SLOT_TYPE eArmorSlotType, CItem* pItem, _uint _iItemNum)
{
	// 아이템 장착 시 인벤 삭제
	m_pArmorArr[eArmorSlotType - 15] = pItem;
	m_pItemArr[_iItemNum] = nullptr;
}

_bool CSlotMgr::Check_InvenItemCount(wstring strName, _int iNeedNum)
{
	for (auto& iter : m_pItemArr)
	{
		if (iter == nullptr || strName != iter->GetObjName())
			continue;

		if (iter->Get_ItemCount() >= iNeedNum)
			return true;
	}

	return false;
}

void CSlotMgr::Remove_CreateItem(wstring strName, _uint iItemCount)
{
	for (int i =0; i < INVENCNT; ++i)
	{
		if (m_pItemArr[i] == nullptr || strName != m_pItemArr[i]->GetObjName())
			continue;

		m_pItemArr[i]->MinusItemCount(iItemCount);

		if (m_pItemArr[i]->Get_ItemCount() <= 0)
		{
			Safe_Release(m_pItemArr[i]);
			m_pItemArr[i] = nullptr;
		}
	}
}

void CSlotMgr::Change_ArmorItem(CItem* pItem, ARMOR_SLOT_TYPE eArmorSlotType, _uint _iItemNum, _vec3 vSlotPos)
{
	CItem* pMoveItem = m_pArmorArr[eArmorSlotType - 15];

	//좌표값 넣어주기
	pMoveItem->Set_fPrePos(vSlotPos);

	m_pItemArr[_iItemNum] = pMoveItem;
	m_pArmorArr[eArmorSlotType - 15] = pItem;
	dynamic_cast<CItemTool*>(pMoveItem)->Set_BoxIndex(_iItemNum);
}

void CSlotMgr::Remove_ArmorItem(ARMOR_SLOT_TYPE eArmorSlotType)
{
	Safe_Release(m_pArmorArr[eArmorSlotType - 15]);
	m_pArmorArr[eArmorSlotType - 15] = nullptr;
}

void CSlotMgr::Set_CookItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring strKeyName, _vec3 vSlotPos, _uint iSlotNum)
{
	CItem* pItem = CItemTool::Create(pGraphicDev, strKeyName, vSlotPos, UI_ITEM_COOK);

	if (m_pCookArr[iSlotNum] == nullptr)
		m_pCookArr[iSlotNum] = pItem;

}

void CSlotMgr::Remove_CookItem()
{
	for (auto& iter : m_pCookArr)
	{
		Safe_Release(iter);
	}
}

_bool CSlotMgr::Start_Cook()
{
	for (auto & iter : m_pCookArr)
	{
		if (iter != nullptr && (iter->GetObjName() == L"Cooked_Meat_Monster" || iter->GetObjName() == L"CookedMeat" || iter->GetObjName() == L"Meat_Monster" || iter->GetObjName() == L"RawMeat"))
			return true;
	}
	return false;
}

HRESULT CSlotMgr::Add_InvenBoxList(LPDIRECT3DDEVICE9 pGraphicDev, BOX_TYPE eType, BOX_DIR eDir, int _iNum)
{
	if (eType > BOX_END || eDir > BOX_DIR_END || 0 > _iNum)
		return E_FAIL;

	vector<CSlot*> pInvenBox;

	if (eType == INVEN) // 세로 방향 (인벤)
	{
		for (int i = 0; i < _iNum; ++i)
			pInvenBox.push_back(CInvenSlot::Create(pGraphicDev, _vec3(WINCX * 0.5f - 300.f + (15.f + 20.f) * i, WINCY - 17.f, 0.f), i, eType));
		for (int i = INVENCNT; i < _iNum +3; ++i)
			pInvenBox.push_back(CArmorSlot::Create(pGraphicDev, _vec3(WINCX * 0.5f - 300.f + (15.f + 20.f) * i, WINCY - 17.f, 0.f), i, (ARMOR_SLOT_TYPE)i));
	}
	if (eType == CREATE) // 가로 방향
	{
		for (int i = 0; i < _iNum; ++i)
			pInvenBox.push_back(CInvenSlot::Create(pGraphicDev, _vec3(25.f, 140.f + 40.f*i, 0.f), i, eType));
	}
	if (eType == COOK) // 가로 방향
	{
		for (int i = 0; i < _iNum; ++i)
			pInvenBox.push_back(CInvenSlot::Create(pGraphicDev, _vec3(450.f, 230.f + 35.f * i, 0.f), i, eType));
	}
	m_mapBox.emplace(eType, pInvenBox);
	return S_OK;
}

void CSlotMgr::Update_InvenBoxMgr(const _float& fTimeDelta, BOX_TYPE eType)
{
	vector<CSlot*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->Update_GameObject(fTimeDelta);

	if (eType == INVEN)
	{
		for (auto& inven : m_pItemArr)
			if(inven) inven->Update_GameObject(fTimeDelta);
		for (auto& iter : m_pArmorArr)
			if (iter) iter->Update_GameObject(fTimeDelta);
	}
	else if (eType == COOK)
	{
		for (auto& item : m_pCookArr)
			if (item) item->Update_GameObject(fTimeDelta);
	}
}

void CSlotMgr::LateUpdate_InvenBoxMgr(BOX_TYPE eType)
{
	vector<CSlot*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->LateUpdate_GameObject();

	if (eType == INVEN)
	{
		for (auto& inven : m_pItemArr)
			if (inven) inven->LateUpdate_GameObject();
		for (auto& iter : m_pArmorArr)
			if (iter) iter->LateUpdate_GameObject();
	}
	else if (eType == COOK)
	{
		for (auto& item : m_pCookArr)
			if (item) item->LateUpdate_GameObject();
	}
}

void CSlotMgr::Render_InvenBoxMgr(BOX_TYPE eType)
{
	vector<CSlot*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->Render_GameObject();

	if (eType == INVEN)
	{
		for (auto& inven : m_pItemArr)
			if (inven) inven->Render_GameObject();
		for (auto& iter : m_pArmorArr)
			if (iter) iter->Render_GameObject();
	}
	else if (eType == COOK)
	{
		for (auto& item : m_pCookArr)
			if (item) item->Render_GameObject();
	}
}
void CSlotMgr::Box_Release(BOX_TYPE eType)
{
	for (auto& iter : m_mapBox[eType])
		Safe_Release(iter);

	m_mapBox.erase(eType);
}

void CSlotMgr::Free()
{
	for (auto& vecBox : m_mapBox)
		for (auto& iter : vecBox.second)
			Safe_Release(iter);

	for (auto& iter : m_pItemArr)
		Safe_Release(iter);

	for (auto& iter : m_pArmorArr)
		Safe_Release(iter);

	for (auto& iter : m_pCookArr)
		Safe_Release(iter);
}
