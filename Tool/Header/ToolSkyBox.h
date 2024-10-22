#pragma once
#include "Export_Utility.h"

class CToolSkyBox :
    public CGameObject
{
private:
    explicit CToolSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CToolSkyBox(const CToolSkyBox& rhs);
    virtual ~CToolSkyBox();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransForm;
	Engine::CTexture* m_pTextureCom;


public:
    static CToolSkyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();

};

