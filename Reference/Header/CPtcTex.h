#pragma once
#include "Component.h"

namespace Engine {

	class ENGINE_DLL CPtcTex : public CComponent
	{
	public:
		_bool	IsEmpty() { return m_ParticleList.empty(); }
		virtual HRESULT		Ready_Buffer();
		virtual void		Render_Buffer();
		float	GetRandomFloat(_float fLowBound, _float fHighBound);
		void	GetRendomVector(_vec3* out, _vec3* min, _vec3* max)
		{
			out->x = GetRandomFloat(min->x, max->x);
			out->y = GetRandomFloat(min->y, max->y);
			out->z = GetRandomFloat(min->z, max->z);
		}
		DWORD	GetVbSize() { return m_vbSize; }
		static CPtcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
		virtual CComponent* Clone();
		void CPtcTex::AddParticle();
		void resetParticle(Attribute* attribute);
		void Update(const _float& timeDelta);

		void SetSomeThing(DWORD vbSize, DWORD Offset, DWORD BatchSize)
		{
			m_vbSize = vbSize;
			m_vbOffset = Offset;
			m_vbBatchSize = BatchSize;
		}



	private:
		virtual void	Free();

	private:
		explicit CPtcTex() = delete;
		explicit CPtcTex(LPDIRECT3DDEVICE9 pGraphicDev);
		explicit CPtcTex(const CPtcTex& rhs);
		virtual ~CPtcTex();

	protected:
		LPDIRECT3DVERTEXBUFFER9			m_pVB;
		_ulong							m_dwFVF;
		_ulong							m_dwVtxCnt;
		_ulong							m_dwVtxSize;
		DWORD							m_vbSize ;
		DWORD							m_vbOffset;
		DWORD							m_vbBatchSize;
		std::list<Attribute>			m_ParticleList;

		_vec3	m_vMinBox;
		_vec3	m_vMaxBox;

	};

}
