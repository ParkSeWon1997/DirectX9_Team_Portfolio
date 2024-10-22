#include "..\Include\stdafx.h"
#include"SlideUI.h"

#include "Export_Utility.h"
#include "Export_System.h"
#include"SlideBox.h"

#include "SlotMgr.h"
#include <Mouse.h>


CSlideUI::CSlideUI(LPDIRECT3DDEVICE9 pGraphicDev, eITEMTOOL_TYPE eType)
	: CUI(pGraphicDev), m_bShow(false), m_eToolType(eType)

{
}

CSlideUI::CSlideUI(const CSlideUI& rhs)
	: CUI(rhs)
{
}

CSlideUI::~CSlideUI()
{
}


HRESULT CSlideUI::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	for (int j = 0; j < 5; ++j)
	{
		_vec3 vPos = _vec3(90.f, 150.f + 55.f * j, 0.f);
		CSlideBox* pSlideBox = CSlideBox::Create(m_pGraphicDev, vPos);
		//CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::SLIDEBOX, pSlideBox);
		m_vecSlideBox.push_back(pSlideBox);
	}

	_vec3 vPos;
	CItem* pItem = nullptr;
	_float pHeight = 7.f;
	//ToolTypeº°·Î ¾ÆÀÌÅÛ ³Ö¾îÁÜ (°íÁ¤°ª)
	
	switch (m_eToolType)
	{
	case Engine::ITEM_EQUIP: // °î±ªÀÌ, µµ³¢, ¸ÁÄ¡, ¿ä¸®µµ±¸
	{
		vPos = _vec3{ m_vecSlideBox[0]->Get_fX(), m_vecSlideBox[0]->Get_fY() - pHeight , 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"Ax", vPos);
		m_vecSlideBox[0]->Set_Item(pItem);

		vPos = _vec3{ m_vecSlideBox[1]->Get_fX(), m_vecSlideBox[1]->Get_fY() - pHeight, 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"Pickaxe", vPos);
		m_vecSlideBox[1]->Set_Item(pItem);

		vPos = _vec3{ m_vecSlideBox[2]->Get_fX(), m_vecSlideBox[2]->Get_fY() - pHeight, 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"Hammer", vPos);
		m_vecSlideBox[2]->Set_Item(pItem);

		vPos = _vec3{ m_vecSlideBox[3]->Get_fX(), m_vecSlideBox[3]->Get_fY() - pHeight, 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"Cook", vPos);
		m_vecSlideBox[3]->Set_Item(pItem);
	}
		break;
	case Engine::ITEM_ALIVE: // È¶ºÒ, ¸ð´ÚºÒ, ÅÙÆ®
	{
		vPos = _vec3{ m_vecSlideBox[0]->Get_fX(), m_vecSlideBox[0]->Get_fY() - pHeight, 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"Torch", vPos);
		m_vecSlideBox[0]->Set_Item(pItem);

		vPos = _vec3{ m_vecSlideBox[1]->Get_fX(), m_vecSlideBox[1]->Get_fY() - pHeight , 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"BonFire", vPos);
		m_vecSlideBox[1]->Set_Item(pItem);

		vPos = _vec3{ m_vecSlideBox[2]->Get_fX(), m_vecSlideBox[2]->Get_fY() - pHeight , 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"Tent", vPos);
		m_vecSlideBox[2]->Set_Item(pItem);
	}
		break;
	case Engine::ITEM_WEAPON: // ÀüÅõÃ¢
	{
		vPos = _vec3{ m_vecSlideBox[0]->Get_fX(), m_vecSlideBox[0]->Get_fY() - pHeight , 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"Lance", vPos);
		m_vecSlideBox[0]->Set_Item(pItem);

		vPos = _vec3{ m_vecSlideBox[1]->Get_fX(), m_vecSlideBox[1]->Get_fY() - pHeight , 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"Armor", vPos);
		m_vecSlideBox[1]->Set_Item(pItem);

		vPos = _vec3{ m_vecSlideBox[2]->Get_fX(), m_vecSlideBox[2]->Get_fY() - pHeight , 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"LogSuit", vPos);
		m_vecSlideBox[2]->Set_Item(pItem);
	}
		break;
	case Engine::ITEM_PROCESSING: // Àç·áµé
	{
		vPos = _vec3{ m_vecSlideBox[0]->Get_fX(), m_vecSlideBox[0]->Get_fY() - pHeight , 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"Rope", vPos);
		m_vecSlideBox[0]->Set_Item(pItem);

		vPos = _vec3{ m_vecSlideBox[1]->Get_fX(), m_vecSlideBox[1]->Get_fY() - pHeight , 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"Charcoal", vPos);
		m_vecSlideBox[1]->Set_Item(pItem);

		vPos = _vec3{ m_vecSlideBox[2]->Get_fX(), m_vecSlideBox[2]->Get_fY() - pHeight , 0.f };
		pItem = CItemTool::Create(m_pGraphicDev, L"CutStone", vPos);
		m_vecSlideBox[2]->Set_Item(pItem);
	}
		break;
	default:
		break;
	}
	return S_OK;
}

_int CSlideUI::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bShow) return 0;
	__super::Update_GameObject(fTimeDelta);

	for (auto& iter : m_vecSlideBox)
		iter->Update_GameObject(fTimeDelta);

	renderer::Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CSlideUI::LateUpdate_GameObject()
{
	if (!m_bShow) return;
	for (auto& iter : m_vecSlideBox)
	{
		iter->LateUpdate_GameObject();

		CUI* pUI = dynamic_cast<CUI*>(iter);
		CMouse* pMouse = dynamic_cast<CMouse*>(scenemgr::Get_CurScene()->GetMouseObject());
		_vec3 vMousePos = pMouse->Get_MousePos();
		if (Engine::Collision_Mouse(_vec2(vMousePos.x, vMousePos.y), pUI->Get_fX(), pUI->Get_fY(), pUI->Get_fSizeX(), pUI->Get_fSizeY()))
		{
			pMouse->IsColl(false);
			return;
		}
	}

	__super::LateUpdate_GameObject();
}

void CSlideUI::Render_GameObject()
{
	if (!m_bShow)
		return;

	for (auto& iter : m_vecSlideBox)
		iter->Render_GameObject();
}

HRESULT CSlideUI::Add_Component()
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

CSlideUI* CSlideUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, eITEMTOOL_TYPE eType)
{
	CSlideUI* pInstance = new CSlideUI(pGraphicDev, eType);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSlideUI::Free()
{
	for (auto& iter : m_vecSlideBox)
		Safe_Release(iter);

	__super::Free();
}