#pragma once
#include "CTrap.h"
class CGeyserTrap :
	public CTrap
{
protected:
	explicit CGeyserTrap(LPDIRECT3DDEVICE9 pGraphicDev) = delete;
	explicit CGeyserTrap(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CGeyserTrap(const CGeyserTrap& rhs);
	virtual ~CGeyserTrap();


public:
	// CTrap을(를) 통해 상속됨
	virtual _int			Update_GameObject(const _float& fTimeDelta)override;

	void Render_GameObject() override;

public:
	static CGeyserTrap* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos);
	static CGeyserTrap* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);

protected:
	HRESULT Add_Component() override;

private:

	_float	 m_fAccTime;
	_float	 m_fOpenTime;
	_float	m_fLoopTime;

	_float	 m_fPlayerHit;


};

