#pragma once
#include "CTrap.h"
class CToothTrap :
	public CTrap
{
protected:
	explicit CToothTrap(LPDIRECT3DDEVICE9 pGraphicDev) = delete;
	explicit CToothTrap(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CToothTrap(const CToothTrap& rhs);
	virtual ~CToothTrap();


public:
	// CTrap을(를) 통해 상속됨
	virtual _int			Update_GameObject(const _float& fTimeDelta)override;

	void Render_GameObject() override;

public:
	static CToothTrap* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos);
	static CToothTrap* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);

protected:
	HRESULT Add_Component() override;

};

