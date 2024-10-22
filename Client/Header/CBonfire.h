
#pragma once
#include "ResObject.h"

class CFire;


class CBonfire :
	public CResObject
{
	enum BONFIRE_STATE { BONFIRE_IDLE,BONFIRE_DROP,BONFIRE_END };

private:
	explicit CBonfire(LPDIRECT3DDEVICE9 pGraphicDev , _bool bInstall);
	explicit CBonfire(const CBonfire& rhs);
	virtual ~CBonfire();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;

public:
	void Install_Obj();
	//처음에는 항상 IDLE임 마우스를 드래그해서 놓으면 해당 함수를 호출	
	void Set_DropBonfire() { m_bIsDrop = true; }

	//1~4 만 적용가능// 장작을 추가하는 함수
	void AddFIre(int _Value);

private:
	virtual HRESULT			Add_Component() override;
	virtual void			Check_FrameState() override;
	virtual void			Ready_Stat() override {};
	virtual void			Change_Frame_Event() override {};



public:
	static CResObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall = false);

private:
	virtual void Free();

private:
	Engine::CCalculator* m_pCalculatorCom;
	CTexture* m_pBonfireTexCom[BONFIRE_END];

	BONFIRE_STATE m_eBonfireCurState;
	BONFIRE_STATE m_eBonfirerPrevState;


	_float m_fFireWoodCount = 0.f;
	CFire* m_pFire = nullptr;

	BOOL m_bIsDrop = false;
	BOOL m_bIsDropEnd = false;

	_bool m_bInstall = false;

};

