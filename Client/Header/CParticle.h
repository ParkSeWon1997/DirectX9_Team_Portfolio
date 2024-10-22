#pragma once
#include "GameObject.h"


namespace Engine
{
	class CTexture;
	class CPtcTex;
}


class CParticle :
	public CGameObject
{


protected:
	explicit CParticle() = delete;
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev) = delete;
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CParticle(const CParticle& rhs);
	virtual ~CParticle() override;


public:
	virtual HRESULT			Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta)override;
	virtual void			LateUpdate_GameObject();
	void Render_GameObject() override;
	DWORD	FtoDw(float f) { return *((DWORD*)&f); }


protected:
	virtual void	preRender();
	virtual void postRender();
	virtual void AddParticle()
	{
		Attribute attribute;
		resetParticle(&attribute);
		m_ParticleList.push_back(attribute);
	}

	void Reset();
	virtual void resetParticle(Attribute* attribute) {}


	float	GetRandomFloat(_float fLowBound, _float fHighBound);
	void	GetRendomVector(_vec3* out, _vec3* min, _vec3* max)
	{
		out->x = GetRandomFloat(min->x, max->x);
		out->y = GetRandomFloat(min->y, max->y);
		out->z = GetRandomFloat(min->z, max->z);
	}



private:
	virtual HRESULT			Add_Component();

	virtual void Free()override;
protected:
	CTexture* m_pTextureCom;
	_float	m_fSize;	//파티클 크기
	_float	m_fEmitRate;// 초당파티클 수
	_int					m_iMaxParticle;

	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	_ulong							m_dwFVF;
	_ulong							m_dwVtxSize;
	DWORD							m_vbSize;
	DWORD							m_vbOffset;
	DWORD							m_vbBatchSize;
	std::list<Attribute>			m_ParticleList;

	_vec3	m_vMinBox;
	_vec3	m_vMaxBox;

};

