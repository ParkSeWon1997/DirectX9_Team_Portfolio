#pragma once

#include "Export_Utility.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}

class CUI
	: public CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI(const CUI& rhs);
	virtual ~CUI();

protected:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 PURE;

private:
	virtual HRESULT			Add_Component() PURE;

public:
	_float Get_fX() { return m_fX; }
	_float Get_fY() { return m_fY; }
	_float Get_fSizeX() { return m_fSizeX; }
	_float Get_fSizeY() { return m_fSizeY; }


	void Set_fX(_float _x) { m_fX = _x; }
	void Set_fY(_float _y) { m_fY = _y; }
	void Set_fSizeX(_float _x) { m_fSizeX = _x; }
	void Set_fSizeY(_float _y) { m_fSizeY = _y; }

protected:
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;

protected:
	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;

protected:
	virtual void Free() override;
};

