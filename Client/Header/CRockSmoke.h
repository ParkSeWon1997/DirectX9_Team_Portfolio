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


	//�Ű�����( ����̽�,�ؽ�ó�̹���(basic�����۰�����),��ƼŬ����,��ġ,��ƼŬũ��,��ƼŬ���̴½ð�,��ƼŬ�ӵ�)
	static CRockSmoke* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _vec3 _vPos, _float _fSize, _float _fLifeTime, _float _fSpeed);

private:

	_float m_fLifeTime;
	_float m_fAccTime;
	_float m_Speed;

};

