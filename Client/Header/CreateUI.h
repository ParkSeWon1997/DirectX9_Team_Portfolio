#pragma once
#include "Export_Utility.h"
#include <SlideUI.h>
#include "UI.h"

class CCreateUI :
    public CUI
{
private:
    explicit CCreateUI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CCreateUI();

public:
    vector<CSlideUI*> Get_Slide() { return m_vecSlide; }

public:
    virtual HRESULT Ready_GameObject() override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject()					 override;
    virtual void Render_GameObject() override;

private:
    virtual HRESULT			Add_Component() override;

    void Picking_Mouse();

public:
    static CUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();

private:
    vector<CSlideUI*> m_vecSlide;
};

