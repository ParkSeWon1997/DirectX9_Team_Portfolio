#pragma once

#include "../Header/CItem.h"

class CItemFood :
	public CItem
{
public:
	explicit CItemFood(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _key);
	virtual ~CItemFood();

public:
	virtual _bool UseItem() override;
	virtual _bool DropItem() override;
};

