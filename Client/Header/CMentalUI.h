#pragma once
#include "StateUI.h"

class CMentalUI :public CStateUI
{
private:
	explicit CMentalUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMentalUI();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component() override;
	void Check_State() override;

public:
	static CMentalUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

