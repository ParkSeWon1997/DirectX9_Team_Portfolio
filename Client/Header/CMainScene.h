#pragma once

#include "Scene.h"

#include "Loading.h"

class CMainScene : public Engine::CScene
{
private:
	explicit CMainScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMainScene();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

private:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag) { return S_OK; }
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag) { return S_OK; }
	HRESULT			Ready_LoadingObject();

public:
	static CMainScene*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

