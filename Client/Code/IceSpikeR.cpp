#include "IceSpikeR.h"
#include "stdafx.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include "Monster.h"
#include "ResObject.h"

IceSpikeR::IceSpikeR(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
	:CEffect(pGraphicDev, _vPos), m_eCurState(ONE),
    m_ePreState(STATE_END), m_bAttack(false)
{
}

IceSpikeR::IceSpikeR(const IceSpikeR& rhs)
	:CEffect(rhs)
{
}

IceSpikeR::~IceSpikeR()
{
}

HRESULT IceSpikeR::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    m_fFrameEnd = 11.f;
    m_fDamage = 10.f;
    int RandomPattern = rand() % 3;         // 0 1 2 ÀÎµ¦½º Áß ·£´ý ÆÐÅÏ Àû¿ë
    m_eCurState = (ICESPIKE)RandomPattern;
   
    return S_OK;
}

_int IceSpikeR::Update_GameObject(const _float& fTimeDelta)
{
    Volume_Controll();
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

void IceSpikeR::LateUpdate_GameObject()
{
    CGameObject::LateUpdate_GameObject();
    m_pTransForm->BillBoard();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);
}

void IceSpikeR::Render_GameObject()
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

HRESULT IceSpikeR::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });


#pragma region TEXCOM

    pComponent = m_pTextureCom[ONE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Ice_spike_1"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Deer_Ice_spike_1", pComponent });

    pComponent = m_pTextureCom[TWO] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Ice_spike_4"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Deer_Ice_spike_4", pComponent });

    pComponent = m_pTextureCom[THREE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Ice_spike_6"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Deer_Ice_spike_6", pComponent });


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

void IceSpikeR::State_Change()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case ONE:
            break;
        case TWO:
            break;
        case THREE:
            break;
        }
        m_fFrameEnd = 11.f;
        m_fFrame = 0.f;
        m_ePreState = m_eCurState;
    }
}

_int IceSpikeR::Appear()
{
    if (m_fFrameEnd-1 < m_fFrame)
    {
        Check_Collision();
        m_fFrame = m_fFrameEnd - 1;
        m_bFrameStop = true;
        DeleteObject(this);
        return 0x80000000;
    }

    if (m_fFrame > 3 && !m_bSound)
    {
       
        Spike_Sound();
        m_bSound = true;
    }

    return 0;
}


IceSpikeR* IceSpikeR::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    IceSpikeR* pInstance = new IceSpikeR(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Effect Create Failed");
        return nullptr;
    }
    return pInstance;
}

void IceSpikeR::Free()
{
    __super::Free();
}
