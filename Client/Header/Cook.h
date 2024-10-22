#pragma once
#include "Export_Utility.h"
#include "UI.h"
class CCook :
    public CUI
{
private:
    explicit CCook(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CCook();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

public:
	void IsShow(_bool bShow) { m_bShow = bShow; }

private:
	virtual HRESULT Add_Component();

public:
	static CCook* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;

private:
	_bool m_bShow;

	CButtonUI* pButton;

};

