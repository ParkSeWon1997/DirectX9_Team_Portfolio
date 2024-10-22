#include "FallMark.h"
#include "stdafx.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include "Monster.h"
#include "ResObject.h"
#include "IceMissileL.h"
#include "IceMissileR.h"
#include "Scene.h"
#include "Layer.h"
#include <random>
FallMark::FallMark(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
	:CEffect(pGraphicDev, _vPos), m_eCurState(APPEAR),
    m_ePreState(STATE_END), m_bMissileLaunch(false)
{
}

FallMark::FallMark(const FallMark& rhs)
	:CEffect(rhs)
{
}

FallMark::~FallMark()
{
}

HRESULT FallMark::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    m_fFrameEnd = 30.f;

    
    return S_OK;
}

_int FallMark::Update_GameObject(const _float& fTimeDelta)
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

void FallMark::LateUpdate_GameObject()
{
    CGameObject::LateUpdate_GameObject();
    m_pTransForm->BillBoard();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);
}

void FallMark::Render_GameObject()
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

void FallMark::Set_Dead()
{
    m_eCurState = DISAPPEAR;
    m_fFrame = 0.f;
}

HRESULT FallMark::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });


#pragma region TEXCOM

    pComponent = m_pTextureCom[APPEAR] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Ice_mark_pre"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Deer_Ice_mark_pre", pComponent });

    pComponent = m_pTextureCom[DISAPPEAR] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Ice_mark_pst"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Deer_Ice_mark_pst", pComponent });
#pragma endregion TEXCOM

    pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    m_pTransForm->Set_Scale({ 1.5f, 1.f, 1.5f });

    pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });


    return S_OK;
}

void FallMark::State_Change()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case APPEAR:
            m_fFrameEnd = 30.f;
            break;
        case DISAPPEAR:
            m_bFrameStop = false;
            m_fFrameEnd = 5.f;
            break;
        }
        m_fFrame = 0.f;
        m_ePreState = m_eCurState;
    }
}

_int FallMark::Appear()
{
    random_device rd;               //난수발생기
    default_random_engine eng(rd());
    uniform_int_distribution<int> distribution(0, 1);
    int randomNumber = distribution(eng);

    _vec3 vrDir, vlDir, vUp, vRight, vLook;
    m_pTransForm->Get_Info(INFO_UP, &vUp);
    m_pTransForm->Get_Info(INFO_LOOK, &vLook);
    m_pTransForm->Get_Info(INFO_RIGHT, &vRight);
    vrDir = vUp + vRight;
    vRight *= -1;
    vlDir = vUp + vRight;
    vrDir += vLook;
    vlDir += vLook;
    D3DXVec3Normalize(&vrDir, &vrDir);
    D3DXVec3Normalize(&vlDir, &vlDir);

   
    if (m_ePreState == APPEAR && !m_bMissileLaunch)
    {
        if (6 < m_fFrame)
        {
            m_bMissileLaunch = true;
            if (randomNumber == 0)
            {
                _vec3 vMissilePos;
                vMissilePos = m_vPos + vrDir * 30.f;
                CGameObject* pGameObject = IceMissileL::Create(m_pGraphicDev, vMissilePos);
                NULL_CHECK_RETURN(pGameObject, E_FAIL);
                CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObject);
                dynamic_cast<IceMissileL*>(pGameObject)->Set_Target(this, m_vPos);
            }
            else
            {
                _vec3 vMissilePos;
                vMissilePos = m_vPos + vlDir * 30.f;
                CGameObject* pGameObject = IceMissileR::Create(m_pGraphicDev, vMissilePos);
                NULL_CHECK_RETURN(pGameObject, E_FAIL);
                CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObject);
                dynamic_cast<IceMissileR*>(pGameObject)->Set_Target(this, m_vPos);
            }
        }
    }


    if (m_ePreState == APPEAR && m_fFrameEnd - 1 < m_fFrame)
    {
        m_fFrame = m_fFrameEnd - 1;
        m_bFrameStop = true;
    }

    if (m_ePreState == DISAPPEAR && m_fFrameEnd-1 < m_fFrame)
    {
        m_bFrameStop = true;
        m_fFrame = m_fFrameEnd - 1;
        DeleteObject(this);
        return 0x80000000;
    }
    
  

    
    return 0;
}


FallMark* FallMark::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    FallMark* pInstance = new FallMark(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Effect Create Failed");
        return nullptr;
    }
    return pInstance;
}

void FallMark::Free()
{
    __super::Free();
}
