#include "..\Include\stdafx.h"

#include "ExplainPanel.h"

#include "Export_Utility.h"
#include "Export_System.h"

#include "UIMgr.h"
#include <ItemTool.h>
#include "InvenSlot.h"

CExplainPanel::CExplainPanel(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, wstring strItemKey)
    : CUI(pGraphicDev), 
    m_strItemKey(strItemKey),
    m_pButton(nullptr),
    m_vPos(vPos),
    m_bSlideBoxColl(false)
{
    ZeroMemory(&m_tCreateInfo, sizeof(m_tCreateInfo));

    for (int i = 0; i < 3; ++i)
        m_pItem[i] = nullptr;
}

CExplainPanel::CExplainPanel(const CExplainPanel& rhs)
    : CUI(rhs),
    m_strItemKey(rhs.m_strItemKey),
    m_pButton(rhs.m_pButton)
{
}

CExplainPanel::~CExplainPanel()
{
}

HRESULT CExplainPanel::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    ZeroMemory(&m_pItem, sizeof(m_pItem));

    m_tCreateInfo = CUIMgr::GetInstance()->Get_CreateInfo(m_strItemKey);

    m_fX = m_vPos.x;
    m_fY = m_vPos.y;

    for (int i = 0; i < m_tCreateInfo.iInfoCount; ++i)
    {
        _vec3 vPos = {};
        if (m_tCreateInfo.iInfoCount == 1)
            vPos = _vec3(m_fX + 12.f, m_fY + 8.f, 0.f);
        else if (m_tCreateInfo.iInfoCount == 2)
            vPos = _vec3(m_fX + 3.f + (30.f * i), m_fY + 8.f, 0.f);
        else if (m_tCreateInfo.iInfoCount == 3)
            vPos = _vec3(m_fX - 13.f + (30.f * i), m_fY + 8.f, 0.f);

        m_pItem[i] = CInvenSlot::Create(m_pGraphicDev, vPos, i, INVEN);
        CItem* pItem = CItemTool::Create(m_pGraphicDev, m_tCreateInfo.tItemInfo[i].strItemName, vPos, UI_ITEM_CREATE_NEED);
        pItem->Set_ItemCount(m_tCreateInfo.tItemInfo[i].iCount);
        m_pItem[i]->Set_Item(pItem);
    }

    m_pButton = CButtonUI::Create(m_pGraphicDev, _vec3(m_fX + 15.f, m_fY + 50.f, 0.f), m_tCreateInfo);

    m_fSizeX = 90.f;
    m_fSizeY = 90.f;

    __super::Ready_GameObject();

    return S_OK;
}

_int CExplainPanel::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bShow && !m_bSlideBoxColl)
        return 0;

    __super::Update_GameObject(fTimeDelta);

    POINT tPt;
    GetCursorPos(&tPt);
    ScreenToClient(g_hWnd, &tPt);
    _vec2 vMousePos = _vec2(tPt.x, tPt.y);
    if (Engine::Collision_Mouse(vMousePos, m_fX, m_fY, m_fSizeX, m_fSizeY))
    {
        m_bSlideBoxColl = false;
        m_bShow = true;
    }
    else
        m_bShow = false;

    for (int i = 0; i < m_tCreateInfo.iInfoCount; ++i)
        m_pItem[i]->Update_GameObject(fTimeDelta);

    m_pButton->Update_GameObject(fTimeDelta);

    return 0;
}

void CExplainPanel::LateUpdate_GameObject()
{
    if (!m_bShow && !m_bSlideBoxColl)
        return;

    for (int i = 0; i < m_tCreateInfo.iInfoCount; ++i)
        m_pItem[i]->LateUpdate_GameObject();

    m_pButton->LateUpdate_GameObject();
    __super::LateUpdate_GameObject();
}

void CExplainPanel::Render_GameObject()
{
    if (!m_bShow && !m_bSlideBoxColl)
		return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();


    //PSW 길이에 맞춰서 띄우기용 -------------------
    wstring wStringName(m_tCreateInfo.strName);
    size_t sNameLen = wStringName.length();
    //띄어쓰기용
    int spacingName =0;
    if (sNameLen == 3 || sNameLen == 4)
        spacingName = 10;
    else if (sNameLen == 5)
        spacingName = 15;
    else if (sNameLen >5 )
        spacingName = 22;
    else 
        spacingName = 0;
    //PSW 길이에 맞춰서 띄우기용 -------------------

    if (sNameLen == 1)
        Engine::Render_Font(L"Panel_Title", m_tCreateInfo.strName, &_vec2(m_fX + 6.f, m_fY - 70.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    else
        Engine::Render_Font(L"Panel_Title", m_tCreateInfo.strName, &_vec2(m_fX- (sNameLen + spacingName), m_fY - 70.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    //PSW 길이에 맞춰서 띄우기용 -------------------
    wstring wStringInfo(m_tCreateInfo.strInfo);
    size_t sInfoLen = wStringInfo.length();
    //PSW 길이에 맞춰서 띄우기용 -------------------
  
    Engine::Render_Font(L"Panel_Info", m_tCreateInfo.strInfo, &_vec2(m_fX- (sInfoLen + 10.f), m_fY - 35.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

    //아이템 넣기
    for (int i = 0; i < m_tCreateInfo.iInfoCount; ++i)
        m_pItem[i]->Render_GameObject();

    m_pButton->Render_GameObject();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CExplainPanel* CExplainPanel::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, wstring strItemKey)
{
    CExplainPanel* pInstance = new CExplainPanel(pGraphicDev, vPos, strItemKey);
    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("UI Create Failed");
        return nullptr;
    }

    return pInstance;
}

HRESULT CExplainPanel::Add_Component()
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

void CExplainPanel::Free()
{
    for (int i = 0; i < m_tCreateInfo.iInfoCount; ++i)
        Safe_Release(m_pItem[i]);
    Safe_Release(m_pButton);

    __super::Free();
}
