#include "stdafx.h"
#include "CToothTrap.h"
#include "Export_Utility.h"
#include "Player.h"

CToothTrap::CToothTrap(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CTrap(pGraphicDev, _strObjName)
{
}

CToothTrap::CToothTrap(const CToothTrap& rhs)
	:CTrap(rhs)
{
}

CToothTrap::~CToothTrap()
{
}



_int CToothTrap::Update_GameObject(const _float& fTimeDelta)
{


	if (m_eCurState == eTRAP_STATE::HIDE)
	{
		//하이드중 대기
		if (PlayerHit(0.7f))//타격범위에 맞았음
		{
			dynamic_cast<CPlayer*>(scenemgr::Get_CurScene()->GetPlayerObject())->Set_Attack(1);
			m_pAnimCom->ChangeAnimation(L"GROW");
			m_pAnimCom->SetLoopAnimator(false);
			m_pTransForm->Set_Scale(_vec3(0.9f, 0.9f, 0.9f));
			m_eCurState = eTRAP_STATE::GROW;
		}
	}

	if (m_eCurState == eTRAP_STATE::GROW)
	{
		if (m_pAnimCom->IsFinish(L"GROW"))//글로우가끝났으면
		{
			m_pAnimCom->ChangeAnimation(L"IDLE");
			m_pAnimCom->SetLoopAnimator(true);
			m_eCurState = eTRAP_STATE::DEAD;
		}
	}



	return __super::Update_GameObject(fTimeDelta);
}

void CToothTrap::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pAnimCom->SetAnimTexture();
	FAILED_CHECK_RETURN(SetUp_Material(), );

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

CToothTrap* CToothTrap::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos)
{
	CToothTrap* pInstance = new CToothTrap(pGraphicDev, _strObjName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->GetTransForm()->Set_Pos(vPos);

	return pInstance;
}

CToothTrap* CToothTrap::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CToothTrap* pInstance = new CToothTrap(pGraphicDev, _strObjName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CToothTrap::Add_Component()
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


	m_pAnimCom->AddAnimation(L"HIDE", proto::Clone_ProtoAnim(L"TRAP_TOOTH_HIDE"));//숨어서대기

	m_pAnimCom->AddAnimation(L"GROW", proto::Clone_ProtoAnim(L"TRAP_TOOTH_GROW"));//타격하면서 나타나기

	m_pAnimCom->AddAnimation(L"IDLE", proto::Clone_ProtoAnim(L"TRAP_TOOTH_IDLE"));//타격후 나타난상태지속


	m_pAnimCom->SetCurAnimation(L"HIDE");
	m_pAnimCom->SetCurAnimationFrame(L"HIDE", m_pAnimCom->GetAnimMaxFrame(L"HIDE"));
	m_eCurState = eTRAP_STATE::HIDE;

	m_pTransForm->Set_Scale(_vec3(0.4f, 0.f, 0.5f));

	return S_OK;
}
