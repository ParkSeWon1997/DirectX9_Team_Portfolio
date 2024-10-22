#pragma once

#include "Scene.h"

#include "Loading.h"

class CLoadingScene : public Engine::CScene
{
private:
	explicit CLoadingScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLoadingScene(LPDIRECT3DDEVICE9 pGraphicDev, CLoading::LOADINGID m_eLoadingID);
	virtual ~CLoadingScene();

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

private:
	CLoading*		m_pLoading;
	CLoading::LOADINGID		m_eLoadingID;
public:
	static CLoadingScene*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CLoading::LOADINGID _eLoadingID);

private:
	virtual void Free() override;

};

