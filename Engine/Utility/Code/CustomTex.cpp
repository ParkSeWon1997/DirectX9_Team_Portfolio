#include "..\..\Header\CustomTex.h"


CustomizeTex::CustomizeTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CustomizeTex::CustomizeTex(const CustomizeTex & rhs)
	:CVIBuffer(rhs)
{
}


CustomizeTex::~CustomizeTex()
{
}

HRESULT CustomizeTex::Ready_Buffer(const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv)
{
	_ulong dwCntX = _dwCntX;
	_ulong dwCntZ = _dwCntZ;
	_ulong dwVtxItv = _dwVtxItv;
	
	m_dwFVF = FVF_TEX;
	m_dwTriCnt = (dwCntX-1)*(dwCntZ-1)*2;
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32; 
	
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_ulong	dwIndex(0);

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPosition.x = static_cast<float>(j) * dwVtxItv;
			pVertex[dwIndex].vPosition.y = static_cast<float>(i) * dwVtxItv;
			pVertex[dwIndex].vPosition.z = 0.0f;
					
			//pVertex[dwIndex].vNormal = D3DXVECTOR3(0.0f, 0.0f, 0.f); // 간단히 z축을 향하도록 설정

			// 좌우 반전하지 않은 uv 좌표 설정
			/*00, 10, 11, 01*/
			pVertex[dwIndex].vTexUV.x = static_cast<float>(j) / (dwCntX - 1);
			pVertex[dwIndex].vTexUV.y = static_cast<float>(i) / (dwCntZ - 1);
			// 좌우 반전
		/*	pVertex[i * dwCntX + j].vTexUV.x = 1.0f - static_cast<float>(j) / (dwCntX - 1);
			pVertex[i * dwCntX + j].vTexUV.y = static_cast<float>(i) / (dwCntZ - 1);*/
		}
	}
	
	m_pVB->Unlock();

	INDEX32* indices;
	m_pIB->Lock(0, 0, (void**)&indices, 0);

	// 인덱스 설정
	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			indices[(i * (dwCntX - 1) + j) * 2]._0 = i * dwCntX + j;
			indices[(i * (dwCntX - 1) + j) * 2]._1 = i * dwCntX + j + 1;
			indices[(i * (dwCntX - 1) + j) * 2]._2 = (i + 1) * dwCntX + j;

			indices[(i * (dwCntX - 1) + j) * 2 + 1]._0 = (i + 1) * dwCntX + j;
			indices[(i * (dwCntX - 1) + j) * 2 + 1]._1 = i * dwCntX + j + 1;
			indices[(i * (dwCntX - 1) + j) * 2 + 1]._2 = (i + 1) * dwCntX + j + 1;
		}
	}
	m_pIB->Unlock();

	return S_OK;
}

void CustomizeTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CustomizeTex * CustomizeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	CustomizeTex *		pInstance = new CustomizeTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pInstance);
		MSG_BOX("RcTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CustomizeTex::Clone()
{
	return new CustomizeTex(*this);
}

void CustomizeTex::Free()
{

	CVIBuffer::Free();
}
