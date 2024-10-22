#include "stdafx.h"
#include "CGeyserTrap.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Export_System.h"

CGeyserTrap::CGeyserTrap(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CTrap(pGraphicDev, _strObjName)
	, m_fAccTime(0.f)
	, m_fOpenTime(2.f)
	, m_fLoopTime(3.8f)
	, m_fPlayerHit(0.f)
{
}

CGeyserTrap::CGeyserTrap(const CGeyserTrap& rhs)
	:CTrap(rhs)
	, m_fAccTime(0.f)
	, m_fOpenTime(0.f)
	, m_fLoopTime(0.f)
{
}

CGeyserTrap::~CGeyserTrap()
{
}



_int CGeyserTrap::Update_GameObject(const _float& fTimeDelta)
{

	if (m_eCurState == eTRAP_STATE::IDLE)
	{

		m_pAnimCom->ChangeAnimation(L"OPEN");

		m_pAnimCom->SetLoopAnimator(false);
		m_eCurState = eTRAP_STATE::OPEN;
	}




	if (m_eCurState == eTRAP_STATE::OPEN && m_pAnimCom->GetAnimFrame(L"OPEN") == 20)
	{
		if (PlayerHit(20.f))
		{
			int randomvalue = rand() % 3;


			switch (randomvalue)
			{
			case 0:
				Engine::PlaySound_W(L"GeyserOpen_1.mp3", SOUND_EFFECT_CONTINUE_CH1, 0.05f);
				break;
			case 1:
				Engine::PlaySound_W(L"GeyserOpen_2.mp3", SOUND_EFFECT_CONTINUE_CH2, 0.05f);
				break;
			case 2:
				Engine::PlaySound_W(L"GeyserOpen_3.mp3", SOUND_EFFECT_CONTINUE_CH3, 0.05f);
				break;
			}
		}

	}


	if (m_eCurState == eTRAP_STATE::OPEN && m_pAnimCom->IsFinish(L"OPEN"))
	{
		m_eCurState = eTRAP_STATE::LOOP;
		m_pAnimCom->ChangeAnimation(L"LOOP");

		if (PlayerHit(20.f))
		{
			int randomvalue = rand() % 3;
			switch (randomvalue)
			{
			case 0:
				Engine::PlaySound_W(L"FlameGeyser_1.mp3", SOUND_EFFECT_CONTINUE_CH3, 0.05f);
				break;
			case 1:
				Engine::PlaySound_W(L"FlameGeyser_2.mp3", SOUND_EFFECT_CONTINUE_CH1, 0.05f);
				break;
			case 2:
				Engine::PlaySound_W(L"FlameGeyser_3.mp3", SOUND_EFFECT_CONTINUE_CH2, 0.05f);
				break;
			}

		}
		m_pAnimCom->SetLoopAnimator(true);
	}

	if (m_eCurState == eTRAP_STATE::LOOP)
	{
		m_fAccTime += fTimeDelta;

		if (PlayerHit(0.7f))
		{
			dynamic_cast<CPlayer*>(scenemgr::Get_CurScene()->GetPlayerObject())->Set_Attack(1);
		}

		if (m_fLoopTime <= m_fAccTime)
		{
			m_fAccTime = 0.f;
			if (PlayerHit(40.f))
				Engine::PlaySound_W(L"GeyserOut.mp3", CHANNELID::SOUND_EFFECT_CONTINUE_CH3, 0.1f);
			m_pAnimCom->ChangeAnimation(L"CLOSE");
			m_pAnimCom->SetLoopAnimator(false);
			m_eCurState = eTRAP_STATE::DEAD;
		}
	}

	if (m_eCurState == eTRAP_STATE::DEAD && m_pAnimCom->IsFinish(L"CLOSE"))
	{
		m_pAnimCom->ChangeAnimation(L"IDLE");

		m_eCurState = eTRAP_STATE::IDLE;
	}


	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 	CGameObject::Update_GameObject(fTimeDelta);
}

void CGeyserTrap::Render_GameObject()
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

CGeyserTrap* CGeyserTrap::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos)
{
	CGeyserTrap* pInstance = new CGeyserTrap(pGraphicDev, _strObjName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->GetTransForm()->Set_Pos(vPos);

	return pInstance;
}

CGeyserTrap* CGeyserTrap::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CGeyserTrap* pInstance = new CGeyserTrap(pGraphicDev, _strObjName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CGeyserTrap::Add_Component()
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


	m_pAnimCom->AddAnimation(L"IDLE", proto::Clone_ProtoAnim(L"GEYSER_IDLE"));// 분출 전

	m_pAnimCom->AddAnimation(L"OPEN", proto::Clone_ProtoAnim(L"GEYSER_OPEN")); // 분출

	m_pAnimCom->AddAnimation(L"LOOP", proto::Clone_ProtoAnim(L"GEYSER_LOOP")); // 계속 분출중


	m_pAnimCom->AddAnimation(L"CLOSE", proto::Clone_ProtoAnim(L"GEYSER_CLOSE")); // 분출끝날때


	m_pAnimCom->SetCurAnimation(L"IDLE");
	m_eCurState = eTRAP_STATE::IDLE;

	m_pTransForm->Set_Scale(_vec3(2.5f, 3.5f, 2.5f));

	return S_OK;
}
