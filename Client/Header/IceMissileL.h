#pragma once
#include "Effect.h"

namespace Engine {
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}
class IceMissileL :
    public CEffect
{

private:
	explicit IceMissileL(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit IceMissileL(const IceMissileL& rhs);
	virtual ~IceMissileL();
public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

	void Set_Target(CGameObject* _target, _vec3 _vtarget);
		

private:
	HRESULT					Add_Component();
	virtual void State_Change()			override;

	_int Appear(const _float& fTimeDelta);
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;

	_vec3 m_vTargetpos;
	_bool m_bShoot;
	CGameObject* m_pTarget;
public:
	static IceMissileL* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);


private:
	virtual void Free() override;
};

