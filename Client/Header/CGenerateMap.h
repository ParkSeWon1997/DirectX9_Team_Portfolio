#pragma once
#include "CAnimationUI.h"
class CGenerateMap :
    public CAnimationUI
{
protected:
	explicit CGenerateMap() = delete;
	explicit CGenerateMap(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGenerateMap(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName);
	virtual ~CGenerateMap();

	

public:
	static CGenerateMap* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName, _vec3 vPos,_vec3 vScale);


	// CAnimationUI을(를) 통해 상속됨
	_int Update_GameObject(const _float& fTimeDelta) override;
	HRESULT Add_Component() override;

};

