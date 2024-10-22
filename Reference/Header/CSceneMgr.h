#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Scene.h"

namespace Engine
{
	class ENGINE_DLL CSceneMgr :public CBase
	{
		DECLARE_SINGLETON(CSceneMgr)

	public:
		HRESULT			Change_Scene(CScene* pScene);
		CScene*			Get_CurScene() { return m_pCurScene; }
		_int			Update_Scene(const _float& fTimeDelta);
		void			LateUpdate_Scene();
		void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);


	public:
		virtual void Free();


	private:
		explicit CSceneMgr();
		virtual ~CSceneMgr();
	private:
		CScene* m_pCurScene;

	};

}

