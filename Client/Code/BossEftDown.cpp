#include "BossEftDown.h"
#include "stdafx.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include "Monster.h"
#include "ResObject.h"

CBossEftDown::CBossEftDown(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
	:CEffect(pGraphicDev, _vPos), m_eCurState(APPEAR),
    m_ePreState(STATE_END), 
    m_bCollision(false)

{
}

CBossEftDown::CBossEftDown(const CBossEftDown& rhs)
	:CEffect(rhs)
{
}

CBossEftDown::~CBossEftDown()
{
}

HRESULT CBossEftDown::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    m_fFrameEnd = 11.f;
    m_fDamage = 30.f;
    m_fVolume = 0.2f;
    
    return S_OK;
}

_int CBossEftDown::Update_GameObject(const _float& fTimeDelta)
{
    
    if (!m_bFrameStop)
    {
        m_fFrame += m_fFrameEnd * fTimeDelta;
    }
    _int iResult = Appear();
    State_Change();
    Look_For_Change();
    Set_Scale();
    CGameObject::Update_GameObject(fTimeDelta);
    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    return iResult;
}

void CBossEftDown::LateUpdate_GameObject()
{
    CGameObject::LateUpdate_GameObject();
    m_pTransForm->BillBoard();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);
}

void CBossEftDown::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


    FAILED_CHECK_RETURN(SetUp_Material(), );
    m_pTextureCom->Set_Texture((_uint)m_fFrame);
    if (m_Dirchange)
    {
        m_pReverseCom->Render_Buffer();
    }
    else if (!m_Dirchange)
    {
        m_pBufferCom->Render_Buffer();
    }


    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CBossEftDown::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

#pragma region TEXCOM

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Atk_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Deer_Atk_Effect", pComponent });


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

void CBossEftDown::State_Change()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case APPEAR:
            m_fFrameEnd = 11.f;
            break;
        }
        m_fFrame = 0.f;
        m_ePreState = m_eCurState;
    }
}

_int CBossEftDown::Appear()
{
    if (m_ePreState == APPEAR)
    {
        if ((m_fFrame > 2) && !m_bCollision)
        {
            //충돌로직
            Check_Collision();
            Spike_Sound();
            m_bCollision = true;
        }

        if (m_fFrameEnd-1 < m_fFrame)
        {
            m_fFrame = m_fFrameEnd - 1;
            m_bFrameStop = true;
            /*DeleteObject(this);*/
            return 0x80000000;
        }
    }

    return 0;
 
}

void CBossEftDown::Set_Scale()
{

    if(m_ePreState == APPEAR)
        m_pTransForm->m_vScale = { 5.f, 2.f, 5.f };

}

void CBossEftDown::Look_For_Change()
{
    if (m_ePreLook != m_eCurLook)
    {
       /* _vec3 vCurPos ,vLook, vRight;
        vCurPos = m_vPos;
        m_pTransForm->Get_Info(INFO_LOOK, &vLook);
        m_pTransForm->Get_Info(INFO_RIGHT, &vRight);

        D3DXVec3Normalize(&vLook, &vLook);
        D3DXVec3Normalize(&vRight, &vRight);*/

        switch (m_eCurLook)
        {
        case LOOK_DOWN:
            m_Dirchange = false;
        /*    vCurPos += vLook * 2;
            m_pTransForm->Set_Pos(vCurPos);*/
            break;
        case LOOK_UP:
            m_Dirchange = true;
           /* vCurPos -= vLook * 2;
            m_pTransForm->Set_Pos(vCurPos);*/
            break;
       case LOOK_LEFT:
            m_Dirchange = false;
        /*    vCurPos += vRight * 2;
            vCurPos += vLook * 4;
            m_pTransForm->Set_Pos(vCurPos);*/
            break;
        case LOOK_RIGHT:
            m_Dirchange = true;
    /*        vCurPos -= vRight * 2;
            m_pTransForm->Set_Pos(vCurPos);*/
            break;
        }
        m_ePreLook = m_eCurLook;
    }

}

CBossEftDown* CBossEftDown::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    CBossEftDown* pInstance = new CBossEftDown(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Effect Create Failed");
        return nullptr;
    }
    return pInstance;
}

void CBossEftDown::Free()
{
    __super::Free();
}
