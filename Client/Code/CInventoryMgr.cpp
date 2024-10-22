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


//������ ������н� false��ȯ
_bool CInventoryMgr::AddItem(CItem* _pItem)
{
	int i = 0;

	for (auto& itemBox : m_pItemArr)
	{
		if (nullptr == itemBox && _pItem->IsEquipment())// ������ĭ�� ����� ���������� �����������
		{
			itemBox = _pItem;
			return true;
		}

		if (nullptr != itemBox && (!_pItem->IsEquipment()))//���������ְ� �Ҹ�ǰ�������������
		{
			if (itemBox->GetObjName()== _pItem->GetObjName())//�ش�ĭ�� �̸��� ���� ���������������
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