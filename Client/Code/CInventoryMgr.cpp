#include "stdafx.h"
#include "CInventoryMgr.h"
#include"CItem.h"


IMPLEMENT_SINGLETON(CInventoryMgr)

CInventoryMgr::CInventoryMgr()
{
	for (int i = 0; i < 15; i++)
	{
		m_pItemArr[i] = nullptr;
	}
}

CInventoryMgr::~CInventoryMgr()
{

}


//아이템 습득실패시 false반환
_bool CInventoryMgr::AddItem(CItem* _pItem)
{
	int i = 0;

	for (auto& itemBox : m_pItemArr)
	{
		if (nullptr == itemBox && _pItem->IsEquipment())// 아이템칸이 비었고 장비아이템을 습득했을경우
		{
			itemBox = _pItem;
			return true;
		}

		if (nullptr != itemBox && (!_pItem->IsEquipment()))//아이템이있고 소모품을습득했을경우
		{
			if (itemBox->GetObjName()== _pItem->GetObjName())//해당칸의 이름이 같은 아이템이있을경우
			{
				itemBox->AddItemCount(_pItem->GetItemInfo().ItemCount);
				return true;
			}
		}
	}

	for (auto& itemBox : m_pItemArr)
	{
		if (itemBox == nullptr)
		{
			itemBox = _pItem;
			return true;
		}
	}

	return false;
}

CItem* CInventoryMgr::GetItemBox(_uint _uint)

{
	if (nullptr != m_pItemArr[_uint])
	{
		return m_pItemArr[_uint];
	}
	else
	{
		return nullptr;
	}
}

void CInventoryMgr::Free()
{
	for (auto iter : m_pItemArr)
	{
		
	}
}