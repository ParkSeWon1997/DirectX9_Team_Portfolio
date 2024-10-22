#include "stdafx.h"
#include "CTeleporterWorm.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "LoadingScene.h"

CTeleporterWorm::CTeleporterWorm(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
{
}

CTeleporterWorm::CTeleporterWorm(const CTeleporterWorm& rhs)
	: CResObject(rhs.m_pGraphicDev)
{
}

CTeleporterWorm::~CTeleporterWorm()
{
}

HRESULT CTeleporterWorm::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransForm->Set_Pos(_vec3(rand() % 20, 1.5f, rand() % 20));


	m_eObject_id = TELEPORTER;
	m_eTelporterCurState = TELEPORTER_IDLE;
	m_fFrame = 0.f;
	m_fFrameEnd = 6.f;
	//Ready_Stat();
	m_Stat.strObjName = L"웜홀";

	return S_OK;
}

_int CTeleporterWorm::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bFrameStop)
	{
		m_fFrame += m_fFrameEnd * fTimeDelta*2.0f;

	}
	ChangeScenePlayer(4.f);





	Change_Frame_Event();
	
	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CTeleporterWorm::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	Check_FrameState();

	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

}

void CTeleporterWorm::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pTeleporterTextureCom[m_eTelporterCurState]->Set_Texture((_uint)m_fFrame);
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CTeleporterWorm::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTeleporterTextureCom[TELEPORTER_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Teleporter_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Teleporter_Idle", pComponent });

	pComponent = m_pTeleporterTextureCom[TELEPORTER_OPEN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Teleporter_Open"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Teleporter_Open", pComponent });

	pComponent = m_pTeleporterTextureCom[TELEPORTER_CLOSE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Teleporter_Close"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Teleporter_Close", pComponent });



	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(2.f, 2.f, 2.f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 0.f, vPos.z);

	return S_OK;
}


void CTeleporterWorm::Change_Frame_Event()
{
	bool IsClose = get<0>(IsPlayerInRadius());
	bool IsMoreClose = get<2>(IsPlayerInRadius());
	//플레이어 pos 저장용 , 나중에 멤버변수로 가지고 있어도 됨
	_vec3 vPlayerPos = get<1>(IsPlayerInRadius());

	if (m_eTelporterCurState == TELEPORTER_IDLE)
	{
		
		if (m_fFrame > m_fFrameEnd)
		{
			m_fFrame = 0.0f;
		}
	}


	if (IsClose)
	{
		m_eTelporterCurState = TELEPORTER_OPEN;

		//열릴 때
		if (m_eTelporterCurState == TELEPORTER_OPEN)
		{
			
			if (m_fFrame > m_fFrameEnd)
			{
				m_bFrameStop = true;
				m_fFrame = 5.0f;
			}

		}


	}
	else if (m_eTelporterCurState == TELEPORTER_OPEN || m_eTelporterCurState == TELEPORTER_CLOSE)
	{
		m_bFrameStop = false;
		m_eTelporterCurState = TELEPORTER_CLOSE;
		//닫힐 때
		if (m_eTelporterCurState == TELEPORTER_CLOSE)
		{
			//m_pTransForm->Set_Scale(_vec3(4.5f, 4.5f, 4.5f));
			if (m_fFrame > m_fFrameEnd)
			{
				m_eTelporterCurState = TELEPORTER_IDLE;
				m_fFrame = 0.0f;
			}
		}

	}


}

void CTeleporterWorm::Check_FrameState()
{

	if (m_eTelporterPreState != m_eTelporterCurState)
	{


		switch (m_eTelporterCurState)
		{
		case TELEPORTER_STATE::TELEPORTER_IDLE:
		{
			//m_pTransForm->Set_Scale(_vec3(2.5f, 2.5f, 2.5f));
			m_fFrameEnd = 6.0f;
			break;

		}
		case TELEPORTER_STATE::TELEPORTER_OPEN:
		{
			Engine::PlaySound_W(L"Obj_Worm_Open.mp3", SOUND_TELEPORT, 0.5f);
			//m_pTransForm->Set_Scale(_vec3(5.5f, 5.5f, 5.5f));
			m_fFrameEnd = 5.0f;
			break;

		}
		case TELEPORTER_STATE::TELEPORTER_CLOSE:
		{
			Engine::PlaySound_W(L"Obj_Worm_Close.mp3", SOUND_TELEPORT, 0.5f);
			m_fFrameEnd = 5.0f;
			break;

		}
		default:
			m_eTelporterCurState = TELEPORTER_IDLE;
			m_fFrame = 0.0f;
			m_fFrameEnd = 5.0f;

			break;

		}

		m_eTelporterPreState = m_eTelporterCurState;
		m_fFrame = 0.0f;
	}

}







bool CTeleporterWorm::IsPlayerInRadius(_vec3& _PlayerPos, _vec3& _MyPos)
{
	_PlayerPos.y = 0.f;
	_MyPos.y = 0.f;
	if (D3DXVec3Length(&(_PlayerPos - _MyPos)) < 3.f)
	{
		return true;
	}
	else
	{
		return  false;
	}


}

tuple<_bool, _vec3, _bool> CTeleporterWorm::IsPlayerInRadius()
{

	bool IsClose = false;
	bool IsMoreClose = false;
	_vec3 vPlayerPos;
	_vec3 vMyPos;
	decltype(auto) vPlayerTrans = scenemgr::Get_CurScene()->GetPlayerObject()->GetTransForm();
	vPlayerTrans->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransForm->Get_Info(INFO_POS, &vMyPos);
	vPlayerPos.y = 0.f;
	vMyPos.y = 0.f;

	_vec3 vDir = vPlayerPos - vMyPos;
	float _test = D3DXVec3Length(&vDir);

	if (D3DXVec3Length(&(vPlayerPos - vMyPos)) < 4.0f)
	{
		if (D3DXVec3Length(&(vPlayerPos - vMyPos)) < 1.0f)
		{
			IsMoreClose = true;
		}
		else
			IsMoreClose = false;

		IsClose = true;
	}
	else
	{
		IsClose = false;
	}




	return make_tuple(IsClose, vPlayerPos, IsMoreClose);
}

//충돌반경
void CTeleporterWorm::ChangeScenePlayer(_float _fDistance)
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
			Engine::PlaySound_W(L"Obj_Worm_Travel.mp3", SOUND_EFFECT, 0.5f);
			ChangeScene(CLoadingScene::Create(m_pGraphicDev,CLoading::LOADING_ROAD));
		}
	}


}


CResObject* CTeleporterWorm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CTeleporterWorm(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CTeleporterWorm::Free()
{
	CGameObject::Free();
}


