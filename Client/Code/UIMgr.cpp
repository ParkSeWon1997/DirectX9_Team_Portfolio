#include "stdafx.h"
#include "UIMgr.h"
#include"CItem.h"
#include "SlotMgr.h"
#include <ItemTool.h>

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
	Free();
}

CREATEINFO CUIMgr::Get_CreateInfo(wstring pKey)
{
	for (auto& iter : m_vecCreateInfo)
	{
		if (iter.strKeyName == pKey)
			return iter;
	}

	return CREATEINFO();
}



void CUIMgr::Ready_CreateInfo()
{
	CREATEINFO tInfo;

	ZeroMemory(&tInfo, sizeof(tInfo));

	// ��Ȱ��ǰ
	tInfo.strKeyName = L"Ax";
	tInfo.strName = L"����";
	tInfo.strInfo = L"������\n�� �� �ִ�.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 1;
	tInfo.tItemInfo[1].strItemName = L"FireSton";
	tInfo.tItemInfo[1].iCount = 1;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Pickaxe";
	tInfo.strName = L"���";
	tInfo.strInfo = L"������\nĶ �� �ִ�.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"FireSton";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Hammer";
	tInfo.strName = L"��ġ";
	tInfo.strInfo = L"�����̵�\n�μ� �� �ִ�.";
	tInfo.iInfoCount = 3;
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 3;
	tInfo.tItemInfo[1].strItemName = L"Rocks_0";
	tInfo.tItemInfo[1].iCount = 3;
	tInfo.tItemInfo[2].strItemName = L"CutGlass";
	tInfo.tItemInfo[2].iCount = 6;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Cook";
	tInfo.strName = L"�丮 ����";
	tInfo.strInfo = L"���ִ� �丮��\n�� �� �ִ�.";
	tInfo.iInfoCount = 3;
	tInfo.tItemInfo[0].strItemName = L"CutStone";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"Charcoal";
	tInfo.tItemInfo[1].iCount = 6;
	tInfo.tItemInfo[2].strItemName = L"Twigs";
	tInfo.tItemInfo[2].iCount = 6;
	m_vecCreateInfo.push_back(tInfo);

	//���� ��ǰ
	tInfo.strKeyName = L"Torch";
	tInfo.strName = L"ȶ��";
	tInfo.strInfo = L"���������\n�ֺ��� �����ش�.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"CutGlass";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"BonFire";
	tInfo.strName = L"��ں�";
	tInfo.strInfo = L"��ġ�ϸ� �ֺ���\nȯ�ϰ� �����ش�.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"Log";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"CutGlass";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Tent";
	tInfo.strName = L"��Ʈ";
	tInfo.strInfo = L"�����ϰ� ����\n�� �� �ִ�.";
	tInfo.iInfoCount = 3;
	tInfo.tItemInfo[0].strItemName = L"Silk";
	tInfo.tItemInfo[0].iCount = 6;
	tInfo.tItemInfo[1].strItemName = L"Twigs";
	tInfo.tItemInfo[1].iCount = 4;
	tInfo.tItemInfo[2].strItemName = L"Rope";
	tInfo.tItemInfo[2].iCount = 3;
	m_vecCreateInfo.push_back(tInfo);

	//������ǰ
	tInfo.strKeyName = L"Lance";
	tInfo.strName = L"����â";
	tInfo.strInfo = L"��� �����Ѵٸ�\n���� ��ų �� �ִ�.";
	tInfo.iInfoCount = 3;
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"FireSton";
	tInfo.tItemInfo[1].iCount = 1;
	tInfo.tItemInfo[2].strItemName = L"Rope";
	tInfo.tItemInfo[2].iCount = 1;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Armor";
	tInfo.strName = L"Ǯ����";
	tInfo.strInfo = L"���� ��~~�� ����\n��ȣ�� �� �ִ�.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"CutGlass";
	tInfo.tItemInfo[0].iCount = 10;
	tInfo.tItemInfo[1].strItemName = L"Twigs";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"LogSuit";
	tInfo.strName = L"��������";
	tInfo.strInfo = L"���� ưư�ϰ�\n��ȣ�� �� �ִ�.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"Log";
	tInfo.tItemInfo[0].iCount = 8;
	tInfo.tItemInfo[1].strItemName = L"Rope";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	//���
	tInfo.strKeyName = L"Rope";
	tInfo.strName = L"����";
	tInfo.strInfo = L"���� �� �ִ�.";
	tInfo.iInfoCount = 1;
	tInfo.tItemInfo[0].strItemName = L"CutGlass";
	tInfo.tItemInfo[0].iCount = 3;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Charcoal";
	tInfo.strName = L"��";
	tInfo.strInfo = L"������ �¿�\n�����.";
	tInfo.iInfoCount = 1;
	tInfo.tItemInfo[0].strItemName = L"Log";
	tInfo.tItemInfo[0].iCount = 3;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"CutStone";
	tInfo.strName = L"����";
	tInfo.strInfo = L"������ �ܴ��ϴ�";
	tInfo.iInfoCount = 1;
	tInfo.tItemInfo[0].strItemName = L"Rocks_0";
	tInfo.tItemInfo[0].iCount = 3;
	m_vecCreateInfo.push_back(tInfo);

}

void CUIMgr::Free()
{
	//m_pItemArr[0].
	//__super::Free();
}