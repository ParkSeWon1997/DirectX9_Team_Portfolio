#pragma once
#include "Component.h"

namespace Engine {

class ENGINE_DLL CTexture : public CComponent
{
protected:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	IDirect3DBaseTexture9* Get_Texture(int _iCnt) { return m_vecTexture[_iCnt]; }

public:
	HRESULT		Ready_Texture(TEXTUREID eType, const _tchar* pPath, const _uint& iCnt, const _uint& iNum);
	void		Set_Texture(const _uint& iIndex = 0);
	void		Set_Texture(class CShader* pShader, D3DXHANDLE hParameter, const _uint& iIndex = 0);
	void		Save_Texture_Path(TEXTUREID eType, const _tchar* pPath, const _uint& iCnt);

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;

public:
	virtual CComponent*	Clone();
	// iNum°ª Ãß°¡ 
	static CTexture*	Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType, const _tchar* pPath, const _uint& iCnt = 1, const _uint& iNum = 0);

public:
	virtual void		Free();

};

}