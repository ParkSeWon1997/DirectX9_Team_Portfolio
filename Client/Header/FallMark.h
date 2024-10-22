#pragma once
#include "Effect.h"

namespace Engine {
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}
class FallMark :
    public CEffect
{
	enum FALLMARK {	APPEAR,DISAPPEAR, STATE_END	};
private:
	explicit FallMark(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit FallMark(const FallMark& rhs);
	virtual ~FallMark();
public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	void Set_Dead();

private:
	HRESULT					Add_Component();
	virtual void State_Change()			override;

	_int Appear();


private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[FALLMARK::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

private:
	FALLMARK m_eCurState;
	FALLMARK m_ePreState;

	_bool	m_bMissileLaunch;

public:
	static FallMark* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);


private:
	virtual void Free() override;
};

