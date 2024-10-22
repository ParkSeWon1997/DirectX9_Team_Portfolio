#include "ItemTool.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "stdafx.h"
#include <Slot.h>
#include "SlotMgr.h"
#include <Player.h>
#include <Mouse.h>
#include "CBonfire.h"
#include "CTent.h"
#include "CCookingPot.h"
#include <ItemBasic.h>

CItemTool::CItemTool(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos, UI_ITEM_TYPE eType, _bool bFood)
	: CItem(pGraphicDev, _strObjName), 
	m_bFood(bFood), 
	m_eItemType(eType), 
	m_eArmorSlotType(ARMOR_SLOT_END), 
	m_vPos(vPos), 
	m_bClick(false), 
	m_bColl(false),
	m_fSpeed(10.f)
{
	m_tItemInfo.ItemCount = 1;
}

CItemTool::CItemTool(const CItem& rhs)
	: CItem(rhs)
{
}

CItemTool::~CItemTool()
{
}

HRESULT CItemTool::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = m_vPos.x;
	m_fY = m_vPos.y;
	
	m_fSizeX = 15.f;
	m_fSizeY = 15.f;

	m_fPreX = m_fX;
	m_fPreY = m_fY;
	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.0f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));
	//m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(90.f));

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	Set_ObjState();

	return S_OK;
}

_int CItemTool::Update_GameObject(const _float& fTimeDelta)
{
	Coll_ItemBasic(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);

	Input_Mouse();

	//횃불일 경우
	if (m_strObjName == L"Torch" && m_iNum == m_eArmorSlotType)
	{
		m_tItemInfo.Durability -= fTimeDelta * 0.7f;

		if (m_tItemInfo.Durability < 0.f)
		{
			CSlotMgr::GetInstance()->Remove_ArmorItem(m_eArmorSlotType);
			dynamic_cast<CPlayer*>(scenemgr::Get_CurScene()->GetPlayerObject())->Set_Weapon_Equip(UNARMED);
			m_eArmorSlotType == ARMOR_SLOT_END;
		}
	}

	return 0;
}

void CItemTool::LateUpdate_GameObject()
{
	Move_Pos();

	__super::LateUpdate_GameObject();
}

