#include "stdafx.h"
#include "CBossDoor.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "LoadingScene.h"

CBossDoor::CBossDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
	, m_pAnimCom(nullptr)
	, m_eBossDoorCurState(BOSSDOOR_END)
{
}

CBossDoor::CBossDoor(const CBossDoor& rhs)
	: CResObject(rhs.m_pGraphicDev)
	, m_eBossDoorCurState(rhs.m_eBossDoorCurState)
	, m_pAnimCom(nullptr)
{

}

CBossDoor::~CBossDoor()
{
}

HRESULT CBossDoor::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransForm->Set_Pos(_vec3(rand() % 20, 1.5f, rand() % 20));
	m_eObject_id = BOSS_DOOR;
	m_eBossDoorPrevState = BOSSDOOR_END;
	m_eBossDoorCurState = SLEEP;
	//Ready_Stat();
	m_Stat.strObjName = L"º¸½º·ë";

	return S_OK;
}

_int CBossDoor::Update_GameObject(const _float& fTimeDelta)
{

	if (!m_bFrameStop)
	{
		m_fFrame += m_fFrameEnd * fTimeDelta;
	}
	if (get<0>(IsPlayerInRadius()) && !m_bStateChange[0])
	{
		m_bStateChange[0] = true;
	}



	if (m_bStateChange[0])
	{
		if (m_bStateChange[1])
		{
			if (m_bStateChange[2])
			{
				Close(fTimeDelta);
			}
			else
				IDLE(fTimeDelta);
		}
		else
			Open(fTimeDelta);
	}
	else
		Sleep(fTimeDelta);

	Check_FrameState();

	ChangeScenePlayer(4.f);//¿úÈ¦ÀÌ¶û°°À½

	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CBossDoor::LateUpdate_GameObject()
{




	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	__super::LateUpdate_GameObject();
}

void CBossDoor::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//m_pAnimCom->SetAnimTexture();
	m_pBossDoorTextureCom[m_eBossDoorPrevState]->Set_Texture((_uint)m_fFrame);
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CBossDoor::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pBossDoorTextureCom[BOSSDOOR_OPEN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_BossDoor_Open"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_BossDoor_Open", pComponent });
	
	
	pComponent = m_pBossDoorTextureCom[BOSSDOOR_CLOSE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_BossDoor_Close"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_BossDoor_Close", pComponent });
	
	pComponent = m_pBossDoorTextureCom[BOSSDOOR_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_BossDoor_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_BossDoor_Idle", pComponent });

	pComponent = m_pBossDoorTextureCom[SLEEP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_BossDoor_Off"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_BossDoor_Off", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });


	m_pTransForm->Set_Scale(_vec3(2.5f, 2.5f, 2.5f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.0f, vPos.z);

	return S_OK;
}

void CBossDoor::Check_FrameState()
{
	if (m_eBossDoorPrevState != m_eBossDoorCurState)
	{
		switch (m_eBossDoorCurState)
		{
		case CBossDoor::BOSSDOOR_OPEN:
		{
			Engine::StopSound(SOUND_EFFECT_CONTINUE_CH1);
			Engine::PlaySound_W(L"Obj_Teleport_Activate.mp3", SOUND_TELEPORT, 0.4f);
			m_fFrameEnd = 19.f;
			break;
		}
		case CBossDoor::BOSSDOOR_CLOSE:
		{
			Engine::StopSound(SOUND_EFFECT_CONTINUE_CH1);
			Engine::PlaySound_W(L"Obj_Teleport_Deactivate.mp3", SOUND_TELEPORT, 0.4);

			m_fFrameEnd = 10.f;
			break;
		}
		case CBossDoor::BOSSDOOR_IDLE:
		{
			Engine::PlayEffectContinue(L"Obj_Teleport_Idle.mp3", 0.2f, SOUND_EFFECT_CONTINUE_CH1);
			m_fFrameEnd = 8.f;
			break;
		}
		case SLEEP:
			m_fFrame = 0;
			m_fFrameEnd = 0;
			break;
		default:
			break;
		}
		m_fFrame = 0.f;
		m_eBossDoorPrevState = m_eBossDoorCurState;
	}
	else
		return;
}

void CBossDoor::Sleep(const _float& fTimeDelta)
{

	return;
}

void CBossDoor::Open(const _float& fTimeDelta)
{
	m_bFrameStop = false;

	if (m_eBossDoorPrevState == SLEEP)
	{
		m_eBossDoorCurState = BOSSDOOR_OPEN;
	}
	else if (m_eBossDoorPrevState == BOSSDOOR_OPEN)
	{
		if (m_fFrameEnd < m_fFrame)
		{
			m_eBossDoorCurState = BOSSDOOR_IDLE;
			m_bStateChange[1] = true;
		}
	}
	
	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0;
	}

}

void CBossDoor::IDLE(const _float& fTimeDelta)
{

	if (!get<0>(IsPlayerInRadius()))
	{
		m_bStateChange[2] = true;
		m_eBossDoorCurState = BOSSDOOR_CLOSE;
	}


	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0;
	}
}

void CBossDoor::Close(const _float& fTimeDelta)
{
	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0;
		m_bFrameStop = true;
		m_eBossDoorCurState = SLEEP;
		m_bStateChange[0] = false;
		m_bStateChange[1] = false;
		m_bStateChange[2] = false;
	}

}

void CBossDoor::ChangeScenePlayer(_float _fDistance)
{

	_vec3 vPlayerPos;
	_vec3 vPos;
	_vec3 vDistance;

	float fDistance = 0.f;
	scenemgr::Get_CurScene()->GetPlayerObject()->GetTransForm()->Get_Info(INFO_POS, &vPlayerPos);
	GetTransForm()->Get_Info(INFO_POS, &vPos);

	vDistance = vPlayerPos - vPos;
	vDistance.y = 0.f;
	fDistance = D3DXVec3Length(&vDistance);
	if (fDistance <= _fDistance)
	{
		if (KEY_TAP(DIK_C))
		{
			Engine::StopSound(SOUND_EFFECT_CONTINUE_CH1);
			Engine::PlaySound_W(L"Obj_Teleport_MoveTeleport.mp3", SOUND_TELEPORT, 0.4f);
			ChangeScene(CLoadingScene::Create(m_pGraphicDev, CLoading::LOADING_BOSS));
		}
	}


}









CResObject* CBossDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CBossDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBossDoor::Free()
{
	CGameObject::Free();
}

tuple<_bool, _vec3> CBossDoor::IsPlayerInRadius()
{
	bool IsClose = false;
	_vec3 vPlayerPos;
	_vec3 vMyPos;
	decltype(auto) vPlayerTrans = scenemgr::Get_CurScene()->GetPlayerObject()->GetTransForm();
	vPlayerTrans->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransForm->Get_Info(INFO_POS, &vMyPos);
	vPlayerPos.y = 0.f;
	vMyPos.y = 0.f;

	_vec3 vDir = vPlayerPos - vMyPos;
	float _test = D3DXVec3Length(&vDir);

	if (D3DXVec3Length(&(vPlayerPos - vMyPos)) < 3.0f)
	{
		//m_eTelporterCurState = TELEPORTER_OPEN;
		IsClose = true;
	}
	else
	{
		//m_eTelporterCurState = TELEPORTER_IDLE;
		IsClose = false;
	}
	return make_tuple(IsClose, vPlayerPos);
}


