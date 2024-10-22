#pragma once
#include "Base.h"
//#include "framework.h"
#include "Engine_Define.h"
#include "ToolTerrain.h"
#include "ToolMgr.h"

namespace Engine {
class CSceneMgr;
class CGraphicDev;
class CTexture;
}

class CMainTool :
    public CBase
{
private:
    explicit CMainTool();
    virtual ~CMainTool();

public:
    HRESULT Ready_MainTool();
    void Update_MainTool(const _float& fTimeDelta);
    void LateUpdate_MainTool();
    void Render_MainTool();

private:
    HRESULT		Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CSceneMgr** ppManagement);
    HRESULT		SetUp_Setting(LPDIRECT3DDEVICE9* ppGraphicDev);

private:
    Engine::CGraphicDev* m_pDeviceClass;
    Engine::CSceneMgr* m_pManagementClass;
    LPDIRECT3DDEVICE9			m_pGraphicDev;
    CToolMgr* m_pToolMgr;

public:
    static CMainTool* Create();

private:
    virtual void Free();

};

