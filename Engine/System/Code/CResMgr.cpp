#include "..\..\Header\CResMgr.h"

#include "CPathMgr.h"

IMPLEMENT_SINGLETON(CResMgr)


CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{
	Free();
}

HRESULT CResMgr::Ready_Resource()
{
	wstring mResPath = CPathMgr::GetInstance()->GetResourcePath();
	ifstream	fin;

	mResPath += L"\\ResourceData\\TextureData.txt";
	fin.open(mResPath.c_str(), ios_base::in);

	fin.close();


	return S_OK;
}

void CResMgr::Save_TexturePath(wstring _fileName,wstring _objKey, TEXTUREID _textureTag, wstring _subPath, int num)
{
	wofstream fout;
	wstring mResPath = CPathMgr::GetInstance()->GetResourcePath();
	mResPath += L"\\ResourceData\\" + _fileName + L".txt";


	fout.open(mResPath, ios_base::out | ios_base::app);
	if (fout.is_open())
	{
		fout << L"[" << _objKey.c_str() << L"]";
		if (TEXTUREID::TEX_NORMAL == _textureTag)
		{
			fout << L"[" << _textureTag << L"]";
		}
		fout << L"[" << _subPath.c_str() << L"]";
		fout << L"[" << num << L"]" << endl;
	}
	fout.close();
}

void CResMgr::Free()
{

}
