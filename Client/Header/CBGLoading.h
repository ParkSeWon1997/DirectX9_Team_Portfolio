#pragma once
#include "CAnimationUI.h"
class CBGLoading :
    public CAnimationUI
{
protected:
	explicit CBGLoading() = delete;
	explicit CBGLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBGLoading(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName);
	virtual ~CBGLoading();
	

public:
	static CBGLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName, _vec3 vPos,_vec3 vScale);


	// CAnimationUI을(를) 통해 상속됨
	_int Update_GameObject(const _float& fTimeDelta) override;

	virtual void CBGLoading::Render_GameObject()override;


	HRESULT Add_Component() override;
};

