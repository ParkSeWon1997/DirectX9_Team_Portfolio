#pragma once
#include "CParticle.h"
class CSnow :
	public CParticle
{
protected:
	explicit CSnow(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CSnow(const CParticle& rhs);
	virtual ~CSnow() override;

public:
	virtual void resetParticle(Attribute* attribute);


public:
	virtual _int			Update_GameObject(const _float& fTimeDelta)override;

	static CSnow* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _vec3 MinBox, _vec3 MaxBox);

};

