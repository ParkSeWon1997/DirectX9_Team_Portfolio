#pragma once
#include "CParticle.h"
class CTreeLeafFall :
	public CParticle
{
protected:
	explicit CTreeLeafFall(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CTreeLeafFall(const CParticle& rhs);
	virtual ~CTreeLeafFall() override;

public:
	virtual void resetParticle(Attribute* attribute)override;

	virtual void LateUpdate_GameObject()override;


public:
	virtual _int			Update_GameObject(const _float& fTimeDelta)override;

	static CTreeLeafFall* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _vec3 _vPos, _vec3 MinBox, _vec3 MaxBox);

private:
	_float	m_fAccTime;
	_float	m_fLifeTime;
};

