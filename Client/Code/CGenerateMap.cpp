#include "stdafx.h"
#include "CGenerateMap.h"



CGenerateMap::CGenerateMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:CAnimationUI(pGraphicDev)
{
}

CGenerateMap::CGenerateMap(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName)
	:CAnimationUI(pGraphicDev, _strObjName)
{
}

CGenerateMap::~CGenerateMap()
{
}

CGenerateMap* CGenerateMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName, _vec3 vPos, _vec3 vScale)
{
	CGenerateMap* pInstance = new CGenerateMap(pGraphicDev, _strObjName);

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
_int CGenerateMap::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	m_pTransForm->Get_WorldMatrix()->_41 = m_fX - (WINCX >> 1);
	m_pTransForm->Get_WorldMatrix()->_42 = -m_fY + (WINCY >> 1);

	renderer::Add_RenderGroup(RENDER_UI, this);
	return 0;
}

HRESULT CGenerateMap::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pAnimCom = dynamic_cast<CAnimator*>(proto::Clone_Proto(L"Proto_Anim"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Anim", pComponent });

	m_pAnimCom->AddAnimation(L"GENERATE", proto::Clone_ProtoAnim(L"GENERATE_WORLD"));


	m_pAnimCom->SetCurAnimation(L"GENERATE");
	m_pAnimCom->SetLoopAnimator(true);

	return S_OK;
}
