#pragma once
#include "CTrap.h"
class CCataProj :
	public CTrap
{
protected:
	explicit CCataProj(LPDIRECT3DDEVICE9 pGraphicDev) = delete;
	explicit CCataProj(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CCataProj(const CCataProj& rhs);
	virtual ~CCataProj();

public:
	void SetSpeed(_float _speed) { m_fSpeed = _speed; }
public:
	virtual HRESULT			Ready_GameObject()override;
	// CTrap을(를) 통해 상속됨
	virtual _int			Update_GameObject(const _float& fTimeDelta)override;
	void Render_GameObject() override;

public:
	static CCataProj* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos);

private:
	void MovePos(const _float& fTimeDelta);
protected:
	HRESULT Add_Component() override;
	void PlayHitSound();
private:
	float m_fSpeed;
};
