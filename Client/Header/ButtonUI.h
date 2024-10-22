#pragma once
#include "Export_Utility.h"

class CButtonUI :
    public CGameObject
{
private:
	explicit CButtonUI(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec2 vSize, _bool bFood);
	explicit CButtonUI(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, CREATEINFO tCreateInfo, _vec2 vSize);
	explicit CButtonUI(const CButtonUI& rhs);
	virtual ~CButtonUI();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component();
	void Input_Mouse();

public:
	static CButtonUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 vPos, CREATEINFO tCreateInfo, _vec2 vSize = { 40.f, 15.f });
	static CButtonUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 vPos, _vec2 vSize = { 40.f, 15.f }, _bool bFood = true);

private:
	virtual void Free();

private:
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;

	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;

	CREATEINFO m_tCreateInfo;

	_bool m_bColl;
	_bool m_bFood;
};

