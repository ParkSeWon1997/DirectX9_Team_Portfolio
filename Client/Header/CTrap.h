#pragma once
#include "GameObject.h"
#include "Engine_Define.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CAnimator;
}

class CTrap :
	public CGameObject
{
protected:
	explicit CTrap() = delete;
	explicit CTrap(LPDIRECT3DDEVICE9 pGraphicDev) = delete;
	explicit CTrap(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CTrap(const CTrap& rhs);
	virtual ~CTrap();

public:
	virtual HRESULT			Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta)override
	{
		renderer::Add_RenderGroup(RENDER_ALPHA, this);
		return __super::Update_GameObject(fTimeDelta);

	}
	virtual void			LateUpdate_GameObject();
	virtual void			Render_GameObject() PURE;

protected:
	_bool PlayerHit(float _fDistance);


	virtual HRESULT			Add_Component() PURE;
	virtual void Free()override;

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CAnimator* m_pAnimCom;
	_bool m_bHit; // 플레이어맞았을경우
	eTRAP_STATE m_eCurState;


};

