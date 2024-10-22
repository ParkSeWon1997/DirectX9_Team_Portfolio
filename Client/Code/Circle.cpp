#include "Circle.h"
#include "stdafx.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include "Monster.h"
#include "ResObject.h"
#include "SnowSplash.h"

CCircle::CCircle(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
	:CEffect(pGraphicDev, _vPos), m_eCurState(APPEAR),
    m_ePreState(STATE_END), m_iCount(0), m_iCountMax(1)
{
}

CCircle::CCircle(const CCircle& rhs)
	:CEffect(rhs)
{
}

CCircle::~CCircle()
{
}

HRESULT CCircle::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    m_fFrameEnd = 11.f;
    return S_OK;
}

_int CCircle::Update_GameObject(const _float& fTimeDelta)
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

void CCircle::LateUpdate_GameObject()
{
    CGameObject::LateUpdate_GameObject();
    m_pTransForm->BillBoard();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);
}

void CCircle::Render_GameObject()
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

HRESULT CCircle::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });


#pragma region TEXCOM

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Circle_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Deer_Circle_Effect", pComponent });


#pragma endregion TEXCOM

    pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    m_pTransForm->Set_Scale({ 10.f, 4.f, 10.f });

    pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });


    return S_OK;
}

void CCircle::State_Change()
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

_int CCircle::Appear()
{
    if (m_fFrameEnd-1 < m_fFrame && m_iCount == m_iCountMax)
    {



        m_fFrame = m_fFrameEnd - 1;
        m_bFrameStop = true;
        DeleteObject(this);
        return 0x80000000;
    }
    
    if (m_fFrameEnd < m_fFrame)
    {
        m_iCount++;
        Generate_SnowSplash();
        m_fFrame = 0.f;
    }
    return 0;
}

void CCircle::Generate_SnowSplash()
{
    srand(unsigned(time(NULL)));
    for (int i = 0; i < 10; ++i)
    {
        _vec3 pPos = m_vPos;
        int randomValue = rand() % 10;
        int randomValue2 = rand() % 5;
        // 부호를 무작위로 선택 (-1 또는 1)
        int sign = (rand() % 2 == 0) ? 1 : -1;
        int sign2 = (rand() % 2 == 0) ? 1 : -1;

        // 랜덤값에 부호를 적용
        int result = randomValue * sign;
        int result2 = randomValue2 * sign2;

        pPos.x += (float)result;
        pPos.z += (float)result2;
        CGameObject* pGameObject = SnowSplash::Create(m_pGraphicDev, pPos);
        CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObject);
    }
}


CCircle* CCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    CCircle* pInstance = new CCircle(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Effect Create Failed");
        return nullptr;
    }
    return pInstance;
}

void CCircle::Free()
{
    __super::Free();
}
