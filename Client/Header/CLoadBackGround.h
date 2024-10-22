#pragma once
#include "CAnimationUI.h"
class CLoadBackGround :
    public CAnimationUI
{
protected:
	explicit CLoadBackGround() = delete;
	explicit CLoadBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLoadBackGround(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName);
	virtual ~CLoadBackGround();
	

public:
	static CLoadBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName, _vec3 vPos,_vec3 vScale);


	// CAnimationUI을(를) 통해 상속됨
	_int Update_GameObject(const _float& fTimeDelta) override;

	virtual void CLoadBackGround::Render_GameObject()override;


	HRESULT Add_Component() override;
};

