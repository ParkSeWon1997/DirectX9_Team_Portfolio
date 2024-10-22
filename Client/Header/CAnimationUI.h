#pragma once
#include "GameObject.h"


namespace Engine
{
	class CRcTex;
	class CRvRcTex;
	class CAnimator;
}


class CAnimationUI :
	public CGameObject
{


public:
	virtual HRESULT			Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _int			Update_GameObject(const _float& fTimeDelta) { return __super::Update_GameObject(fTimeDelta); }
	//virtual void			LateUpdate_GameObject();
	virtual void			Render_GameObject() override;
	virtual HRESULT			Add_Component()PURE;

protected:
	virtual void Free()override;

protected:
	explicit CAnimationUI() = delete;
	explicit CAnimationUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimationUI(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName);
	virtual ~CAnimationUI();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CAnimator* m_pAnimCom;

	_float m_fX;
	_float m_fY;

};

