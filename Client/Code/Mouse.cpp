#include "Mouse.h"
#include "stdafx.h"
#include "Export_System.h"
#include "SlotMgr.h"
#include <ItemTool.h>
#include <CreateUI.h>

CMouse::CMouse(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_bColl(false), m_eGroupType(eOBJECT_GROUPTYPE::END), m_bInstall(false)
{
	ZeroMemory(&m_eObjState, sizeof(m_eObjState));
}

CMouse::~CMouse()
{
}

HRESULT CMouse::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//ShowCursor(NULL);
	//m_pGraphicDev->ShowCursor(TRUE);

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}

_int CMouse::Update_GameObject(const _float& fTimeDelta)
{
	POINT tPt;
	GetCursorPos(&tPt);
	ScreenToClient(g_hWnd, &tPt);
	m_vMousePos = _vec3(tPt.x, tPt.y, 0.1f);

	__super::Update_GameObject(fTimeDelta);

	Check_Coll();
	
	renderer::Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CMouse::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMouse::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (m_bColl && !m_bInstall)
	{
		// 이름
		Engine::Render_Font(L"Mouse_Title", m_eObjState.strObjName, &_vec2(m_vMousePos.x + 20.f, m_vMousePos.y - 20.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		if (m_eGroupType == eOBJECT_GROUPTYPE::MONSTER)
		{
			_tchar strInfo[32];
			Engine::Render_Font(L"Mouse_Sub", L"생명력 : ", &_vec2(m_vMousePos.x + 20.f, m_vMousePos.y + 5.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			_itow_s(m_eObjState.fHP, strInfo, 10);
			Engine::Render_Font(L"Mouse_Sub", strInfo, &_vec2(m_vMousePos.x + 60.f, m_vMousePos.y + 5.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			Engine::Render_Font(L"Mouse_Sub", L"/", &_vec2(m_vMousePos.x + 80.f, m_vMousePos.y + 5.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			_itow_s(m_eObjState.fMxHP, strInfo, 10);
			Engine::Render_Font(L"Mouse_Sub", strInfo, &_vec2(m_vMousePos.x + 90.f, m_vMousePos.y + 5.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			//공격력
			Engine::Render_Font(L"Mouse_Sub", L"공격력 : ", &_vec2(m_vMousePos.x + 20.f, m_vMousePos.y + 25.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			_itow_s(m_eObjState.fATK, strInfo, 10);
			Engine::Render_Font(L"Mouse_Sub", strInfo, &_vec2(m_vMousePos.x + 60.f, m_vMousePos.y + 25.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			
		}
	}

	scenemgr::Get_CurScene()->EndOrtho();
}

HRESULT CMouse::Add_Component()
{
	CComponent* pComponent = nullptr;

	//VIBUFFER
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	////TEXTURE
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(GetObjName().c_str()));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ GetObjName().c_str(), pComponent });

	//TransForm
	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	//m_pTransForm->Set_Scale(_vec3(0.5f, 0.3f, 0.5f));

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}

void CMouse::Check_Coll()
{
	//몬스터
	auto& vecMonster = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::MONSTER);
	_vec3 vMouseScale, vMonsterPos, vMonsterScale;

	CItem* pInven[INVENCNT];
	CSlotMgr::GetInstance()->Get_Inven(pInven);
	vector<CSlot*> vecSlot = CSlotMgr::GetInstance()->Get_BoxList(INVEN);
	//인벤 아이템 
	for (size_t i = 0; i < INVENCNT; ++i)
	{
		CSlot* pSlot = vecSlot[i];
		if (Engine::Collision_Mouse(_vec2(m_vMousePos.x, m_vMousePos.y), pSlot->Get_fX(), pSlot->Get_fY(), pSlot->Get_fSizeX(), pSlot->Get_fSizeY()))
		{
			//Engine::PlaySound_W(L"Mouse_Click_Object.mp3", SOUND_MOUSE, 10.f);
			if (pInven[i]) // 아이템 충돌 시 
			{
				m_bColl = true;
				m_eGroupType = eOBJECT_GROUPTYPE::ITEM;
				m_eObjState = pInven[i]->Get_State();
				return;
			}

			m_bColl = false;
			return;
		}
		else
		{
			m_bColl = false;
			m_eGroupType = eOBJECT_GROUPTYPE::END;
		}
	}

	//UI와 충돌
	auto& vecUI = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI);
	
	for (auto& iter : vecUI)
	{
		vMonsterScale = iter->GetTransForm()->Get_Scale();
		CUI* pUI = dynamic_cast<CUI*>(iter);
		if (Engine::Collision_Mouse(_vec2(m_vMousePos.x, m_vMousePos.y), pUI->Get_fX(), pUI->Get_fY(), pUI->Get_fSizeX(), pUI->Get_fSizeY()))
		{
			m_bColl = false;
			return;
		}
	}

	//auto& vecCreateUI = vecUI[0]; //createUI;
	//auto& vecSlideUI = dynamic_cast<CCreateUI*>(vecCreateUI)->Get_Slide();

	//for (auto& iter : vecSlideUI)
	//{
	//	iter->GetTransForm()->Get_Info(INFO_POS, &vMonsterPos);
	//	vMonsterScale = iter->GetTransForm()->Get_Scale();
	//	CUI* pUI = dynamic_cast<CUI*>(iter);
	//	if (Engine::Collision_Mouse(_vec2(m_vMousePos.x, m_vMousePos.y), pUI->Get_fX(), pUI->Get_fY(), pUI->Get_fSizeX(), pUI->Get_fSizeY()))
	//	{
	//		m_bColl = false;
	//		return;
	//	}
	//}

	//마우스 좌표 변환
	m_pCalculatorCom->Change_MouseMatrix(g_hWnd, m_vMousePos, &m_vRayPos, &m_vRayDir);
	vMouseScale = m_pTransForm->Get_Scale();

	for (auto& iter : vecMonster)
	{
		iter->GetTransForm()->Get_Info(INFO_POS, &vMonsterPos);
		vMonsterScale = iter->GetTransForm()->Get_Scale();

		if (Engine::Collision_Mouse_Object(m_vRayPos, m_vRayDir, vMonsterPos, vMonsterScale))
		{
			m_bColl = true;
			m_eGroupType = eOBJECT_GROUPTYPE::MONSTER;
			m_eObjState = iter->Get_State();
			return;
		}
		else
		{
			m_bColl = false;
			m_eGroupType = eOBJECT_GROUPTYPE::END;
		}
	}

	//설치 오브젝트들
	auto& vecObj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::OBJECT);

	for (auto& iter : vecObj)
	{
		iter->GetTransForm()->Get_Info(INFO_POS, &vMonsterPos);
		vMonsterScale = iter->GetTransForm()->Get_Scale();

		vMonsterScale = _vec3{ vMonsterScale.x - 1.f, vMonsterScale.y - 1.f, vMonsterScale.z - 1.f };

		if (Engine::Collision_Mouse_Object(m_vRayPos, m_vRayDir, vMonsterPos, vMonsterScale) && iter->GetObjName() != L"FireFlies")
		{
			m_bColl = true;
			m_eGroupType = eOBJECT_GROUPTYPE::OBJECT;
			m_eObjState = iter->Get_State();
			iter->Set_Shader(true);
			return;
		}
		else
		{
			m_bColl = false;
			m_eGroupType = eOBJECT_GROUPTYPE::END;
			iter->Set_Shader(false);
		}
	}

	//지형지물
	auto& vecResObj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::RESOURCE_OBJECT);

	for (auto& iter : vecResObj)
	{
		iter->GetTransForm()->Get_Info(INFO_POS, &vMonsterPos);
		vMonsterScale = iter->GetTransForm()->Get_Scale();

		if (iter->Get_State().strObjName == L"나무")
			vMonsterScale = _vec3{ vMonsterScale.x - 3.f, vMonsterScale.y - 3.f, vMonsterScale.z - 3.f };

		if (Engine::Collision_Mouse_Object(m_vRayPos, m_vRayDir, vMonsterPos, vMonsterScale))
		{
			m_bColl = true;
			m_eGroupType = eOBJECT_GROUPTYPE::RESOURCE_OBJECT;
			m_eObjState = iter->Get_State();
			return;
		}
		else
		{
			m_bColl = false;
			m_eGroupType = eOBJECT_GROUPTYPE::END;
		}
	}

	auto& vecItem = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::ITEM);
	//아이템
	for (auto& iter : vecItem)
	{
		iter->GetTransForm()->Get_Info(INFO_POS, &vMonsterPos);
		vMonsterScale = iter->GetTransForm()->Get_Scale();

		if (Engine::Collision_Mouse_Object(m_vRayPos, m_vRayDir, vMonsterPos, vMonsterScale))
		{
			m_bColl = true;
			m_eGroupType = eOBJECT_GROUPTYPE::RESOURCE_OBJECT;
			m_eObjState = iter->Get_State();
			return;
		}
		else
		{
			m_bColl = false;
			m_eGroupType = eOBJECT_GROUPTYPE::END;
		}
	}
}

CMouse* CMouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMouse* pInstance = new CMouse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMouse::Free()
{
	__super::Free();
}
