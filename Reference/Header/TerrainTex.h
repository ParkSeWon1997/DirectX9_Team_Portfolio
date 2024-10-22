#pragma once

#include "VIBuffer.h"
#include "Engine_Define.h"

namespace Engine {

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex()=delete;
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();

public:
	const _vec3*		Get_VtxPos()const { return m_pPos; }
	vector<_int> Get_VecPos() { return m_vecInaccessiblePos; }
	
	void Set_VecPos(vector<_int> _vecPos) { m_vecInaccessiblePos = _vecPos; }

public:
	HRESULT				Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual void		Render_Buffer();

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

	_vec3*				m_pPos;
	vector<_int> m_vecInaccessiblePos; // 접근 불가능한 index

public:
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const _ulong& dwCntX = VTXCNTX, 
									const _ulong& dwCntZ = VTXCNTZ,
									const _ulong& dwVtxItv = VTXITV);

	virtual CComponent*	Clone();

private:
	virtual void	Free();
};

}