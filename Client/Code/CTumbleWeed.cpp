#include "stdafx.h"
#include "CTumbleWeed.h"
#include "Export_Utility.h"
#include "Player.h"

CTumbleWeed::CTumbleWeed(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CTrap(pGraphicDev, _strObjName)
{
}

CTumbleWeed::CTumbleWeed(const CTumbleWeed& rhs)
	:CTrap(rhs)
{
}

CTumbleWeed::~CTumbleWeed()
{
}



_int CTumbleWeed::Update_GameObject(const _float& fTimeDelta)
{

	if (m_eCurState == eTRAP_STATE::MOVE)//�����̴»��¸� �÷��̾�üũ
	{
		if (Collision_Circle(scenemgr::Get_CurScene()->GetPlayerObject()))//�÷��̾� �¾������
		{
			m_eCurState = eTRAP_STATE::DEAD;
			dynamic_cast<CPlayer*>(scenemgr::Get_CurScene()->GetPlayerObject())->Set_Attack(1);
			m_pAnimCom->ChangeAnimation(L"BREAK");
			m_pAnimCom->SetLoopAnimator(false);
		}
		else//�ƴϸ��̵�
		{
			MovePos(fTimeDelta);
		}
	}

	if (m_eCurState == eTRAP_STATE::DEAD)//��������ϰ�� ����ִϸ��̼� �ٳ����� ����
	{
		if (m_pAnimCom->IsFinish(L"BREAK"))
		{
			DeleteObject(this);
		}
	}


	return __super::Update_GameObject(fTimeDelta);
}

void CTumbleWeed::Render_GameObject()
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

CTumbleWeed* CTumbleWeed::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos)
{
	CTumbleWeed* pInstance = new CTumbleWeed(pGraphicDev, _strObjName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->GetTransForm()->Set_Pos(vPos);

	return pInstance;
}

CTumbleWeed* CTumbleWeed::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CTumbleWeed* pInstance = new CTumbleWeed(pGraphicDev, _strObjName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTumbleWeed::MovePos(const _float& fTimeDelta)
{

	_vec3 vPos(-1.f,0.f,0.f);


	m_pTransForm->Move_Pos(&vPos,3.f, fTimeDelta);

}

HRESULT CTumbleWeed::Add_Component()
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


	m_pAnimCom->AddAnimation(L"MOVE_DOWN", proto::Clone_ProtoAnim(L"TRAP_TUMBLE_MOVE_DOWN"));//�Ʒ�

	m_pAnimCom->AddAnimation(L"MOVE_UP", proto::Clone_ProtoAnim(L"TRAP_TUMBLE_MOVE_UP"));//��

	m_pAnimCom->AddAnimation(L"MOVE_SIDE", proto::Clone_ProtoAnim(L"TRAP_TUMBLE_MOVE_SIDE"));// ������ ����

	m_pAnimCom->AddAnimation(L"BREAK", proto::Clone_ProtoAnim(L"TRAP_TUMBLE_BREAK"));//Ÿ���� �����


	m_pAnimCom->SetCurAnimation(L"MOVE_DOWN");
	m_pAnimCom->SetLoopAnimator(true);
	m_eCurState = eTRAP_STATE::MOVE;

	m_pTransForm->Set_Scale(_vec3(1.f, 1.f, 1.f));

	return S_OK;
}
