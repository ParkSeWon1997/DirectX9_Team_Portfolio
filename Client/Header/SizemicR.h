#pragma once
#include "Effect.h"

namespace Engine {
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}
class SizemicR :
    public CEffect
{
	enum SIZEMIC { ONE, TWO, THREE, ERASE ,STATE_END };
private:
	explicit SizemicR(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit SizemicR(const SizemicR& rhs);
	virtual ~SizemicR();
public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

	void Set_Number(_int _index) {	m_eCurState = (SIZEMIC)_index;	}

private:
	HRESULT					Add_Component();
	virtual void State_Change()			override;

	_int Appear();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[SIZEMIC::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

private:
	SIZEMIC m_eCurState;
	SIZEMIC m_ePreState;

	_bool	m_bAttack;

public:
	static SizemicR* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	virtual void Free() override;
};

