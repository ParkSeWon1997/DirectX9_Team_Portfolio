#pragma once
#include "Effect.h"

namespace Engine {
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}
class IceSpikeR :
    public CEffect
{
	enum ICESPIKE { ONE, TWO, THREE, STATE_END };
private:
	explicit IceSpikeR(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit IceSpikeR(const IceSpikeR& rhs);
	virtual ~IceSpikeR();
public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT					Add_Component();
	virtual void State_Change()			override;

	_int Appear();


private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[ICESPIKE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

private:
	ICESPIKE m_eCurState;
	ICESPIKE m_ePreState;

	_bool	m_bAttack;

public:
	static IceSpikeR* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	virtual void Free() override;
};
