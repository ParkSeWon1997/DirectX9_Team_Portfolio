#pragma once
#include "CTrap.h"
class CSpike :
	public CTrap
{
protected:
	explicit CSpike(LPDIRECT3DDEVICE9 pGraphicDev) = delete;
	explicit CSpike(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CSpike(const CSpike& rhs);
	virtual ~CSpike();


public:
	// CTrap을(를) 통해 상속됨
	virtual _int			Update_GameObject(const _float& fTimeDelta)override;

	void Render_GameObject() override;

public:
	static CSpike* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos);
	static CSpike* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);

protected:
	HRESULT Add_Component() override;

};

