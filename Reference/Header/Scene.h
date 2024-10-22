#pragma once

#include "Engine_Define.h"
#include "Base.h"


namespace Engine
{
	class CCamera;
	class CLayer;
	class CGameObject;
	class ENGINE_DLL CScene :public CBase
	{

	public:
		const vector<CGameObject*>& GetGroupObject(eLAYER_TYPE _eLayerType, eOBJECT_GROUPTYPE _eObjGroupType);

	public:
		void	BeginOrtho();
		void	EndOrtho();
		_matrix* Get_OrthoViewMatrix();
		_matrix* Get_OrthoMatrix();

		//³ªÁß¿¡ ½Ì±ÛÅæÀ¸·Î ±³Ã¼
		CGameObject* GetTerrainObject() { return m_pTerrain; }
		CGameObject* GetPlayerObject() { return m_pPlayer; }
		CGameObject* GetMouseObject() { return m_pMouse; }
		CCamera* GetCameraObject() { return m_pCamera; }
		CLayer* GetLayer(eLAYER_TYPE _eLayerType) { return m_arrLayer[(int)_eLayerType]; }
		HRESULT		AddGameObject(eLAYER_TYPE _eLayerType, eOBJECT_GROUPTYPE _eObjType, CGameObject* pGameObject);
		wstring Get_Scene_Name() { return m_strSceneName; }

	public:
		virtual HRESULT		Ready_Scene();
		virtual _int		Update_Scene(const _float& fTimeDelta);
		virtual void		LateUpdate_Scene();
		virtual void		Render_Scene()	PURE;
		virtual void		Update_Event();

	public:
		virtual void Free();

	protected:
		explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName);
		virtual ~CScene();

	protected:
		LPDIRECT3DDEVICE9		m_pGraphicDev;
		CLayer* m_arrLayer[(int)eLAYER_TYPE::END];
		wstring					m_strSceneName;
		CCamera* m_pCamera;

		//³ªÁß¿¡ ½Ì±ÛÅæÀ¸·Î ±³Ã¼
		CGameObject* m_pTerrain;
		CGameObject* m_pPlayer;
		CGameObject* m_pMouse;
	};
}