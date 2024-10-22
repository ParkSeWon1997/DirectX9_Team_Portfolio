#pragma once
#include "GameObject.h"


namespace Engine
{
	class CRcTex;
	class CTexture;
}



class CMainUI :
    public CGameObject
{

protected:
	explicit CMainUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CMainUI(const CMainUI& rhs);
	virtual ~CMainUI();

public:
	virtual HRESULT Ready_GameObject(_vec3 _pos, _vec3 _size, float _Angle = 0);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;


	//자기 자신만 충돌 체크가능
	virtual BOOL UI_Collision();
	virtual BOOL IsItemOn() { return m_bItemChek; }
	virtual void SetItemOn(BOOL _ItemPut) { m_bItemChek = _ItemPut; }




	void Change_Scene();


	_float GetX() { return m_fX; }
	_float GetY() { return m_fY; }
	_float GetSizeX() { return m_fSizeX; }
	_float GetSizeY() { return m_fSizeY; }

	virtual HRESULT			Add_Component();
protected:
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;
	UI_STATE		m_eUIState;
protected:
	_float m_fX;
	_float m_fY;
	_float m_fSizeX;
	_float m_fSizeY;
	_float m_fAngle;

	_bool m_bColl;
	_bool m_bSoundStart = false;
	POINT m_MousePoint;
	BOOL m_bItemChek;

public:
	static CMainUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle = 0.f);

public:
	virtual void Free() override;
};





