#include "..\..\Header\ProtoMgr.h"

IMPLEMENT_SINGLETON(CProtoMgr)

CProtoMgr::CProtoMgr()
{
}

CProtoMgr::~CProtoMgr()
{
	Free();
}

HRESULT CProtoMgr::Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent)
{
	CComponent* pInstance = Find_Prototype(pProtoTag);

	if (nullptr != pInstance)
	{
		//return E_File;
		Safe_Release(pComponent);
		return S_OK;
	}

	m_mapProto.insert({ pProtoTag, pComponent });

	return S_OK;
}

CComponent* CProtoMgr::Clone_Proto(const _tchar* pProtoTag)
{
	CComponent* pComponent = Find_Prototype(pProtoTag);

	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent->Clone();
}

HRESULT CProtoMgr::Ready_ProtoAnim(const _tchar* pProtoAnimTag, CAnimation* pAnim)
{

	CAnimation* pInstance = Find_ProtoAnim(pProtoAnimTag);

	if (nullptr != pInstance)
	{
		Safe_Release(pAnim);
		return S_OK;
	}

	m_mapProtoAnim.insert({ pProtoAnimTag, pAnim });

	return S_OK;
}

CAnimation* CProtoMgr::Clone_ProtoAnim(const _tchar* pProtoAnimTag)
{
	CAnimation* pComponent = Find_ProtoAnim(pProtoAnimTag);

	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent->Clone();
}

CComponent* CProtoMgr::Find_Prototype(const _tchar* pProtoTag)
{
	auto	iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapProto.end())
		return nullptr;

	return iter->second;
}

CAnimation* CProtoMgr::Find_ProtoAnim(const _tchar* pProtoAnimTag)
{
	auto	iter = find_if(m_mapProtoAnim.begin(), m_mapProtoAnim.end(), CTag_Finder(pProtoAnimTag));

	if (iter == m_mapProtoAnim.end())
		return nullptr;

	return iter->second;
}

void CProtoMgr::Free()
{
	for_each(m_mapProto.begin(), m_mapProto.end(), CDeleteMap());
	for_each(m_mapProtoAnim.begin(), m_mapProtoAnim.end(), CDeleteMap());
	m_mapProto.clear();
	m_mapProtoAnim.clear();
}
