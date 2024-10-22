#include "ButtonUI.h"
#include "Export_System.h"
#include "stdafx.h"
#include "SlotMgr.h"
#include <Mouse.h>
#include <Cook.h>
#include <CCookingPot.h>
#include <ItemBasic.h>
#include <Player.h>

CButtonUI::CButtonUI(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec2 vSize, _bool bFood)
    : CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_fX(vPos.x), m_fY(vPos.y), m_bColl(false), m_fSizeX(vSize.x), m_fSizeY(vSize.y), m_bFood(bFood)
{
}

CButtonUI::CButtonUI(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, CREATEINFO tCreateInfo, _vec2 vSize)
	: CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_fX(vPos.x), m_fY(vPos.y), m_bColl(false), m_fSizeX(vSize.x), m_fSizeY(vSize.y), m_tCreateInfo(tCreateInfo), m_bFood(false)
{

}

CButtonUI::CButtonUI(const CButtonUI& rhs)
	: CGameObject(rhs), m_fX(rhs.m_fX), m_fY(rhs.m_fY), m_tCreateInfo(rhs.m_tCreateInfo)
{
}

CButtonUI::~CButtonUI()
{
}

HRESULT CButtonUI::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransForm->Set_Pos(_vec3((m_fX - WINCX * 0.5f), -m_fY + WINCY * 0.5f, 0.f));
    m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.1f));

    D3DXMatrixIdentity(&m_ViewMatrix);
    D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}

_int CButtonUI::Update_GameObject(const _float& fTimeDelta)
{
    __super::Update_GameObject(fTimeDelta);

    Input_Mouse();

	return 0;
}

void CButtonUI::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
}

void CButtonUI::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(m_bColl);

    m_pBufferCom->Render_Buffer();

    if (m_bFood)
        Engine::Render_Font(L"Button_Cook_Make", L"요리", &_vec2(m_fX - 12.f, m_fY - 8.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
    else
        Engine::Render_Font(L"Button_Make", L"제작", &_vec2(m_fX - 20.f, m_fY - 8.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CButtonUI::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Make_Button"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Make_Button", pComponent });

    pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CButtonUI::Input_Mouse()
{
    POINT tPt;
    GetCursorPos(&tPt);
    ScreenToClient(g_hWnd, &tPt);
    _vec2 vMousePos = _vec2(tPt.x, tPt.y);

    m_bColl = Engine::Collision_Mouse(vMousePos, m_fX, m_fY, m_fSizeX, m_fSizeY);

    //아이템 제작 
    if (m_bColl && !m_bFood) // 제작 탭에서 아이템 제작
    {
        if (Engine::GetMouseState(DIM_LB) == eKEY_STATE::TAP)
        {
            _vec3 vSlotPos;
            // 생산 로직 구현 
            _int iNum = m_tCreateInfo.iInfoCount;
            _bool bCheck(false);

            for (int i = 0; i < m_tCreateInfo.iInfoCount; ++i)
            {
                bCheck = CSlotMgr::GetInstance()->Check_InvenItemCount(m_tCreateInfo.tItemInfo[i].strItemName, m_tCreateInfo.tItemInfo[i].iCount);

                if (!bCheck) break;
            }
           
            if (bCheck) // 모두 true인 경우에 생성 가능 
            {
                for (int i= 0; i< m_tCreateInfo.iInfoCount; ++i)
                {
                    CSlotMgr::GetInstance()->Remove_CreateItem(m_tCreateInfo.tItemInfo[i].strItemName, m_tCreateInfo.tItemInfo[i].iCount);
                }

                if (CSlotMgr::GetInstance()->Check_AddItem(m_pGraphicDev, m_tCreateInfo.strKeyName, &vSlotPos)) // 아이템 제작 시 애니메이션
                {
                    Engine::PlaySound_W(L"UI_Collect_Res.mp3", SOUND_CREATE, 1.f);
                    _vec3 vPos;
                    scenemgr::Get_CurScene()->GetPlayerObject()->GetTransForm()->Get_Info(INFO_POS, &vPos);
                    CItem* pItem = CItemBasic::Create(m_pGraphicDev, m_tCreateInfo.strKeyName);
                    CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::ITEM, pItem);
                    pItem->GetTransForm()->Set_Pos(vPos);
                    dynamic_cast<CItemBasic*>(pItem)->Pickup_Item(vSlotPos);
                }
            }
            else // 제작 실패 시 
            {
                dynamic_cast<CPlayer*>(scenemgr::Get_CurScene()->GetPlayerObject())->Set_Dialog();
            }
        }
        else if (Engine::GetMouseState(DIM_RB) == eKEY_STATE::TAP) //치트
        {
            _vec3 vSlotPos;

            if (CSlotMgr::GetInstance()->Check_AddItem(m_pGraphicDev, m_tCreateInfo.strKeyName, &vSlotPos)) // 아이템 제작 시 애니메이션
            {
                Engine::PlaySound_W(L"UI_Collect_Res.mp3", SOUND_CREATE, 1.f);
                _vec3 vPos;
                scenemgr::Get_CurScene()->GetPlayerObject()->GetTransForm()->Get_Info(INFO_POS, &vPos);
                CItem* pItem = CItemBasic::Create(m_pGraphicDev, m_tCreateInfo.strKeyName);
                CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::ITEM, pItem);
                pItem->GetTransForm()->Set_Pos(vPos);
                dynamic_cast<CItemBasic*>(pItem)->Pickup_Item(vSlotPos);
            }
        }
    }
    else if (m_bColl && m_bFood) // 요리 탭에서 요리 제작, 요리 끝나면 아이템 다 사라지도록 
    {

        if (Engine::GetMouseState(DIM_LB) == eKEY_STATE::TAP)
        {
            //if (CSlotMgr::GetInstance()->AddItem(m_pGraphicDev, ))

            auto& vecUI = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI);
            for (auto& iter : vecUI)
            {
                if (iter->Get_State().strObjName == L"요리 도구")
                {
                    dynamic_cast<CCook*>(iter)->IsShow(false);
                }
            }

            auto& vecObject = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::OBJECT);
            for (auto& iter : vecObject)
            {
                if (iter->Get_State().strObjName == L"요리 솥")
                {

                    CCookingPot* pCookingPot = dynamic_cast<CCookingPot*>(iter);
                    pCookingPot->Set_Cooking(true);
                    pCookingPot->Set_Success(CSlotMgr::GetInstance()->Start_Cook());
                }
            }

            CSlotMgr::GetInstance()->Remove_CookItem();
        }
    }
}

CButtonUI* CButtonUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec2 vSize, _bool bFood)
{
    CButtonUI* pInstance = new CButtonUI(pGraphicDev, vPos, vSize, bFood);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CButtonUI* CButtonUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, CREATEINFO tCreateInfo, _vec2 vSize)
{
    CButtonUI* pInstance = new CButtonUI(pGraphicDev, vPos, tCreateInfo, vSize);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);

        return nullptr;
    }

	return pInstance;
}

void CButtonUI::Free()
{
    __super::Free();
}