void CItemTool::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	FAILED_CHECK_RETURN(SetUp_Material(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)), );

	m_pBufferCom->Render_Buffer();

	if ((m_eItemType == UI_ITEM_INVEN || m_eItemType == UI_ITEM_CREATE_NEED) && m_eArmorSlotType == ARMOR_SLOT_END)
	{
		_tchar strItemCount[32];

		_itow_s(m_tItemInfo.ItemCount, strItemCount, 10);
		Engine::Render_Font(L"Panel_Info", strItemCount, &_vec2(m_fX + 5.f, m_fY - 15.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	}
	else if (m_eItemType == UI_ITEM_INVEN && m_strObjName == L"Torch")
	{
		_tchar strItemCount[32];

		//_stprintf_l(strItemCount, sizeof(strItemCount), L"%d\%", m_tItemInfo.ItemCount);
		_itow_s(m_tItemInfo.Durability, strItemCount, 10);
		Engine::Render_Font(L"Panel_Info", strItemCount, &_vec2(m_fX - 10.f, m_fY - 15.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	}

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CItemTool::Input_Mouse()
{
	POINT tPt;
	GetCursorPos(&tPt);
	ScreenToClient(g_hWnd, &tPt);
	_vec2 vMousePos = _vec2(tPt.x, tPt.y);

	if (Engine::GetMouseState(DIM_LB) == eKEY_STATE::TAP && m_eItemType == UI_ITEM_INVEN) // 마우스 탭 하면 아이템이 따라오도록 수정 
	{
		if (!m_bClick) 
		{
			if (Engine::Collision_Mouse(vMousePos, m_fX, m_fY, m_fSizeX, m_fSizeY))
			{
				Engine::PlaySound_W(L"UI_Click_Move.mp3", SOUND_MOUSE, 1.f);
				m_bClick = true;
			}
			return;
		}

		_vec2 vItemPos = _vec2(m_fX, m_fY);

		if (Coll_Cook(vItemPos)) return;
		if (Coll_BonFire(vItemPos)) return;
		
		m_bClick = false;
		Engine::PlaySound_W(L"UI_Click_Move.mp3", SOUND_MOUSE, 1.f);
		m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f)); // 클릭을 놓는 순간 스케일값 돌아감
		vector<CSlot*> vecBox = CSlotMgr::GetInstance()->Get_BoxList(INVEN);

		Coll_Inven(vecBox, vItemPos);
		Coll_Armor(vecBox, vItemPos);
	}
	else if (Engine::GetMouseState(DIM_RB) == eKEY_STATE::TAP && m_eItemType == UI_ITEM_INVEN) // 마우스 오른쪽 클릭
	{
		if (Engine::Collision_Mouse(vMousePos, m_fX, m_fY, m_fSizeX, m_fSizeY))
		{
			Engine::PlaySound_W(L"UI_Click_Move.mp3", SOUND_MOUSE, 1.f);

			if (m_bFood) // 음식이먼 먹기 가능
			{
				Eat_Food();
			}

			else if (m_strObjName == L"BonFire") // 설치
			{
				auto vecMouse = scenemgr::Get_CurScene()->GetMouseObject();
				CMouse* pMouse = dynamic_cast<CMouse*>(vecMouse);
				pMouse->Set_Install(true);

				CGameObject* pBonfire = CBonfire::Create(m_pGraphicDev, true);
				pBonfire->Set_SlotNum(m_iNum);
				CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::OBJECT, pBonfire);
			}
			else if (m_strObjName == L"Tent") // 설치
			{
				auto vecMouse = scenemgr::Get_CurScene()->GetMouseObject();
				CMouse* pMouse = dynamic_cast<CMouse*> (vecMouse);
				pMouse->Set_Install(true);

				CGameObject* pTent = CTent::Create(m_pGraphicDev, true);
				pTent->Set_SlotNum(m_iNum);
				CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::OBJECT, pTent);
			}
			else if (m_strObjName == L"Cook") // 설치
			{
				auto vecMouse = scenemgr::Get_CurScene()->GetMouseObject();
				CMouse* pMouse = dynamic_cast<CMouse*> (vecMouse);
				pMouse->Set_Install(true);

				CGameObject* pTent = CCookingPot::Create(m_pGraphicDev, true);
				pTent->Set_SlotNum(m_iNum);
				CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::OBJECT, pTent);
			}
			else if (m_iNum >= 15) // 장착 칸이 장비창이면
			{
				for (int i = 0; i < INVENCNT; ++i)
				{
					CItem* pItem = CSlotMgr::GetInstance()->Get_InvenItem(i);
					if (pItem != nullptr) 
						continue;

					if (m_eArmorSlotType == HAND)
						Change_Player_Weapon();

					vector<CSlot*> vecBox = CSlotMgr::GetInstance()->Get_BoxList(INVEN);

					m_fX = vecBox[i]->Get_fX();
					m_fY = vecBox[i]->Get_fY();
					m_fPreX = m_fX;
					m_fPreY = m_fY;

					m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
					CSlotMgr::GetInstance()->Move_InvenItem(this, m_iNum, i);

					m_iNum = i;

					return;
				}
			}
			else if (m_eArmorSlotType != ARMOR_SLOT_END)
			{
				Change_Armor();
			}
		}
	}
	else if (m_eItemType == UI_ITEM_INVEN)
	{
		m_fX = m_fPreX;
		m_fY = m_fPreY;

		m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
	}
}

void CItemTool::Eat_Food()
{
	Engine::PlaySound_W(L"wilson_Eat_2.mp3", SOUND_PLAYER, 1.f);
	
	//플레이어 먹기 기능
	auto& vecPlayer = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::PLAYER)[0];
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(vecPlayer);
	

	if (m_strObjName == L"Berries")
	{
		pPlayer->Set_PlayerHangry(9.5f);
		//pPlayer->Set_PlayerHp(1.f);
	}
	else if (m_strObjName == L"Cooked_berries")
	{
		pPlayer->Set_PlayerHangry(12.5f);
		pPlayer->Set_PlayerHp(1.f);
	}
	else if (m_strObjName == L"Cooked_Meat_Monste")
	{
		pPlayer->Set_PlayerHangry(18.7f);
		pPlayer->Set_PlayerHp(-3.f);
		pPlayer->Set_PlayerMental(-15.f);
	}
	else if (m_strObjName == L"CookedMeat")
	{
		pPlayer->Set_PlayerHangry(25.f);
		pPlayer->Set_PlayerHp(3.f);
	}
	else if (m_strObjName == L"Meat_Monster")
	{
		pPlayer->Set_PlayerHangry(18.7f);
		pPlayer->Set_PlayerHp(-20.f);
		pPlayer->Set_PlayerMental(-15.f);
	}
	else if (m_strObjName == L"RawMeat")
	{
		pPlayer->Set_PlayerHangry(25.f);
		pPlayer->Set_PlayerHp(1.f);
	}
	else if (m_strObjName == L"Meatballs")
	{
		pPlayer->Set_PlayerHangry(62.f);
		pPlayer->Set_PlayerHp(3.f);
		pPlayer->Set_PlayerMental(5.f);
	}
	else if (m_strObjName == L"Wetgoop")
	{
		pPlayer->Set_PlayerHangry(0.f);
		pPlayer->Set_PlayerHp(0.f);
	}
	pPlayer->Set_Eat();
	CSlotMgr::GetInstance()->Remove_InvenItem(m_iNum);
}

