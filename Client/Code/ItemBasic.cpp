#include "ItemBasic.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "stdafx.h"
#include "SlotMgr.h"
#include <Terrain.h>

CItemBasic::CItemBasic(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev), m_bChangeRander(false)
{
}

CItemBasic::CItemBasic(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	: CItem(pGraphicDev, _strObjName)
{
}

CItemBasic::CItemBasic(const CItem& rhs)
	: CItem(rhs)
{
}

CItemBasic::~CItemBasic()
{
}

void CItemBasic::Pickup_Item(_vec3 vSlotPos)
{
	m_vPos = m_pTransForm->Get_Pos();
	m_pCalculatorCom->Change_OnObjectMatrix(g_hWnd, &m_vPos);
	m_bChangeRander = true;

	m_vSlotPos = vSlotPos;

	m_fX = m_vPos.x;
	m_fY = m_vPos.y;

	m_fSizeX = 15.f;
	m_fSizeY = 15.f;
	
	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.0f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));
	//m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(180.f));

	m_fViewZ = -1.f;
	
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);
}

HRESULT CItemBasic::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Set_Scale(_vec3{ 0.3f, 0.5f, 0.3f });

	Set_ObjState();
	m_vUp = { 0,1,0 };
	int randomValue = rand() % 360;
	int randomValue2 = rand() % 360;
	// 부호를 무작위로 선택 (-1 또는 1)
	int sign = (rand() % 2 == 0) ? 1 : -1;
	int sign2 = (rand() % 2 == 0) ? 1 : -1;
	// 랜덤값에 부호를 적용
	int result = randomValue * sign;
	int result2 = randomValue2 * sign2;
	m_vDir = { (float)result,0.f,(float)result2 };
	D3DXVec3Normalize(&m_vDir, &m_vDir);




    return S_OK;
}

_int CItemBasic::Update_GameObject(const _float& fTimeDelta)
{

	CGameObject::Update_GameObject(fTimeDelta);

	if (m_bChangeRander)
	{
		if (m_vSlotPos.y - m_fY <= 2.f)
		{
			_vec3 vSlotPos = {};
			CSlotMgr::GetInstance()->AddItem(m_pGraphicDev, m_strObjName, &vSlotPos);
			//사운드 출력
			PickUp_Sound();
			DeleteObject(this);
			//return 0x80000000;
		}

		// 아이템 먹엇을때 슬롯쪽으로 이동
		m_fX += (m_vSlotPos.x - m_fX) * 12.f * fTimeDelta;
		m_fY += (m_vSlotPos.y - m_fY) * 12.f * fTimeDelta;

		m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.1f));
		//m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));
	}

	MousePicking();

	if (m_bIsCreateByObject)
	{
		DropMotion(fTimeDelta);
	}

	renderer::Add_RenderGroup(m_bChangeRander ? RENDER_UI : RENDER_ALPHA, this);

	return 0;
}

void CItemBasic::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	if (!m_bChangeRander)
	{
		m_pTransForm->BillBoard();
		_vec3	vPos;
		m_pTransForm->Get_Info(INFO_POS, &vPos);
		__super::Compute_ViewZ(&vPos);
	}
}

void CItemBasic::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

	if (m_bChangeRander)
	{
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	}

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (!m_bChangeRander) m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);
	if (!m_bChangeRander) FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	if (!m_bChangeRander) m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	scenemgr::Get_CurScene()->EndOrtho();
}

void CItemBasic::DropMotion(const _float& fTimeDelta)
{

	const float Gravity = 9.8f;
	_vec3 vUp;

	m_pTransForm->Move_Pos(&m_vDir, 0.7f, fTimeDelta);
	m_pTransForm->Move_Pos(&m_vUp, 10.f, fTimeDelta);
	

	m_vUp.y-= 2.f * fTimeDelta;

	if (m_pTransForm->Get_Pos().y < 0)
		m_bIsCreateByObject = false;
	
}

void CItemBasic::PickUp_Sound()
{
	_tchar* pSoundFileName;
	if (m_strObjName == L"Log" || m_strObjName == L"Twigs" || m_strObjName == L"Charcoal" || m_strObjName == L"LogSuit")
		pSoundFileName = L"Pickup_Wood.mp3";
	else if (m_strObjName == L"CutStone" || m_strObjName == L"Rope")
		pSoundFileName = L"Pichup_Grass.mp3";
	else if (m_strObjName == L"Rocks_0" || m_strObjName == L"CutStone")
		pSoundFileName = L"Pickup_Rock.mp3";
	else if (m_strObjName == L"FireSton" || m_strObjName == L"Ax" || m_strObjName == L"Lance" || m_strObjName == L"Hammer" || m_strObjName == L"Pickaxe")
		pSoundFileName = L"Pickup_Metal.mp3";
	else
		pSoundFileName = L"Pickup_Gem.mp3";
		
	Engine::PlaySound_W(pSoundFileName, SOUND_UI, 1.f);
}


CItem* CItemBasic::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _szName)
{
	CItem* pInstance = new CItemBasic(pGraphicDev, _szName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CItemBasic::Free()
{
	__super::Free();
}
