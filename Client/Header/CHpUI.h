#pragma once
#include "UI.h"
#include "StateUI.h"
class CHpUI : public CStateUI
{
private:
	explicit CHpUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHpUI(const CHpUI& rhs);
	virtual ~CHpUI();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component() override;
	void Check_State() override;

public:
	static CHpUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

