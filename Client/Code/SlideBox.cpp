#include "stdafx.h"
#include "SlideBox.h"


#include "Export_Utility.h"
#include "Export_System.h"


#include "UIMgr.h"
#include "CItem.h"
#include "Texture.h"
#include "Scene.h"
#include <Mouse.h>

CSlideBox::CSlideBox(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CUI(pGraphicDev), m_vPos(vPos), m_bPanelShow(false)
{
}

CSlideBox::CSlideBox(const CSlideBox& rhs)
	: CUI(rhs)
{
}

CSlideBox::~CSlideBox()
{
}

void CSlideBox::Set_Item(CItem* pItem)
{
	m_pItem = pItem;

	_vec3 vPos = _vec3(m_vPos.x + 120.f, m_fY, 0.f);
	m_pPanel = CExplainPanel::Create(m_pGraphicDev, vPos, m_pItem->GetObjName());
}

HRESULT CSlideBox::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = m_vPos.x - 100.f;
	m_fY = m_vPos.y;
	m_fSizeX = 30.f;
	m_fSizeY = 30.f;

	m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(90.f));

	__super::Ready_GameObject();

	return S_OK;
}

_int CSlideBox::Update_GameObject(const _float& fTimeDelta)
{
	if (m_fX != m_vPos.x)
	{
		m_fX += 200.f * fTimeDelta;

		if (m_bPanelShow) m_bPanelShow = false;

		if (m_fX > m_vPos.x)
		{
			m_fX = m_vPos.x;
			m_bPanelShow = true;
		}
			
		if (m_pItem) m_pItem->Set_fX(m_fX);
		m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.1f));
	}

	if (m_pItem) m_pItem->Update_GameObject(fTimeDelta);
	if (m_pPanel) m_pPanel->Update_GameObject(fTimeDelta);

	Input_Mouse();

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CSlideBox::LateUpdate_GameObject()
{
	if (m_pItem) m_pItem->LateUpdate_GameObject();
	if (m_pPanel && m_bPanelShow) // 알파소팅
	{
		m_pPanel->LateUpdate_GameObject();

		CUI* pUI = dynamic_cast<CUI*>(m_pPanel);
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

void CSlideBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);
	
	m_pBufferCom->Render_Buffer();
	if (m_pItem) m_pItem->Render_GameObject();
	if (m_pPanel && m_bPanelShow) m_pPanel->Render_GameObject();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CSlideBox* CSlideBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
	CSlideBox* pInstance = new CSlideBox(pGraphicDev, _vPos);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CSlideBox::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Explain_PopUp_Panel"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Explain_PopUp_Panel", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CSlideBox::Input_Mouse()
{
	if (!m_pPanel || !m_bPanelShow) return;

	POINT tPt;
	GetCursorPos(&tPt);
	ScreenToClient(g_hWnd, &tPt);
	_vec2 vMousePos = _vec2(tPt.x, tPt.y);

	if (Engine::Collision_Mouse(vMousePos, m_fX, m_fY, m_fSizeX, m_fSizeY))
	{
		if (!m_pPanel->Get_m_bSlideBoxColl())
			Engine::PlaySound_W(L"UI_Click_Mouse.mp3", SOUND_MOUSE, 0.1f);

		m_pPanel->Set_m_bSlideBoxColl(true);
	}
	else
	{
		if (!m_pPanel->Get_Show())
			m_pPanel->Set_m_bSlideBoxColl(false);
	}
}


void CSlideBox::Free()
{
	Safe_Release(m_pPanel);
	Safe_Release(m_pItem);

	__super::Free();
}
