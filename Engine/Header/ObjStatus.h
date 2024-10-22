#pragma once
#include "Component.h"

namespace Engine {

	class ENGINE_DLL CObjStatus :
		public CComponent
	{
	protected:
		explicit CObjStatus()=delete;
		explicit CObjStatus(LPDIRECT3DDEVICE9 pGraphicDev);
		explicit CObjStatus(const CObjStatus& rhs);
		virtual ~CObjStatus();
		// CComponent을(를) 통해 상속됨

	public:
		OBJSTAT* Get_Stat() { return m_Stat; }
		_float Get_HP() { return m_Stat->fHP; }
		void Minus_HP(_float _value) { m_Stat->fHP -= _value; }
		void Plus_HP(_float _value) { m_Stat->fHP += _value; }

	protected:
		OBJSTAT* m_Stat;

	public:

		CComponent* Clone() override;
		virtual void Free();
	};

}