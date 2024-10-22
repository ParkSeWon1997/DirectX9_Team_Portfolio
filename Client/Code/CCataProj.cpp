#include "stdafx.h"
#include "CCataProj.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "Player.h"

CCataProj::CCataProj(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CTrap(pGraphicDev, _strObjName)
	, m_fSpeed(8.f)
{
}

CCataProj::CCataProj(const CCataProj& rhs)
	:CTrap(rhs)
{
}

CCataProj::~CCataProj()
{
}

HRESULT CCataProj::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos;
	vPos = m_pTransForm->Get_Pos();

	vPos.y -= 0.5f;
	m_pTransForm->Set_Pos(vPos);

	m_pTransForm->Set_Scale(_vec3(0.3f, 0.3f, 0.3f));
	return S_OK;
}

_int CCataProj::Update_GameObject(const _float& fTimeDelta)
{

	if (m_eCurState == eTRAP_STATE::MOVE)//움직이는상태면 플레이어체크
	{
		if (PlayerHit(0.5f))//플레이어 맞았을경우
		{
			m_eCurState = eTRAP_STATE::DEAD;
			dynamic_cast<CPlayer*>(scenemgr::Get_CurScene()->GetPlayerObject())->Set_Attack(1);
			m_pAnimCom->ChangeAnimation(L"HIT");
			m_pAnimCom->SetLoopAnimator(false);
			PlayHitSound();
		}
		else//아니면이동
		{
			MovePos(fTimeDelta);
		}
	}

	if (m_eCurState == eTRAP_STATE::DEAD)//데드상태일경우 데드애니메이션 다끝나면 삭제
	{
		if (m_pAnimCom->IsFinish(L"HIT"))
		{
			DeleteObject(this);
		}
	}


	return __super::Update_GameObject(fTimeDelta);
}

void CCataProj::Render_GameObject()
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

CCataProj* CCataProj::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos)
{
	CCataProj* pInstance = new CCataProj(pGraphicDev, _strObjName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->GetTransForm()->Set_Pos(vPos);

	return pInstance;
}

void CCataProj::MovePos(const _float& fTimeDelta)
{

	_vec3 vPos(-1.f, 0.f, 0.f);


	m_pTransForm->Move_Pos(&vPos, m_fSpeed, fTimeDelta);

}

HRESULT CCataProj::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });


	pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pAnimCom = dynamic_cast<CAnimator*>(proto::Clone_Proto(L"Proto_Anim"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Anim", pComponent });


	m_pAnimCom->AddAnimation(L"AIR", proto::Clone_ProtoAnim(L"PROJ_CATAPULT_AIR"));//아래

	m_pAnimCom->AddAnimation(L"HIT", proto::Clone_ProtoAnim(L"PROJ_CATAPULT_HIT"));


	m_pAnimCom->SetCurAnimation(L"AIR");
	m_pAnimCom->SetLoopAnimator(true);
	m_eCurState = eTRAP_STATE::MOVE;


	return S_OK;
}

void CCataProj::PlayHitSound()
{
	int randomvalue = rand() % 5;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"CataPultRockHit_1.mp3", CHANNELID::SOUND_EFFECT_CONTINUE_CH1, 0.2f);
		break;
	case 1:
		Engine::PlaySound_W(L"CataPultRockHit_2.mp3", CHANNELID::SOUND_EFFECT_CONTINUE_CH2, 0.2f);
		break;
	case 2:
		Engine::PlaySound_W(L"CataPultRockHit_3.mp3", CHANNELID::SOUND_EFFECT_CONTINUE_CH3, 0.2f);
		break;
	case 3:
		Engine::PlaySound_W(L"CataPultRockHit_4.mp3", CHANNELID::SOUND_EFFECT_CONTINUE_CH4, 0.2f);
		break;
	case 4:
		Engine::PlaySound_W(L"CataPultRockHit_5.mp3", CHANNELID::SOUND_EFFECT_CONTINUE_CH5, 0.2f);
		break;
	default:
		break;
	}
}