#pragma once
#include "CTrap.h"
class CTumbleWeed :
	public CTrap
{
protected:
	explicit CTumbleWeed(LPDIRECT3DDEVICE9 pGraphicDev) = delete;
	explicit CTumbleWeed(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CTumbleWeed(const CTumbleWeed& rhs);
	virtual ~CTumbleWeed();

public:
	// CTrap을(를) 통해 상속됨
	virtual _int			Update_GameObject(const _float& fTimeDelta)override;
	void Render_GameObject() override;

public:
	static CTumbleWeed* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos);
	static CTumbleWeed* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);

private:
	void MovePos(const _float& fTimeDelta);
protected:
	HRESULT Add_Component() override;

};

