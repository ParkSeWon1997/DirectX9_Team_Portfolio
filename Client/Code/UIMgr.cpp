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

	// 생활용품
	tInfo.strKeyName = L"Ax";
	tInfo.strName = L"도끼";
	tInfo.strInfo = L"나무를\n밸 수 있다.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 1;
	tInfo.tItemInfo[1].strItemName = L"FireSton";
	tInfo.tItemInfo[1].iCount = 1;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Pickaxe";
	tInfo.strName = L"곡괭이";
	tInfo.strInfo = L"바위를\n캘 수 있다.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"FireSton";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Hammer";
	tInfo.strName = L"망치";
	tInfo.strInfo = L"무엇이든\n부술 수 있다.";
	tInfo.iInfoCount = 3;
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 3;
	tInfo.tItemInfo[1].strItemName = L"Rocks_0";
	tInfo.tItemInfo[1].iCount = 3;
	tInfo.tItemInfo[2].strItemName = L"CutGlass";
	tInfo.tItemInfo[2].iCount = 6;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Cook";
	tInfo.strName = L"요리 도구";
	tInfo.strInfo = L"맛있는 요리를\n할 수 있다.";
	tInfo.iInfoCount = 3;
	tInfo.tItemInfo[0].strItemName = L"CutStone";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"Charcoal";
	tInfo.tItemInfo[1].iCount = 6;
	tInfo.tItemInfo[2].strItemName = L"Twigs";
	tInfo.tItemInfo[2].iCount = 6;
	m_vecCreateInfo.push_back(tInfo);

	//생존 용품
	tInfo.strKeyName = L"Torch";
	tInfo.strName = L"횃불";
	tInfo.strInfo = L"들고있으면\n주변을 밝혀준다.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"CutGlass";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"BonFire";
	tInfo.strName = L"모닥불";
	tInfo.strInfo = L"설치하면 주변을\n환하게 밝혀준다.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"Log";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"CutGlass";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Tent";
	tInfo.strName = L"텐트";
	tInfo.strInfo = L"안전하게 잠을\n잘 수 있다.";
	tInfo.iInfoCount = 3;
	tInfo.tItemInfo[0].strItemName = L"Silk";
	tInfo.tItemInfo[0].iCount = 6;
	tInfo.tItemInfo[1].strItemName = L"Twigs";
	tInfo.tItemInfo[1].iCount = 4;
	tInfo.tItemInfo[2].strItemName = L"Rope";
	tInfo.tItemInfo[2].iCount = 3;
	m_vecCreateInfo.push_back(tInfo);

	//전투용품
	tInfo.strKeyName = L"Lance";
	tInfo.strName = L"전투창";
	tInfo.strInfo = L"들고 공격한다면\n몸을 지킬 수 있다.";
	tInfo.iInfoCount = 3;
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"FireSton";
	tInfo.tItemInfo[1].iCount = 1;
	tInfo.tItemInfo[2].strItemName = L"Rope";
	tInfo.tItemInfo[2].iCount = 1;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Armor";
	tInfo.strName = L"풀갑옷";
	tInfo.strInfo = L"몸을 아~~주 조금\n보호할 수 있다.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"CutGlass";
	tInfo.tItemInfo[0].iCount = 10;
	tInfo.tItemInfo[1].strItemName = L"Twigs";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"LogSuit";
	tInfo.strName = L"나무갑옷";
	tInfo.strInfo = L"몸을 튼튼하게\n보호할 수 있다.";
	tInfo.iInfoCount = 2;
	tInfo.tItemInfo[0].strItemName = L"Log";
	tInfo.tItemInfo[0].iCount = 8;
	tInfo.tItemInfo[1].strItemName = L"Rope";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	//재료
	tInfo.strKeyName = L"Rope";
	tInfo.strName = L"밧줄";
	tInfo.strInfo = L"묶을 수 있다.";
	tInfo.iInfoCount = 1;
	tInfo.tItemInfo[0].strItemName = L"CutGlass";
	tInfo.tItemInfo[0].iCount = 3;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Charcoal";
	tInfo.strName = L"숯";
	tInfo.strInfo = L"나무를 태워\n만든다.";
	tInfo.iInfoCount = 1;
	tInfo.tItemInfo[0].strItemName = L"Log";
	tInfo.tItemInfo[0].iCount = 3;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"CutStone";
	tInfo.strName = L"석재";
	tInfo.strInfo = L"돌보다 단단하다";
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