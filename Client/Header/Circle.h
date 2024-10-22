#pragma once
#include "Effect.h"

namespace Engine {
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}
class CCircle :
    public CEffect
{
	enum CIRCLESTATE {	APPEAR,DEAD, STATE_END	};
private:
	explicit CCircle(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CCircle(const CCircle& rhs);
	virtual ~CCircle();
public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	void		Set_Count(_int _icount) { m_iCountMax = _icount; }
private:
	HRESULT					Add_Component();
	virtual void State_Change()			override;
	_int Appear();
	void Generate_SnowSplash();				//눈 터지는 효과 생성하기

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;

private:
	CIRCLESTATE m_eCurState;
	CIRCLESTATE m_ePreState;

	_int	m_iCount;
	_int	m_iCountMax;
public:
	static CCircle* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);


private:
	virtual void Free() override;
};

