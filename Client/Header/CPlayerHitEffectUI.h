#pragma once
#include "CAnimationUI.h"

class CPlayerHitEffectUI :
	public CAnimationUI
{
protected:
	explicit CPlayerHitEffectUI() = delete;
	explicit CPlayerHitEffectUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerHitEffectUI(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName);
	virtual ~CPlayerHitEffectUI();


public:
	static CPlayerHitEffectUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName, _vec3 vPos, _vec3 vScale);


	// CAnimationUI��(��) ���� ��ӵ�
	_int Update_GameObject(const _float& fTimeDelta) override;

	virtual void CPlayerHitEffectUI::Render_GameObject()override;


	HRESULT Add_Component() override;
};

