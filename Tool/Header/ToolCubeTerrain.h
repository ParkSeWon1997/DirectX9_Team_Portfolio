#pragma once
#include "Export_Utility.h"

class CToolCubeTerrain :
    public CGameObject
{
private:
    explicit CToolCubeTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CToolCubeTerrain();

public:
    vector<CGameObject*> Get_TileList() { return m_pCubeCom; }

public:
    virtual HRESULT Ready_GameObject()						 override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject()					 override;
    virtual void Render_GameObject()override;

private:
    HRESULT Add_Component();

    void Input_Mouse();

    HRESULT Picking_OnTerrain();

private:
    vector<CGameObject*> m_pCubeCom; 
    Engine::CCalculator* m_pCalculatorCom;

    LPDIRECT3DDEVICE9 m_pGraphicDev;

public:
    static CToolCubeTerrain* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
    virtual void Free() override;

};

