#pragma once
#include "ResObject.h"
class CBerryBush :
    public CResObject
{
    enum BERRYBUSHSTATE { BERRY_IDLE, BERRY_PICK, BERRY_EMPTY, BERRY_EMPTY_PICK, BERRY_END };
private:
    explicit CBerryBush(LPDIRECT3DDEVICE9 pGrahpicDev);
    virtual ~CBerryBush();

public:
    virtual HRESULT Ready_GameObject()						 override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject()					 override;
    virtual void Render_GameObject()						override;

private:
    HRESULT Add_Component() override;
    void Change_Frame_Event() override;
    void Check_FrameState() override;
    void Ready_Stat() override;

private:
    Engine::CTexture* m_pBerryTextureCom[BERRY_END];

    BERRYBUSHSTATE m_eBerryCurState;
    BERRYBUSHSTATE m_eBerryPreState;

public:
    static CResObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();

    BOOL _bDropItem = false;
};

