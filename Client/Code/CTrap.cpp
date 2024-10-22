#include "stdafx.h"
#include "CTrap.h"

#include "Export_Utility.h"

CTrap::CTrap(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CGameObject(pGraphicDev, _strObjName)
	, m_pBufferCom(nullptr)
	, m_pAnimCom(nullptr)
	, m_bHit(false)
	, m_eCurState(eTRAP_STATE::NONE)
{
}

CTrap::CTrap(const CTrap& rhs)
	:CGameObject(rhs)
	, m_pBufferCom(nullptr)
	, m_pAnimCom(nullptr)
	, m_bHit(false)

{
}

CTrap::~CTrap()
{
}

HRESULT CTrap::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CTrap::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

}

_bool CTrap::PlayerHit(float _fDistance)
{
	_vec3 vTargetPos, vPos, vDir;
	scenemgr::Get_CurScene()->GetPlayerObject()->GetTransForm()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	vTargetPos.y = 0.f;
	vPos.y = 0.f;

	if (D3DXVec3Length(&(vTargetPos - vPos)) < _fDistance)
		return true;
	else
		return false;
}

void CTrap::Free()
{
	__super::Free();
}