void CItemTool::Move_Pos()
{
	if (!m_bClick) return;

	POINT tPt;
	GetCursorPos(&tPt);
	ScreenToClient(g_hWnd, &tPt);

	m_fX = tPt.x;
	m_fY = tPt.y;

	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX + 5.f, m_fSizeY + 5.f, 0.f));

}

_bool CItemTool::Coll_Cook(_vec2 vItemPos)
{
	//요리칸에 닿았다면
	vector<CSlot*> vecCookBox = CSlotMgr::GetInstance()->Get_BoxList(COOK);
	for (int i = 0; i < 4; ++i)
	{
		if (Engine::Collision_Mouse(vItemPos, vecCookBox[i]->Get_fX(), vecCookBox[i]->Get_fY(), vecCookBox[i]->Get_fSizeX(), vecCookBox[i]->Get_fSizeY()))
		{
			//아이템과 요리 아이템 충돌
			_vec3 vSlotPos = _vec3{ vecCookBox[i]->Get_fX(), vecCookBox[i]->Get_fY(), 0.f };
			CSlotMgr::GetInstance()->Set_CookItem(m_pGraphicDev, m_strObjName, vSlotPos, i);
			CSlotMgr::GetInstance()->Remove_InvenItem(m_iNum);

			return true;
		}
	}
	return false;
}

_bool CItemTool::Coll_BonFire(_vec2 vItemPos)
{
	auto& vecObjBox = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::OBJECT);
	for (auto& iter : vecObjBox)
	{
		if (iter->Get_State().strObjName == L"모닥불") // 모닥불 오브젝트와 충돌하면
		{
			_vec3 vRayPos, vRayDir;
			m_pCalculatorCom->Change_MouseMatrix(g_hWnd, _vec3{ m_fX, m_fY, 0.f }, &vRayPos, &vRayDir);
			_vec3 vObjPos;
			iter->GetTransForm()->Get_Info(INFO_POS, &vObjPos);

			if (Engine::Collision_Mouse_Object(vRayPos, vRayDir, vObjPos, iter->GetTransForm()->Get_Scale()))
			{
				if (m_bFood)
				{
					wstring strCookFood = L"";
					// 요리일 때, 구워진다.
					if (m_strObjName == L"Berries")
						strCookFood = L"Cooked_berries";
					else if (m_strObjName == L"RawMeat")
						strCookFood = L"CookedMeat";
					else if (m_strObjName == L"Meat_Monster")
						strCookFood = L"Cooked_Meat_Monster";
					else return false;

					_vec3 vSlotPos;
					if (CSlotMgr::GetInstance()->AddItem(m_pGraphicDev, strCookFood, &vSlotPos))
					{
						//월드 아이템 생성해서 이동하도록 구현 
						CSlotMgr::GetInstance()->Remove_InvenItem(m_iNum);
					}
				}
				else
				{
					dynamic_cast<CBonfire*>(iter)->AddFIre(1);
					CSlotMgr::GetInstance()->Remove_InvenItem(m_iNum);
				}
				return true;
			}
		}
	}

	return false;
}

_bool CItemTool::Coll_Inven(vector<CSlot*> vecBox, _vec2 vItemPos)
{
	for (int i = 0; i < INVENCNT; ++i)
	{
		if (Engine::Collision_Mouse(vItemPos, vecBox[i]->Get_fX(), vecBox[i]->Get_fY(), vecBox[i]->Get_fSizeX(), vecBox[i]->Get_fY()))
		{
			CItem* pItem = CSlotMgr::GetInstance()->Get_InvenItem(i);
			//아이템과 박스 충돌
			if (pItem != nullptr) // 충돌한 박스가 null이 아니라면
				break;

			m_fX = vecBox[i]->Get_fX();
			m_fY = vecBox[i]->Get_fY();
			m_fPreX = m_fX;
			m_fPreY = m_fY;

			m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
			CSlotMgr::GetInstance()->Move_InvenItem(this, m_iNum, i);
			m_iNum = i;

			return true;
		}
	}

	return false;
}

