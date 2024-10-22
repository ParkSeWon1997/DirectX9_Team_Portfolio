#pragma once
#include "Slot.h"
class CInvenSlot :
    public CSlot
{
private:
    explicit CInvenSlot(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _uint iIndex, BOX_TYPE eType);
    virtual ~CInvenSlot();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

public:
	virtual HRESULT			Add_Component() override;

public:
	static CSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _uint iIndex, BOX_TYPE eType = INVEN);

private:
	virtual void Free();
};

