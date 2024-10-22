#pragma once
#include "CParticle.h"
class CRockSmoke :
	public CParticle
{
protected:
	explicit CRockSmoke(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CRockSmoke(const CParticle& rhs);
	virtual ~CRockSmoke() override;

	virtual void resetParticle(Attribute* attribute);


public:
	virtual _int			Update_GameObject(const _float& fTimeDelta)override;


	//매개변수( 디바이스,텍스처이미지(basic아이템과같음),파티클개수,위치,파티클크기,파티클보이는시간,파티클속도)
	static CRockSmoke* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _vec3 _vPos, _float _fSize, _float _fLifeTime, _float _fSpeed);

private:

	_float m_fLifeTime;
	_float m_fAccTime;
	_float m_Speed;

};

