#include "MyFont.h"

CMyFont::CMyFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pFont(nullptr)
	, m_pSprite(nullptr)
{
	m_pGraphicDev->AddRef();
}

CMyFont::~CMyFont()
{
	
}

HRESULT CMyFont::Ready_Font(const _tchar * pFontType, const _uint & iWidth, const _uint & iHeight, const _uint & iWeight)
{
	D3DXFONT_DESC				Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));
	
	Font_Desc.CharSet = HANGEUL_CHARSET;
	lstrcpy(Font_Desc.FaceName, pFontType);
	Font_Desc.Width  = iWidth;
	Font_Desc.Height = iHeight;
	Font_Desc.Weight = iWeight;

	Font_Desc.OutputPrecision = OUT_RASTER_PRECIS;
	Font_Desc.Quality= PROOF_QUALITY;

	//Font_Desc.
	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &Font_Desc, &m_pFont)))
	{
		MSG_BOX("Font Created Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		MSG_BOX("Sprite Created Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CMyFont::Render_Font(const _tchar * pString, const _vec2 * pPos, D3DXCOLOR Color)
{
	RECT	rc{ long(pPos->x), long(pPos->y) };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	
	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);
	
	m_pSprite->End();
}

CMyFont * CMyFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFontType, const _uint & iWidth, const _uint & iHeight, const _uint & iWeight)
{
	CMyFont *		pInstance = new CMyFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
	{
		MSG_BOX("Font Created Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMyFont::Free()
{
	Safe_Release(m_pSprite);
	Safe_Release(m_pFont);
	Safe_Release(m_pGraphicDev);
}
