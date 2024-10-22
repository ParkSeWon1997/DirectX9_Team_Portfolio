#pragma once
#include "Export_Utility.h"
class CRunTerrain :
    public CGameObject
{
private:
    explicit CRunTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CRunTerrain();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_Material();

private:
	vector<CGameObject*> m_pCubeCom;

public:
	static CRunTerrain* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};

