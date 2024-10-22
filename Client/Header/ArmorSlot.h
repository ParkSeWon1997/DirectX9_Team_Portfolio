#pragma once
#include "Slot.h"
#include "Engine_Define.h"

class CArmorSlot :
    public CSlot
{
private:
    explicit CArmorSlot(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _uint iIndex, ARMOR_SLOT_TYPE eType);
    virtual ~CArmorSlot();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

public:
	virtual HRESULT			Add_Component();

private:
	ARMOR_SLOT_TYPE m_eArmorType;
	
public:
	static CSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _uint iIndex, ARMOR_SLOT_TYPE eType);

private:
	virtual void Free();

};

