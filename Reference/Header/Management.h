#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Scene.h"

namespace Engine {

class ENGINE_DLL CManagement :public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	CComponent* Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag);
	CLayer* Get_Layer(const _tchar* pLayerTag);

public:
	HRESULT			Set_Scene(CScene* pScene);
	CScene*			Get_Scene() { return m_pScene; }

	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene();
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	CScene* m_pScene;

public:
	virtual void Free();
};

END