#pragma once
#include "Effect.h"

namespace Engine {
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}
class CGhost :
    public CEffect
{
	enum GHOSTSTATE {	APPEAR, IDLE,  STATE_END	};
private:
	explicit CGhost(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CGhost(const CGhost& rhs);
	virtual ~CGhost();
public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
private:
	HRESULT					Add_Component();
	virtual void State_Change()			override;
	void Appear();
	void Set_Scale();
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTexture* m_pTextureCom[GHOSTSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

private:
	GHOSTSTATE m_eCurState;
	GHOSTSTATE m_ePreState;

	_bool m_bAppear;

public:
	static CGhost* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);


private:
	virtual void Free() override;
};

