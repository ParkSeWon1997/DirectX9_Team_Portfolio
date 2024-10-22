#pragma once
#include "VIBuffer.h"

namespace Engine {

	class ENGINE_DLL CRcCol : public CVIBuffer
	{
	private:
		explicit CRcCol()=delete;
		explicit CRcCol(LPDIRECT3DDEVICE9 pGraphicDev);
		explicit CRcCol(const CRcCol& rhs);
		virtual ~CRcCol();

	public:
		virtual HRESULT		Ready_Buffer();
		virtual void		Render_Buffer();

	public:
		static CRcCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
		virtual CComponent* Clone();

	private:
		virtual void	Free();
	};

}