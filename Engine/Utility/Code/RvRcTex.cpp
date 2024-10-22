#include "..\..\Header\RvRcTex.h"


CRvRcTex::CRvRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CRvRcTex::CRvRcTex(const CRvRcTex & rhs)
	:CVIBuffer(rhs)
{
}


CRvRcTex::~CRvRcTex()
{
}

HRESULT CRvRcTex::Ready_Buffer()
{

	m_dwFVF = FVF_TEX;
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32; 
	
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { -1.f, 1.f, 0.f };
	/*pVertex[0].vTexUV = { 0.f, 0.f };*/
	pVertex[0].vTexUV = { 1.f, 0.f };

	pVertex[1].vPosition = { 1.f, 1.f, 0.f };
	/*pVertex[1].vTexUV = { 1.f, 0.f };*/
	pVertex[1].vTexUV = { 0.f, 0.f };

	pVertex[2].vPosition = { 1.f, -1.f, 0.f };
	/*pVertex[2].vTexUV = { 1.f, 1.f };*/
	pVertex[2].vTexUV = { 0.f, 1.f };
		
	pVertex[3].vPosition = { -1.f, -1.f, 0.f };
	/*pVertex[3].vTexUV = { 0.f, 1.f };*/
	pVertex[3].vTexUV = { 1.f, 1.f };

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0; 
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CRvRcTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CRvRcTex * CRvRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRvRcTex *		pInstance = new CRvRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("RcTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CRvRcTex::Clone()
{
	return new CRvRcTex(*this);
}

void CRvRcTex::Free()
{
	CVIBuffer::Free();
}
