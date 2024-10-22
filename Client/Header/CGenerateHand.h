#pragma once
#include "CAnimationUI.h"
class CGenerateHand :
    public CAnimationUI
{
protected:
	explicit CGenerateHand() = delete;
	explicit CGenerateHand(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGenerateHand(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName);
	virtual ~CGenerateHand();

	

public:
	static CGenerateHand* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName, _vec3 vPos,_vec3 vScale,_bool bFlip);


	// CAnimationUI을(를) 통해 상속됨
	_int Update_GameObject(const _float& fTimeDelta) override;

	virtual void CGenerateHand::Render_GameObject()override;


	HRESULT Add_Component() override;

private:
	_bool m_bFlip;
};

