#include "stdafx.h"
#include "CObjectGrass.h"
#include "Export_System.h"
#include "Export_Utility.h"

CObjectGrass::CObjectGrass(LPDIRECT3DDEVICE9 pGraphicDev)
	:CResObject(pGraphicDev)
{
}

CObjectGrass::CObjectGrass(const CObjectGrass& rhs)
	:CResObject(rhs.m_pGraphicDev)
{
}

CObjectGrass::~CObjectGrass()
{
}

HRESULT CObjectGrass::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);



		_vec3 vPos;
	m_pTransForm->Set_Scale(_vec3(1.5f, 1.5f, 1.5f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 0.f, vPos.z);

	Ready_Stat();
	m_fCollisionRadius = 0.4;
	m_eCurState = RES_IDLE;
	m_eObject_id = GRASS;
	m_fFrame = 0;
	m_fFrameEnd = 28;
	return S_OK;
}

_int CObjectGrass::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += m_fFrameEnd * fTimeDelta;

	if (m_fFrameEnd < m_fFrame)
	{
		if (m_eCurState == RES_DEAD)
		{
			CreateItem(L"CutGlass", this, this->m_pGraphicDev,2);
			DeleteObject(this);//??
		}
			

		m_fFrame = 0.f;
	}

	CGameObject::Update_GameObject(fTimeDelta);

	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CObjectGrass::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	Change_Frame_Event();
	Check_FrameState();

	CGameObject::LateUpdate_GameObject();
	m_pTransForm->BillBoard();
	_vec3	vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CObjectGrass::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom[m_eCurState]->Set_Texture((_uint)m_fFrame);
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CObjectGrass::Add_Component()
{
	CComponent* pComponent = nullptr;


	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom[RES_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Grass_idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Grass_idle", pComponent });

	pComponent = m_pTextureCom[RES_DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Grass_dead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Grass_dead", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });



	return S_OK;
}

void CObjectGrass::Check_FrameState()
{
	if (m_ePreState == m_eCurState)
		return;

	if (m_eCurState == RES_IDLE)
		m_fFrameEnd = 28;
	if (m_eCurState == RES_DEAD)
		m_fFrameEnd = 7;

	m_ePreState = m_eCurState;
	m_fFrame = 0;
}

void CObjectGrass::Change_Frame_Event()
{
	if (m_Stat.fHP <= 0)
		m_eCurState = RES_DEAD;
}

void CObjectGrass::Ready_Stat()
{
	m_Stat.strObjName = L"Ç®»Ñ¸®";
	m_Stat.fHP = 1;
	m_Stat.fMxHP = 1;
	m_Stat.fSpeed = 1.f;
	m_Stat.bDead = false;
}

CResObject* CObjectGrass::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CObjectGrass(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CObjectGrass::Free()
{
	CGameObject::Free();
}
