#pragma once
#include "VIBuffer.h"

namespace Engine {

	class ENGINE_DLL CCubeTex : public CVIBuffer
	{
	private:
		explicit CCubeTex()=delete;
		explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
		explicit CCubeTex(const CCubeTex& rhs);
		virtual ~CCubeTex();

	public:
		_vec3* Get_Pos() { return m_pPos; }

	public:
		virtual HRESULT		Ready_Buffer();
		virtual void		Render_Buffer();

	private:
		_vec3* m_pPos;

	public:
		static CCubeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
		virtual CComponent* Clone();

	private:
		virtual void	Free();
	};

}