#pragma once
#include "Effect.h"

namespace Engine {
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}
class CRebirth :
    public CEffect
{
	
private:
	explicit CRebirth(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CRebirth(const CRebirth& rhs);
	virtual ~CRebirth();
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
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;

public:
	static CRebirth* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);


private:
	virtual void Free() override;
};

