#pragma once
#include "Effect.h"

namespace Engine {
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}
class CBossEftDown :
    public CEffect
{
	enum BE_UPSTATE {	APPEAR,DEAD, STATE_END	};
private:
	explicit CBossEftDown(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CBossEftDown(const CBossEftDown& rhs);
	virtual ~CBossEftDown();
public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	void		Set_Look_Dir(LOOKDIR _Look) { m_eCurLook = _Look; }
private:
	HRESULT					Add_Component();
	virtual void State_Change()			override;

	
	_int Appear();
	void Set_Scale();
	void Look_For_Change();
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;

private:
	BE_UPSTATE m_eCurState;
	BE_UPSTATE m_ePreState;


	_bool		m_bCollision;
public:
	static CBossEftDown* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);


private:
	virtual void Free() override;
};

