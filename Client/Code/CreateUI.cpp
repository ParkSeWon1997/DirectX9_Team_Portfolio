#include "CreateUI.h"
#include "SlotMgr.h"
#include <ItemTool.h>
#include "Export_System.h"
#include "stdafx.h"
#include "UIMgr.h"

CCreateUI::CCreateUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CCreateUI::~CCreateUI()
{
}

HRESULT CCreateUI::Ready_GameObject()
{
	CSlotMgr::GetInstance()->Add_InvenBoxList(m_pGraphicDev, CREATE, HEIGHT, 4);

	// 고정 아이템 이미지 넣어주기 
	_vec3 vPos;
	CSlotMgr::GetInstance()->Get_BoxPos(CREATE, 0, &vPos);
	CItem* pItem = CItemTool::Create(m_pGraphicDev, L"Proto_UI_Weapon", vPos);
	CSlotMgr::GetInstance()->Set_Create_Menu(0, pItem);

	CSlotMgr::GetInstance()->Get_BoxPos(CREATE, 1, &vPos);
	pItem = CItemTool::Create(m_pGraphicDev, L"Proto_UI_Alive", vPos);
	CSlotMgr::GetInstance()->Set_Create_Menu(1, pItem);

	CSlotMgr::GetInstance()->Get_BoxPos(CREATE, 2, &vPos);
	pItem = CItemTool::Create(m_pGraphicDev, L"Proto_UI_Equipment", vPos);
	CSlotMgr::GetInstance()->Set_Create_Menu(2, pItem);

	CSlotMgr::GetInstance()->Get_BoxPos(CREATE, 3, &vPos);
	pItem = CItemTool::Create(m_pGraphicDev, L"Proto_UI_Processing", vPos);
	CSlotMgr::GetInstance()->Set_Create_Menu(3, pItem);

	for (size_t i = 0; i < 4; ++i)
	{
		CSlideUI* pUI = CSlideUI::Create(m_pGraphicDev, (eITEMTOOL_TYPE)i);
		m_vecSlide.push_back(pUI);
	}

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = 30.f;
	m_fY = 300.f;

	m_fSizeX = 30.f;
	m_fSizeY = 200.f;

	//직교투영
	__super::Ready_GameObject();

	return S_OK;
}

_int CCreateUI::Update_GameObject(const _float& fTimeDelta)
{
	Picking_Mouse();

	CSlotMgr::GetInstance()->Update_InvenBoxMgr(fTimeDelta, CREATE);
	for (auto& iter : m_vecSlide) iter->Update_GameObject(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CCreateUI::LateUpdate_GameObject()
{
	CSlotMgr::GetInstance()->LateUpdate_InvenBoxMgr(CREATE);
	for (auto& iter : m_vecSlide) iter->LateUpdate_GameObject();

	__super::LateUpdate_GameObject();
}

void CCreateUI::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	for (auto& iter : m_vecSlide) iter->Render_GameObject();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);
	FAILED_CHECK_RETURN(SetUp_Material(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)), );
	m_pBufferCom->Render_Buffer();

	CSlotMgr::GetInstance()->Render_InvenBoxMgr(CREATE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CCreateUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Left_Panel"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Left_Panel", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CCreateUI::Picking_Mouse() // 제작 목록 클릭 시 설정
{
	if (Engine::GetMouseState(DIM_LB) == eKEY_STATE::TAP)
	{
		POINT tPt;
		GetCursorPos(&tPt);
		ScreenToClient(g_hWnd, &tPt);
		_vec2 vMousePos = _vec2(tPt.x, tPt.y);
		vector<CSlot*> vecInvenBox = CSlotMgr::GetInstance()->Get_BoxList(CREATE);
	
		for (int i = 0; i < vecInvenBox.size(); ++i)
		{
			if (!Engine::Collision_Mouse(vMousePos, vecInvenBox[i]->Get_fX(), vecInvenBox[i]->Get_fY(), vecInvenBox[i]->Get_fSizeX(), vecInvenBox[i]->Get_fSizeY()))
				continue;

			Engine::PlaySound_W(L"UI_Click_Move.mp3", SOUND_MOUSE, 1.f);
			if (m_vecSlide[i]->Get_Show()) // 누른 목록이 이미 보여지고 있다면
			{
				m_vecSlide[i]->Set_Show(false);
				break;
			}

			// 누른 목록이 보여지고 있지 않다면 누른 목록을 제외하고 모두 보여지지 않도록 설정한다.
			for (int j = 0; j < m_vecSlide.size(); ++j)
				m_vecSlide[j]->Set_Show(i == j ? true : false);

			break;
		}
	}
}

CUI* CCreateUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCreateUI* pInstance = new CCreateUI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CCreateUI::Free()
{
	for (auto& iter : m_vecSlide)
		Safe_Release(iter);

	__super::Free();
}
