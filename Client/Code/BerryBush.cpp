#include "BerryBush.h"


CBerryBush::CBerryBush(LPDIRECT3DDEVICE9 pGrahpicDev)
	: CResObject(pGrahpicDev),
	m_eBerryCurState(BERRY_END),
	m_eBerryPreState(BERRY_END)
{
}

CBerryBush::~CBerryBush()
{
}

HRESULT CBerryBush::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eBerryCurState = BERRY_IDLE;
	m_eObject_id = BERRY_BUSH;
	m_fFrame = 0.f;
	m_fFrameEnd = 0.f;
	m_fCollisionRadius = 0.8f;
	Ready_Stat();
	return S_OK;
}

_int CBerryBush::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += m_fFrameEnd * fTimeDelta;

	if (m_fFrameEnd < m_fFrame)
	{
		if (m_eBerryCurState == BERRY_PICK || m_eBerryCurState == BERRY_EMPTY_PICK)
			m_eBerryCurState = BERRY_EMPTY;

		m_fFrame = 0.f;
	}

	CGameObject::Update_GameObject(fTimeDelta);


	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CBerryBush::LateUpdate_GameObject()
{
	Change_Frame_Event();
	Check_FrameState();
	_vec3 vPos;
	m_pTransForm->BillBoard();

	__super::LateUpdate_GameObject();

	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CBerryBush::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pBerryTextureCom[m_eBerryCurState]->Set_Texture((int)m_fFrame);
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CBerryBush::Change_Frame_Event()
{
	if (m_bHit && m_eBerryCurState == BERRY_IDLE)
		m_eBerryCurState = BERRY_PICK;
	else if (m_bHit && m_eBerryCurState == BERRY_EMPTY)
		m_eBerryCurState = BERRY_EMPTY_PICK;
	else if (m_Stat.fHP <= 0)
	{

		if (_bDropItem == false)
		{
			//srand(static_cast<unsigned int>(time(nullptr)));
			//int iItemCount = rand() % 1 + 3;	//¾ÆÀÌÅÛ °¹¼ö¿ë
			//for (int i = 0; i < iItemCount; ++i)
			//{
			//	int signX = (rand() % 2 == 0) ? -1 : 1;
			//	int signZ = (rand() % 2 == 0) ? -1 : 1;
			//	int iItemPosX = rand() % 3 * signX;
			//	int iItemPosZ = rand() % 3 * signZ;
			//	_vec3 vPos;
			//	m_pTransForm->Get_Info(INFO_POS, &vPos);
			//	vPos.x += iItemPosX;
			//	vPos.y = 0.8f;
			//	vPos.z += iItemPosZ;
			//	CGameObject* pGameObj = CItemBasic::Create(m_pGraphicDev, L"Berries");
			//	dynamic_cast<CItemBasic*>(pGameObj)->SetCreateByObject();
			//	pGameObj->GetTransForm()->Set_Pos(vPos);
			//	scenemgr::Get_CurScene()->AddGameObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::ITEM, pGameObj);
			//}

			CreateItem(L"Berries", this, this->m_pGraphicDev);
	
		}
		_bDropItem= true;
		m_eBerryCurState = BERRY_EMPTY;

	}
}

void CBerryBush::Check_FrameState()
{
	if (m_eBerryPreState == m_eBerryCurState)
		return;

	if (m_eBerryCurState == BERRY_IDLE || m_eBerryCurState == BERRY_EMPTY)
		m_fFrameEnd = 0;
	if (m_eBerryCurState == BERRY_PICK)
		m_fFrameEnd = 3;
	if (m_eBerryCurState == BERRY_EMPTY_PICK)
		m_fFrameEnd = 5;

	m_eBerryPreState = m_eBerryCurState;
	m_fFrame = 0;
}

void CBerryBush::Ready_Stat()
{
	m_Stat.strObjName = L"º£¸® ´ýºÒ";
	m_Stat.fHP = 1.f;
	m_Stat.fMxHP = 1.f;
	m_Stat.fSpeed = 1.f;
	m_Stat.bDead = false;
}

HRESULT CBerryBush::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pBerryTextureCom[BERRY_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_most_idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_most_idle", pComponent });

	pComponent = m_pBerryTextureCom[BERRY_PICK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_most_pick"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_most_pick", pComponent });

	pComponent = m_pBerryTextureCom[BERRY_EMPTY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_empty_idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_empty_idle", pComponent });

	pComponent = m_pBerryTextureCom[BERRY_EMPTY_PICK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_empty_pick"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_empty_pick", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(1.f, 1.f, 1.f));
	//m_pTransForm->Get_Info(INFO_POS, &vPos);
	//m_pTransForm->Set_Pos(vPos.x, 0.5f, vPos.z);

	return S_OK;
}

CResObject* CBerryBush::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CBerryBush(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBerryBush::Free()
{
	CGameObject::Free();
}

