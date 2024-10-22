#pragma once
#include "StateUI.h"
class CWorldHand :
    public CStateUI
{
private:
    explicit CWorldHand(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CWorldHand();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component() override;
	void Check_State() override;

public:
	static CWorldHand* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

private:
	_float m_fWorldTime = 0.f;
};

