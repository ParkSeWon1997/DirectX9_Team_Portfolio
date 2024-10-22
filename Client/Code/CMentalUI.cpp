#include"CMentalUI.h"
#include "Export_Utility.h"
#include "Export_System.h"

#include "Scene.h"
#include <Player.h>


CMentalUI::CMentalUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CStateUI(pGraphicDev)
{
}

CMentalUI::~CMentalUI()
{
}

HRESULT CMentalUI::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_fX = WINCX - 30.f;
	m_fY = 110.f;

	m_fSizeX = 30.f;
	m_fSizeY = 30.f;

	__super::Ready_GameObject();

	m_frameEnd = 20.f;

	return S_OK;
}

_int CMentalUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CMentalUI::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	Check_State();
}

void CMentalUI::Render_GameObject()
{
	__super::Render_GameObject();

	_tchar strMental[32];
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(scenemgr::Get_CurScene()->GetPlayerObject());
	_itow_s(pPlayer->Get_PlayerMental(), strMental, 10);

	Engine::Render_Font(L"Mouse_Sub", strMental, &_vec2(m_fX - 11.f, m_fY), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CMentalUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Mental"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Mental", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CMentalUI::Check_State()
{
	CGameObject* pP = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::PLAYER)[0];
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pP);

	_float fMental = pPlayer->Get_PlayerMental();
	_float fMaxMental = pPlayer->Get_PlayerMaxMental();

	_float fFrame = (fMaxMental - fMental) / 10.f;

	if (fFrame < m_frameEnd)
		m_frame = fFrame;
}

CMentalUI* CMentalUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMentalUI* pInstance = new CMentalUI(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}


void CMentalUI::Free()
{
	__super::Free();
}
