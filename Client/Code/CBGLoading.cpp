#include "stdafx.h"
#include "CBGLoading.h"



CBGLoading::CBGLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	:CAnimationUI(pGraphicDev)
{
}

CBGLoading::CBGLoading(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName)
	:CAnimationUI(pGraphicDev, _strObjName)
{
}

CBGLoading::~CBGLoading()
{
}

CBGLoading* CBGLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName, _vec3 vPos, _vec3 vScale)
{
	CBGLoading* pInstance = new CBGLoading(pGraphicDev, _strObjName);

	if (FAILED(pInstance->Ready_GameObject(pGraphicDev)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->GetTransForm()->Set_Pos(vPos);
	pInstance->GetTransForm()->Set_Scale(vScale);
	pInstance->m_fX = vPos.x;
	pInstance->m_fY = vPos.y;

	return pInstance;
}
_int CBGLoading::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	m_pTransForm->Get_WorldMatrix()->_41 = m_fX - (WINCX >> 1);
	m_pTransForm->Get_WorldMatrix()->_42 = -m_fY + (WINCY >> 1);

	renderer::Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CBGLoading::Render_GameObject()
{
	scenemgr::Get_CurScene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

	m_pAnimCom->SetAnimTexture();
	m_pBufferCom->Render_Buffer();


	scenemgr::Get_CurScene()->EndOrtho();
}

HRESULT CBGLoading::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pAnimCom = dynamic_cast<CAnimator*>(proto::Clone_Proto(L"Proto_Anim"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Anim", pComponent });

	m_pAnimCom->AddAnimation(m_strObjName.c_str(), proto::Clone_ProtoAnim(m_strObjName.c_str()));
	m_pAnimCom->SetCurAnimation(m_strObjName.c_str());
	m_pAnimCom->SetLoopAnimator(true);


	return S_OK;
}
