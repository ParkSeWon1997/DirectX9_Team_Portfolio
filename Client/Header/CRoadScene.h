#pragma once

#include "Scene.h"

class CRoadScene : public Engine::CScene
{
private:
	explicit CRoadScene(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName);
	virtual ~CRoadScene();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

private:

	HRESULT			Ready_Layer_Environment();
	HRESULT			Ready_Layer_GameLogic();
	HRESULT			Ready_Layer_UI();
	HRESULT			Ready_LightInfo();

private:
	HRESULT Load_Data();
	HRESULT Change_LightInfo(const _float& fTimeDelta);
	HRESULT Create_Object(const _tchar* pName, _vec3 vPos, _vec3 vScale);
private:
	_vec3 m_vDirectionDiffuseColor[3] = {};
	_vec3 m_vDirectionAmbientColor[3] = {};
	_vec3 m_vDirectionSpecularColor[3] = {};

	public:
	static CRoadScene* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName);

private:
	virtual void Free() override;

};

