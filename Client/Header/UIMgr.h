#pragma once

#include "Engine_Define.h"
#include "Base.h"

class CItem;

class CUIMgr :
	public CBase
{
	DECLARE_SINGLETON(CUIMgr)

private:
	explicit CUIMgr();
	virtual ~CUIMgr();

public:
	CREATEINFO Get_CreateInfo(wstring pKey);

public:
	void Ready_CreateInfo();

private:
	virtual void Free();

private:


	// »ý»ê¹ý
	vector<CREATEINFO> m_vecCreateInfo;


};

