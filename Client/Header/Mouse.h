#pragma once
#include "Export_Utility.h"
class CMouse :
    public CGameObject
{
private:
    explicit CMouse(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CMouse();

public:
	_vec3 Get_MousePos() { return m_vMousePos; }
	_vec3 Get_MouseRayPos() { return m_vRayPos; }
	_vec3 Get_MouseRayDir() { return m_vRayDir; }

	void Set_Install(_bool bInstall) { m_bInstall = bInstall; }
	void IsColl(_bool bColl) { m_bColl = bColl; }

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

private:
	HRESULT Add_Component();
	void Check_Coll();

public:
	static CMouse* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;

	_bool m_bColl;
	_bool m_bInstall;

	_vec3 m_vMousePos;
	_vec3 m_vRayPos;
	_vec3 m_vRayDir;

	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float m_fPreX, m_fPreY;

	eOBJECT_GROUPTYPE m_eGroupType;
	OBJSTAT m_eObjState;
};

