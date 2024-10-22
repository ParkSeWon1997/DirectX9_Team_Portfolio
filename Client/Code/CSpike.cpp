#include "stdafx.h"
#include "CSpike.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "Player.h"
CSpike::CSpike(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CTrap(pGraphicDev, _strObjName)
{
}

CSpike::CSpike(const CSpike& rhs)
	:CTrap(rhs)
{
}

CSpike::~CSpike()
{
}



_int CSpike::Update_GameObject(const _float& fTimeDelta)
{

	//플레이어와 충돌했을경우

	if (m_eCurState == eTRAP_STATE::HIDE)
	{
		if (PlayerHit(3.6f))
		{
			m_pAnimCom->ChangeAnimation(L"GROW");
			m_pAnimCom->SetLoopAnimator(false);
			m_eCurState = eTRAP_STATE::GROW;
			Engine::PlaySound_W(L"TreeGrow.mp3",CHANNELID::SOUND_EFFECT_CONTINUE_CH4, 0.2f);
		}
	}

	//나오는중이거나 나온상태일때
	if ((m_eCurState != eTRAP_STATE::DEAD))
	{
		if (m_eCurState == eTRAP_STATE::GROW || m_eCurState == eTRAP_STATE::IDLE)
		{
			if (m_pAnimCom->IsFinish(L"GROW"))
			{
				m_pAnimCom->ChangeAnimation(L"IDLE");
			}
			if (PlayerHit(0.7f))
			{
				dynamic_cast<CPlayer*>(scenemgr::Get_CurScene()->GetPlayerObject())->Set_Attack(1);
				m_pAnimCom->ChangeAnimation(L"HIDE");
				m_eCurState = eTRAP_STATE::DEAD;
			}

		}
	}




	return __super::Update_GameObject(fTimeDelta);
}

void CSpike::Render_GameObject()
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

CSpike* CSpike::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos)
{
	CSpike* pInstance = new CSpike(pGraphicDev, _strObjName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->GetTransForm()->Set_Pos(vPos);

	return pInstance;
}

CSpike* CSpike::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CSpike* pInstance = new CSpike(pGraphicDev, _strObjName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CSpike::Add_Component()
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


	m_pAnimCom->AddAnimation(L"HIDE", proto::Clone_ProtoAnim(L"TRAP_SPIKE_HIDE"));//숨기
	m_pAnimCom->AddAnimation(L"GROW", proto::Clone_ProtoAnim(L"TRAP_SPIKE_GROW"));//나타나기
	m_pAnimCom->AddAnimation(L"IDLE", proto::Clone_ProtoAnim(L"TRAP_SPIKE_IDLE"));//나타난상태지속


	m_pAnimCom->SetCurAnimation(L"HIDE");
	m_pAnimCom->SetCurAnimationFrame(L"HIDE", m_pAnimCom->GetAnimMaxFrame(L"HIDE"));
	m_eCurState = eTRAP_STATE::HIDE;


	m_pTransForm->Set_Scale(_vec3(2.f, 2.f, 2.f));

	return S_OK;
}
