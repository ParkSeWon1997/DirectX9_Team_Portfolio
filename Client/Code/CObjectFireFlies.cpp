#include "stdafx.h"
#include "CObjectFireFlies.h"

#include "Export_System.h"
#include "Export_Utility.h"

CObjectFireFlies::CObjectFireFlies(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
{
}

CObjectFireFlies::CObjectFireFlies(const CObjectFireFlies& rhs)
	: CResObject(rhs.m_pGraphicDev)
{
}

CObjectFireFlies::~CObjectFireFlies()
{
}

HRESULT CObjectFireFlies::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransForm->Set_Pos(_vec3(rand() % 20, 1.5f, rand() % 20));

	m_eFireFliesCurState = FIREFLIES_PRE;
	m_eObject_id = FIRE_FLIES;
	m_fFrame = 0.f;
	//Ready_Stat();

	m_Stat.strObjName = L"馆调阂捞";

	return S_OK;
}

_int CObjectFireFlies::Update_GameObject(const _float& fTimeDelta)
{
	
	if(!m_bIsFrameStop)
	m_fFrame += m_fFrameEnd * fTimeDelta*0.5;
	






	Change_Frame_Event();
	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CObjectFireFlies::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();


	
	Check_FrameState();





	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

}

void CObjectFireFlies::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pFireFliesTextureCom[m_eFireFliesCurState]->Set_Texture((_uint)m_fFrame);
	FAILED_CHECK_RETURN(SetUp_Material(D3DXCOLOR{1.f, 1.f, 1.f, 1.f}), );
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CObjectFireFlies::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pFireFliesTextureCom[FIREFLIES_PRE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FireFlies_Pre"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FireFlies_Pre", pComponent });

	pComponent = m_pFireFliesTextureCom[FIREFLIES_LOOP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FireFlies_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FireFlies_Idle", pComponent });

	pComponent = m_pFireFliesTextureCom[FIREFLIES_PST] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FireFlies_Pst"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FireFlies_Pst", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(1.f, 1.f, 1.f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 2.3f, vPos.z);

	return S_OK;
}


void CObjectFireFlies::Change_Frame_Event()
{

	//广 老锭
	if (Check_Night())
	{
		if (m_eFireFliesCurState== FIREFLIES_PRE)
		{
			m_bIsFrameStop= false;
			m_fFrameEnd = 16.0f;
			//m_eFireFliesCurState = FIREFLIES_LOOP;
			if (m_fFrame > m_fFrameEnd)
			{
				m_eFireFliesCurState = FIREFLIES_LOOP;
				m_fFrame = 0.0f;
			}
		}
		if (m_eFireFliesCurState == FIREFLIES_LOOP)
		{
			if (m_fFrame > m_fFrameEnd)
			{
				m_fFrame = 0.0f;
			}
		}
	}
	//撤 老锭
	else
	{


		if (m_eFireFliesCurState == FIREFLIES_LOOP)
		{
			m_fFrame = 0.0f;
			m_eFireFliesCurState = FIREFLIES_PST;

		}
		if (m_eFireFliesCurState == FIREFLIES_PST)
		{
			if (m_fFrame > m_fFrameEnd)
			{
				m_eFireFliesCurState = FIREFLIES_PRE;
				m_fFrame = 0.0f;
				m_bIsFrameStop= true;
			}
		}


	}



}

void CObjectFireFlies::Check_FrameState()
{
	if (m_eFireFliesPreState != m_eFireFliesCurState)
	{
		switch (m_eFireFliesCurState)
		{
		case CObjectFireFlies::FIREFLIES_PRE:
			m_fFrameEnd = 16.0f;
			break;
		case CObjectFireFlies::FIREFLIES_LOOP:
			m_fFrameEnd = 32.0f;
			break;
		case CObjectFireFlies::FIREFLIES_PST:
			m_fFrameEnd = 17.0f;
			break;
		case CObjectFireFlies::FIREFLIES_END:
			break;
		default:
			break;
		}
		m_eFireFliesPreState = m_eFireFliesCurState;
		m_fFrame = 0.0f;

	}


}





BOOL CObjectFireFlies::Check_Night()
{
	if (light::Get_TimeIndex() == TIME_STATE::NIGHT)
		return true;
	else
		return false;

}







CResObject* CObjectFireFlies::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CObjectFireFlies(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CObjectFireFlies::Free()
{
	CGameObject::Free();
}


