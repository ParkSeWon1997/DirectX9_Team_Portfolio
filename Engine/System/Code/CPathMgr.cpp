#include "..\..\Header\CPathMgr.h"

IMPLEMENT_SINGLETON(CPathMgr)


CPathMgr::CPathMgr()
{
	memset(m_strBasePath, 0, sizeof(m_strBasePath));
}


CPathMgr::~CPathMgr()
{
	Free();
}

HRESULT CPathMgr::Ready_Path()
{
	GetCurrentDirectory(255, m_strBasePath);

	size_t iLen = wcslen(m_strBasePath);

	//상위 폴더로
	for (size_t i = iLen - 1; 0 <= i; --i)
	{
		if ('\\' == m_strBasePath[i])
		{
			m_strBasePath[i] = '\0';
			break;
		}
	}
	wcscat_s(m_strBasePath, 255, L"\\bin\\Resource");


	return S_OK;
}

wstring CPathMgr::GetRelativePath(const wchar_t* filePath)
{
	wstring strFilePath = filePath;

	size_t absoluteLength = wcslen(m_strBasePath);
	size_t fullLength = strFilePath.length();

	wstring strRelativePath = strFilePath.substr(absoluteLength, fullLength - absoluteLength);

	return strRelativePath;
}



void CPathMgr::Free()
{

}
