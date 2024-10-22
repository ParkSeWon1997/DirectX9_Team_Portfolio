#pragma once
#include "Export_Utility.h"
class CToolBossScene :
    public CScene
{
private:
    explicit CToolBossScene(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CToolBossScene();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

private:
	HRESULT Input_Mouse();
	_vec3 Picking_Terrain();

private:
	HRESULT			Ready_Layer_Environment();
	HRESULT			Ready_Layer_GameLogic();
	HRESULT			Ready_LightInfo();

private:
	void Save_ObjFile();
	HRESULT Load_ObjFile();
	void Save_PointFile();
	HRESULT Load_PointFile();
	void Save_LightFile();
	HRESULT Load_LightFile();

	HRESULT Create_Object(const _tchar* pName, _vec3 vPos, _vec3 vScale);
	HRESULT Change_LightInfo(const _float& fTimeDelta);

private:
	Engine::CCalculator* m_pCalculator;

	_vec3 vLight[3] = { { 1.f, 1.f, 1.f} };

public:
	static CToolBossScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

