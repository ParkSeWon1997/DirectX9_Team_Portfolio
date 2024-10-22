#pragma once
#include "VIBuffer.h"

namespace Engine
{
	class ENGINE_DLL CTriCol : public CVIBuffer
	{
	private:
		explicit CTriCol()=delete;
		explicit CTriCol(LPDIRECT3DDEVICE9 pGraphicDev);
		explicit CTriCol(const CTriCol& rhs);
		virtual ~CTriCol();

	public:
		virtual HRESULT		Ready_Buffer();
		virtual void		Render_Buffer();

	public:
		static CTriCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
		virtual CComponent* Clone();

	private:
		virtual void	Free();
	};

}