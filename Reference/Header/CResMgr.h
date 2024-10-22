#pragma once

#include "Engine_Define.h"
#include "Base.h"

namespace Engine 
{

	class ENGINE_DLL CResMgr : public CBase
	{
		DECLARE_SINGLETON(CResMgr)
	public:
		HRESULT Ready_Resource();
		void Save_TexturePath(wstring _fileName,wstring _objKey, TEXTUREID _textureTag, wstring  _subPath, int num);
	private:
		explicit CResMgr();
		virtual ~CResMgr();
	private:
		virtual void		Free();
	};

}