#pragma once

#include "Engine_Define.h"
#include "Base.h"

namespace Engine
{

	class ENGINE_DLL CPathMgr : public CBase
	{
		DECLARE_SINGLETON(CPathMgr)

	private:
		explicit CPathMgr();
		virtual ~CPathMgr();

	public:
		HRESULT			Ready_Path();
		wstring GetRelativePath(const wchar_t* filePath);
		const _tchar* GetResourcePath() { return m_strBasePath; }
	private:

	private:
		virtual void		Free();

	private:
		_tchar		m_strBasePath[MAX_PATH];

	};

}