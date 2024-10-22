#pragma once
#include "Export_Utility.h"
#include "UI.h"

class CStateUI :
    public CUI
{
protected:
    explicit CStateUI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CStateUI();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT Add_Component() PURE;
	virtual void Check_State() PURE;

protected:
	virtual void Free();

protected:
	_float m_frame; //출력될 사진 인덱스
	_float m_frameEnd; //프레임이 시작되고나서 움직이는 값
	_float m_PreFrame;
	BOOL m_bFrameStop = true;
};

