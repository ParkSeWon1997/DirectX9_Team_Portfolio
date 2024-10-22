#include "..\Include\stdafx.h"
#include "..\Header\CMainUI.h"


#include "Export_Utility.h"
#include "Export_System.h"

#include "Scene.h"
#include"Logo.h"

CMainUI::CMainUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, const _tchar* _UiName)
	: Engine::CGameObject(pGraphicDev, _UiName)
	, m_eUIState(_State)
	, m_bItemChek(false)
	, m_pTextureCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_bColl(false)

{

}

CMainUI::CMainUI(const CMainUI& rhs)
	: Engine::CGameObject(rhs),
	m_eUIState(rhs.m_eUIState)
	, m_bItemChek(rhs.m_bItemChek)

{

}

CMainUI::~CMainUI()
{

}

HRESULT CMainUI::Ready_GameObject(_vec3 _pos, _vec3 _size, float _Angle)
{
	m_fX = _pos.x;
	m_fY = _pos.y;
	m_fSizeX = _size.x;
	m_fSizeY = _size.y;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(_Angle));

	m_pTransForm->Set_Pos(_pos);
	m_pTransForm->Set_Scale(_size);
	return S_OK;
}

Engine::_int CMainUI::Update_GameObject(const _float& fTimeDelta)
{

	Change_Scene();

	__super::Update_GameObject(fTimeDelta);
	m_pTransForm->Get_WorldMatrix()->_41 = m_fX - (WINCX >> 1);
	m_pTransForm->Get_WorldMatrix()->_42 = -m_fY + (WINCY >> 1);

	renderer::Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CMainUI::LateUpdate_GameObject()
{

	__super::LateUpdate_GameObject();
}

void CMainUI::Render_GameObject()
{
	scenemgr::Get_CurScene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_bColl);
	//Engine::Render_Font(L"Button_Make", L"게임 시작", &_vec2(m_fX - 20.f, m_fY - 8.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	//m_pTransForm->Set_Pos(m_fX - (WINCX >> 1), -m_fY + (WINCY >> 1), 0.f);
	//m_pTransForm->Set_Scale(_vec3{ m_fSizeX, m_fSizeY, 1.f });

	m_pBufferCom->Render_Buffer();



	scenemgr::Get_CurScene()->EndOrtho();


}



BOOL CMainUI::UI_Collision()
{


	GetCursorPos(&m_MousePoint);
	ScreenToClient(g_hWnd, &m_MousePoint);

	if (Engine::Collision_Mouse(_vec2{ (_float)m_MousePoint.x, (_float)m_MousePoint.y }, m_fX, m_fY, m_fSizeX, m_fSizeY))
	{
		if (!m_bSoundStart)
		{
			Engine::PlaySound_W(L"UI_Click_Mouse.mp3", SOUND_MOUSE, 0.5f);
			m_bSoundStart = true;
		}
			
		m_bColl = true;
		if (Engine::GetMouseState(DIM_LB) == eKEY_STATE::TAP)
		{
			Engine::PlaySound_W(L"UI_Click_Move.mp3", SOUND_MOUSE, 0.5f);
			return true;
		}
	}
	else
	{
		m_bSoundStart = false;
		m_bColl = false;
		return false;
	}

}

void CMainUI::Change_Scene()
{
	if (UI_Collision())
	{
		ChangeScene(CLogo::Create(m_pGraphicDev));
	}
}




HRESULT CMainUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(GetObjName().c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ GetObjName().c_str(), pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}



CMainUI* CMainUI::Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
	CMainUI* pInstance = new CMainUI(pGraphicDev, _State, _UI_Name);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMainUI::Free()
{
	__super::Free();
}
