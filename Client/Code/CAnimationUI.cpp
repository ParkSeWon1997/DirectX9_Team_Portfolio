#include "stdafx.h"
#include "CAnimationUI.h"
#include "Export_Utility.h"


CAnimationUI::CAnimationUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pReverseCom(nullptr)
	, m_pAnimCom(nullptr)
{
}

CAnimationUI::CAnimationUI(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName)
	:CGameObject(pGraphicDev, _strObjName)
	, m_pBufferCom(nullptr)
	, m_pReverseCom(nullptr)
	, m_pAnimCom(nullptr)
{
}

CAnimationUI::~CAnimationUI()
{
}

HRESULT CAnimationUI::Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CAnimationUI::Render_GameObject()
{
	scenemgr::Get_CurScene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

	m_pAnimCom->SetAnimTexture();
	m_pBufferCom->Render_Buffer();


	scenemgr::Get_CurScene()->EndOrtho();
}

void CAnimationUI::Free()
{
	__super::Free();
}


