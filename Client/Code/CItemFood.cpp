#include "stdafx.h"

#include "../Header/CItemFood.h"
#include "Export_System.h"
#include "Export_Utility.h"

CItemFood::CItemFood(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _key)
	:CItem(pGraphicDev, _key)
{

}

CItemFood::~CItemFood()
{

}

_bool CItemFood::UseItem()
{
	return _bool();
}

_bool CItemFood::DropItem()
{
	return _bool();
}
