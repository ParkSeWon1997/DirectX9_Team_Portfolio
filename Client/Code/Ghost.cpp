#include "Ghost.h"
#include "stdafx.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"

CGhost::CGhost(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
	:CEffect(pGraphicDev, _vPos), m_eCurState(APPEAR), m_ePreState(STATE_END), m_bAppear(false)
{
}

CGhost::CGhost(const CGhost& rhs)
	:CEffect(rhs)
{
}

CGhost::~CGhost()
{
}

HRESULT CGhost::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    m_fFrameEnd = 12.f;
    return S_OK;
}

_int CGhost::Update_GameObject(const _float& fTimeDelta)
{

    m_fFrame += m_fFrameEnd * fTimeDelta;
    
    Appear();
    State_Change();
    Set_Scale();
    CGameObject::Update_GameObject(fTimeDelta);
    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    return 0;
}

void CGhost::LateUpdate_GameObject()
{
    CGameObject::LateUpdate_GameObject();
    m_pTransForm->BillBoard();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);
}

void CGhost::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


    FAILED_CHECK_RETURN(SetUp_Material(), );
    m_pTextureCom[m_ePreState]->Set_Texture((_uint)m_fFrame);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CGhost::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

#pragma region TEXCOM

    pComponent = m_pTextureCom[APPEAR] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Ghost_Appear"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Ghost_Appear", pComponent });

    pComponent = m_pTextureCom[IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Ghost_IDLE"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Ghost_IDLE", pComponent });

#pragma endregion TEXCOM

    pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    m_pTransForm->Set_Scale({ 1.f, 1.f, 1.f });

    pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });


    return S_OK;
}

void CGhost::State_Change()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case APPEAR:
            m_fFrameEnd = 12.f;
            break;
        case IDLE:
            m_fFrameEnd = 26.f;
            break;
        }
        m_fFrame = 0.f;
        m_ePreState = m_eCurState;
    }
}

void CGhost::Appear()
{
    if (m_ePreState == APPEAR && !m_bAppear)
    {
        if (m_fFrameEnd < m_fFrame)
        {
            m_eCurState = IDLE;
            m_bAppear = true;
        }
    }
   
    if (m_fFrameEnd < m_fFrame)
        m_fFrame = 0.f;
}

void CGhost::Set_Scale()
{

    if(m_ePreState == APPEAR)
        m_pTransForm->m_vScale = { 1.f, 1.f, 1.f };
    else if(m_ePreState == IDLE)
        m_pTransForm->m_vScale = { 2.f, 4.f, 2.f };
}

CGhost* CGhost::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    CGhost* pInstance = new CGhost(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Ghost Create Failed");
        return nullptr;
    }
    return pInstance;
}

void CGhost::Free()
{
    __super::Free();
}
