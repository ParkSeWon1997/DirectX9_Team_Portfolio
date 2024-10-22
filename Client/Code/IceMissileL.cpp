#include "IceMissileL.h"
#include "stdafx.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include "Monster.h"
#include "ResObject.h"
#include "FallMark.h"
#include "SnowSplash.h"
#include "IceSpikeL.h"
IceMissileL::IceMissileL(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
	:CEffect(pGraphicDev, _vPos), m_bShoot(false), m_pTarget(nullptr)
{
}

IceMissileL::IceMissileL(const IceMissileL& rhs)
	:CEffect(rhs)
{
}

IceMissileL::~IceMissileL()
{
}

HRESULT IceMissileL::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    return S_OK;
}

_int IceMissileL::Update_GameObject(const _float& fTimeDelta)
{

    _int iResult = Appear(fTimeDelta);
    State_Change();
  
    CGameObject::Update_GameObject(fTimeDelta);
    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    return iResult;
}

void IceMissileL::LateUpdate_GameObject()
{
    CGameObject::LateUpdate_GameObject();
    m_pTransForm->BillBoard();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);
}

void IceMissileL::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


    FAILED_CHECK_RETURN(SetUp_Material(), );

    m_pTextureCom->Set_Texture(0);
    m_pBufferCom->Render_Buffer();
  


    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}



void IceMissileL::Set_Target(CGameObject* _target, _vec3 _vtarget)
{
    m_pTarget = _target;
    m_vTargetpos = _vtarget;
    if (m_bShoot == false)
        m_bShoot = true;
}

HRESULT IceMissileL::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });


#pragma region TEXCOM

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Ice_missile_right"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Deer_Ice_missile_right", pComponent });


#pragma endregion TEXCOM

    pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    m_pTransForm->Set_Scale({ 1.f, 4.f, 1.f });

    pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });


    return S_OK;
}

void IceMissileL::State_Change()
{
 
}

_int IceMissileL::Appear(const _float& fTimeDelta)
{
    if (m_bShoot)
    {
        _vec3 vDir = m_vTargetpos - m_vPos;
        D3DXVec3Normalize(&vDir, &vDir);
        m_pTransForm->Move_Pos(&vDir, 50.f, fTimeDelta);

        if (Collision_Transform(m_pTransForm, m_pTarget->GetTransForm()))
        {
            dynamic_cast<FallMark*>(m_pTarget)->Set_Dead();
            
            CGameObject* pGameObject = nullptr;
            CGameObject* lpGameObject = nullptr;

            pGameObject = SnowSplash::Create(m_pGraphicDev, m_vTargetpos);
            NULL_CHECK_RETURN(pGameObject, E_FAIL);
            CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObject);

            lpGameObject = IceSpikeL::Create(m_pGraphicDev, m_vTargetpos);
            NULL_CHECK_RETURN(lpGameObject, E_FAIL);
            CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, lpGameObject);

            DeleteObject(this);
            return 0x80000000;
        }

    }
    return 0;
}


IceMissileL* IceMissileL::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    IceMissileL* pInstance = new IceMissileL(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Effect Create Failed");
        return nullptr;
    }
    return pInstance;
}

void IceMissileL::Free()
{
    __super::Free();
}
