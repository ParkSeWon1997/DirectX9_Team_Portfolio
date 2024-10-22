#include "SnowSplash.h"
#include "stdafx.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include "Monster.h"
#include "ResObject.h"

SnowSplash::SnowSplash(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
	:CEffect(pGraphicDev, _vPos)
{
}

SnowSplash::SnowSplash(const SnowSplash& rhs)
	:CEffect(rhs)
{
}

SnowSplash::~SnowSplash()
{
}

HRESULT SnowSplash::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    m_fFrameEnd = 13.f;
    return S_OK;
}

_int SnowSplash::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bFrameStop)
    {
        m_fFrame += m_fFrameEnd * fTimeDelta;
    }
    _int iResult = Appear();
    State_Change();
  
    CGameObject::Update_GameObject(fTimeDelta);
    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    return iResult;
}

void SnowSplash::LateUpdate_GameObject()
{
    CGameObject::LateUpdate_GameObject();
    m_pTransForm->BillBoard();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);
}

void SnowSplash::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


    FAILED_CHECK_RETURN(SetUp_Material(), );

    m_pTextureCom->Set_Texture((_uint)m_fFrame);
    m_pBufferCom->Render_Buffer();
  


    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT SnowSplash::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });


#pragma region TEXCOM

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Snow_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Deer_Snow_Effect", pComponent });


#pragma endregion TEXCOM

    pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    m_pTransForm->Set_Scale({ 2.f, 2.f, 2.f });

    pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });


    return S_OK;
}

void SnowSplash::State_Change()
{
 
}

_int SnowSplash::Appear()
{

    if (m_fFrameEnd-1 < m_fFrame)
    {
        m_fFrame = m_fFrameEnd - 1;
        m_bFrameStop = true;
        DeleteObject(this);
        return 0x80000000;
    }
    
    return 0;
}


SnowSplash* SnowSplash::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    SnowSplash* pInstance = new SnowSplash(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Effect Create Failed");
        return nullptr;
    }
    return pInstance;
}

void SnowSplash::Free()
{
    __super::Free();
}
