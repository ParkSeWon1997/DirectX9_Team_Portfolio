#pragma once
#include "Export_Utility.h"
class CToolObj :
    public CGameObject
{
private:
	explicit CToolObj(LPDIRECT3DDEVICE9 pGraphicDev, wstring pName);
	virtual ~CToolObj();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();
	void BillBoard();
	
	void Mouse_Input();
	void Height_Obj();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculator;

	wstring m_pName;



public:
	static CToolObj* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring pName);

private:
	virtual void Free();
};