_bool CItemTool::Coll_Armor(vector<CSlot*> vecBox, _vec2 vItemPos)
{
	for (int i = 15; i < INVENCNT + 3; ++i) // 장착칸에 닿았다면
	{
		if (Engine::Collision_Mouse(vItemPos, vecBox[i]->Get_fX(), vecBox[i]->Get_fY(), vecBox[i]->Get_fSizeX(), vecBox[i]->Get_fY()))
		{
			//아이템과 박스 충돌
			// 아이템이 장착 아이템이 아니라면 
			// 충돌한 아이템이 알맞은 장착칸에 들어갔는지 확인
			if (m_eArmorSlotType == ARMOR_SLOT_END || m_eArmorSlotType != (ARMOR_SLOT_TYPE)i)
				return false;

			Change_Armor();
			return true;
		}
	}

	return false;
}

void CItemTool::Change_Armor()
{
	vector<CSlot*> vecBox = CSlotMgr::GetInstance()->Get_BoxList(INVEN);

	// 아이템 장착
	m_fX = vecBox[m_eArmorSlotType]->Get_fX();
	m_fY = vecBox[m_eArmorSlotType]->Get_fY();

	if (m_eArmorSlotType == HAND)
		Change_Player_Weapon();

	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
	
	if (CSlotMgr::GetInstance()->Get_ArmorItem(m_eArmorSlotType) != nullptr) // 충돌한 박스가 null이 아니라면
		CSlotMgr::GetInstance()->Change_ArmorItem(this, m_eArmorSlotType, m_iNum, _vec3{ m_fPreX , m_fPreY, 0.f });
	else
		CSlotMgr::GetInstance()->Set_ArmorItem(m_eArmorSlotType, this, m_iNum);

	m_fPreX = m_fX;
	m_fPreY = m_fY;

	m_iNum = m_eArmorSlotType;
}

void CItemTool::Change_Player_Weapon()
{
	WEAPON eWeapon = WEAPON_END;
	if (m_Stat.strObjName == L"도끼")
		eWeapon = AXE;
	else if (m_Stat.strObjName == L"망치")
		eWeapon = HAMMER;
	else if (m_Stat.strObjName == L"곡괭이")
		eWeapon = PICK;
	else if (m_Stat.strObjName == L"전투창")
		eWeapon = SPEAR;
	else if (m_Stat.strObjName == L"횃불")
		eWeapon = TORCH;

	dynamic_cast<CPlayer*>(scenemgr::Get_CurScene()->GetPlayerObject())->Set_Weapon_Equip(eWeapon);
}

void CItemTool::Coll_ItemBasic(const float& fTimeDelta)
{
	if (m_bColl) // 충돌인 상태이면
	{
		static _bool bSizeUp(true);
		
		if (bSizeUp)
		{
			if (m_fSizeX < 20.f)
				m_fSizeX +=  fTimeDelta * m_fSpeed;
			if (m_fSizeY < 20.f)
				m_fSizeY +=  fTimeDelta * m_fSpeed;

			if (m_fSizeX >= 20.f || m_fSizeY >= 20.f)
				bSizeUp = false;
		}
		else
		{
			if (m_fSizeX > 15.f)
				m_fSizeX -=  fTimeDelta * m_fSpeed;
			if (m_fSizeY > 15.f)
				m_fSizeY -=  fTimeDelta * m_fSpeed;

			if (m_fSizeX <= 15.f || m_fSizeY <= 15.f)
			{
				m_fSizeX = 15.f;
				m_fSizeY = 15.f;
				m_bColl = false;
				bSizeUp = true;
			}
		}

		m_pTransForm->Set_Scale(_vec3{ m_fSizeX, m_fSizeY, 0.f });

		return;
	}
	auto& vecItem = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::ITEM);

	for (auto& iter : vecItem)
	{
		CItemBasic* pItem = dynamic_cast<CItemBasic*>(iter);
		if (!pItem->Get_ChangeRender() || pItem->GetObjName() != m_strObjName)
			continue;

		if (Engine::Collision_Mouse(_vec2{m_fX, m_fY}, pItem->Get_fX(), pItem->Get_fY(), pItem->Get_fSizeX(), pItem->Get_fSizeY()))
		{
			m_bColl = true;
			//DeleteObject(pItem);
			return;
		}
	}
}

CItem* CItemTool::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos, UI_ITEM_TYPE eType, _bool bFood)
{
	CItem* pInstance = new CItemTool(pGraphicDev, _strObjName, vPos, eType, bFood);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CItemTool::Free()
{
	__super::Free();
}
