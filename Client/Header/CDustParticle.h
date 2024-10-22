#pragma once
#include "CParticle.h"
class CDustParticle :
	public CParticle
{
protected:
	explicit CDustParticle(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CDustParticle(const CParticle& rhs);
	virtual ~CDustParticle() override;

	virtual void resetParticle(Attribute* attribute);


public:
	virtual _int			Update_GameObject(const _float& fTimeDelta)override;


	//�Ű�����( ����̽�,�ؽ�ó�̹���(basic�����۰�����),��ƼŬ����,��ġ,��ƼŬũ��,��ƼŬ���̴½ð�,��ƼŬ�ӵ�)
	static CDustParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _vec3 _vPos, _float _fSize, _float _fLifeTime, _float _fSpeed);


	_float m_fLifeTime;
	_float m_fAccTime;
	_float m_Speed;

};

