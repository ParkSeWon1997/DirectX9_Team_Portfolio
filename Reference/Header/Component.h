#pragma once

#include "Engine_Define.h"
#include "Base.h"

namespace Engine
{
	class CGameObject;

	class ENGINE_DLL CComponent : public CBase
	{
	public:
		CGameObject*		GetOwner() { return m_pOwner; }
		void				SetOwner(CGameObject* _pOwner) { m_pOwner = _pOwner; }
		virtual _int		Update_Component(const _float& fTimeDelta) { return 0; }
		virtual void		LateUpdate_Component() {}


		virtual CComponent* Clone()PURE;
		virtual void		Free();

	protected:
		explicit CComponent() = delete;
		explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
		explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pOwner);
		explicit CComponent(const CComponent& rhs);
		virtual ~CComponent();

	protected:
		LPDIRECT3DDEVICE9			m_pGraphicDev;
		CGameObject*				m_pOwner; //자신을 보유하고있는 GameObject포인터
		_bool						m_bClone;
	};
}


