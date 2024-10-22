#pragma once
#include "Engine_Define.h"
#include "Base.h"

namespace Engine
{
	class CGameObject;

	class ENGINE_DLL CLayer : public CBase
	{
	public:
		static		CLayer* Create();

	public:
		const vector<CGameObject*>& GetGroupObject(eOBJECT_GROUPTYPE _eType) { return m_vecObject[(int)_eType]; } // Tool »ç¿ë
		HRESULT		AddGameObject(eOBJECT_GROUPTYPE _eObjType, CGameObject* pGameObject);

	public:
		HRESULT					ReadyLayer();
		_int					UpdateLayer(const _float& fTimeDelta);
		void					LateUpdateLayer();
		void					DeleteEvent();

		virtual		void		Free();
	private:
		explicit CLayer();
		virtual ~CLayer();
	private:
		vector<CGameObject*>		m_vecObject[(int)eOBJECT_GROUPTYPE::END];
	};

}