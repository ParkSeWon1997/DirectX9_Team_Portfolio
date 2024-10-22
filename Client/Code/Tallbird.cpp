#include "stdafx.h"
#include "Tallbird.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include"ResObject.h"

CTallbird::CTallbird(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
    :CMonster(pGraphicDev, _vPos),
    m_eCurstate(SLEEP),
    m_ePrestate(STATE_END),
    m_bWakeUp(false)
{ 
}

CTallbird::CTallbird(const CTallbird& rhs)
    :CMonster(rhs)
{
}

CTallbird::~CTallbird()
{
}

HRESULT CTallbird::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    m_pTransForm->Set_Scale({ 3.5f, 3.5f, 3.5f });
    Set_ObjStat();
    m_bStatChange[0] = false;
    m_bStatChange[1] = false;
    m_fFrameEnd = 0;
    m_bFrameStop = true;
    m_fFrameChange = rand() % 3;
    m_fCollisionRadius = 0.7f;
    D3DXVec3Normalize(&m_vDir, &m_vDir);
    Look_Change();
    return S_OK;
}

_int CTallbird::Update_GameObject(const _float& fTimeDelta)
{
  
    Volume_Controll();

    if (!m_bFrameStop)
    {
        m_fFrame += m_fFrameSpeed * fTimeDelta;
    }

    if (m_bWakeUp)  //1회 최초진입
    {
        m_bStatChange[0] = true;
        m_bFrameStop = false;
    }

    _int iResult = Die_Check();    //죽었는지 검사
    if (!m_Stat.bDead)      //죽지 않았을시 진입
    {
        if (m_bStatChange[0])
        {           
            if (m_bStatChange[1])
            {
                Second_Phase(fTimeDelta);
            }
            else
            {
                First_Phase(fTimeDelta);
            }
            
        }
    }
    


    State_Change();
    Look_Change();
    //Set_Scale();
    CGameObject::Update_GameObject(fTimeDelta);

    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    return iResult;
}

void CTallbird::LateUpdate_GameObject()
{
    CGameObject::LateUpdate_GameObject();

    m_pTransForm->BillBoard();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);

}

void CTallbird::Render_GameObject()
{
    //Test
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    //m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


    if (m_bHit)
    {
        m_pTextureCom[m_ePreLook][m_ePrestate]->Set_Texture(m_pShaderCom, "g_Texture", m_fFrame);
        _matrix maxView, maxProj;
        m_pGraphicDev->GetTransform(D3DTS_VIEW, &maxView);
        m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &maxProj);

        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransForm->Get_WorldMatrix())))
            return;
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &maxView)))
            return;
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &maxProj)))
            return;

        if (FAILED(m_pShaderCom->Begin_Shader(2)))
            return;

        if (m_Dirchange)
        {
            m_pReverseCom->Render_Buffer();
        }
        else if (!m_Dirchange)
        {
            m_pBufferCom->Render_Buffer();
        }

        if (FAILED(m_pShaderCom->End_Shader()))
            return;
    }
    else
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

        m_pTextureCom[m_ePreLook][m_ePrestate]->Set_Texture((_uint)m_fFrame);

        FAILED_CHECK_RETURN(SetUp_Material(), );
        if (m_Dirchange)
        {
            m_pReverseCom->Render_Buffer();
        }
        else if (!m_Dirchange)
        {
            m_pBufferCom->Render_Buffer();
        }
    }

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CTallbird::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

    //Texturecom
#pragma region TEXCOM
    //IDLE
    pComponent = m_pTextureCom[LOOK_DOWN][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_idle"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_idle", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_idle"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_idle", pComponent });




    //WALK
    pComponent = m_pTextureCom[LOOK_DOWN][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_walkdown"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_walkdown", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_walkup"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_walkup", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_walkiside"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_walkiside", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_walkiside"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_walkiside", pComponent });

    //ATTACK
    pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_atk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_atk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_atk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_atk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_atk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_atk_side", pComponent });


    
    //HIT
    pComponent = m_pTextureCom[LOOK_DOWN][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_hit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_hit", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_hit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_hit", pComponent });

    //Sleep
    pComponent = m_pTextureCom[LOOK_DOWN][SLEEP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_sleep"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_sleep", pComponent });

    //Wakeup
    pComponent = m_pTextureCom[LOOK_DOWN][WAKE_UP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_wakeup"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_wakeup", pComponent });

    //taunt
    pComponent = m_pTextureCom[LOOK_DOWN][TAUNT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_taunt"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_taunt", pComponent });

    //DEAD
    pComponent = m_pTextureCom[LOOK_DOWN][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_dead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_dead", pComponent });

    //ERASE
    pComponent = m_pTextureCom[LOOK_DOWN][ERASE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Erase"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Erase", pComponent });
#pragma endregion TEXCOM
    
    

    pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

    pComponent = m_pShaderCom = dynamic_cast<CShader*>(proto::Clone_Proto(L"Proto_Shader_Rect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Shader_Rect", pComponent });

    return S_OK;
}



void CTallbird::State_Change()
{
    if (m_ePrestate != m_eCurstate)
    {
        switch (m_eCurstate)
        {
        case IDLE:
            m_fFrameEnd = 14.f;
            if (m_eCurLook != LOOK_LEFT)
            {
                m_eCurLook = LOOK_DOWN;
            }
            m_fFrameSpeed = 14.f;
            break;
        case WALK:
            m_fFrameSpeed = 9.f;
            m_fFrameEnd = 8.f;
            break;
        case ATTACK:
            m_fFrameSpeed = 11.f;
            m_fFrameEnd = 12.f;
            break;
        case SLEEP:
            m_fFrameSpeed = 1.f;
            m_eCurLook = LOOK_DOWN;
            m_fFrameEnd = 0;
            break;
        case WAKE_UP:
            m_fFrameSpeed = 14.f;
           // Engine::SpatialPlay_Sound(L"Obj_TallBird_Wakeup.mp3", SOUND_EFFECT);

           Engine::PlaySound_W(L"Obj_TallBird_Wakeup.mp3", SOUND_TALLBIRD, m_fVolume);
            m_eCurLook = LOOK_DOWN;
            m_fFrameEnd = 16;
            break;
        case TAUNT:
            m_fFrameSpeed = 14.f;
            m_eCurLook = LOOK_DOWN;
            m_fFrameEnd = 22.f;
            break;
        case HIT:
            m_fFrameSpeed = 14.f;
            //Engine::SpatialPlay_Sound(L"Obj_TallBird_Hurt_2.mp3", SOUND_EFFECT);

            Engine::PlaySound_W(L"Obj_TallBird_Hurt_2.mp3", SOUND_TALLBIRD, m_fVolume);
            m_fFrameEnd = 6;
            if (m_eCurLook != LOOK_LEFT)
            {
                m_eCurLook = LOOK_DOWN;
            }
            break;
        case DEAD:
            m_fFrameSpeed = 10.f;
            //Engine::SpatialPlay_Sound(L"Obj_TallBird_Death.mp3", SOUND_EFFECT);
            
            Engine::PlaySound_W(L"Obj_TallBird_Death.mp3", SOUND_TALLBIRD, m_fVolume);
            m_eCurLook = LOOK_DOWN;
            m_fFrameEnd = 10.f;
            break;
        case ERASE:
            m_fFrameSpeed = 8.f;
            m_fFrameEnd = 5;
            m_eCurLook = LOOK_DOWN;
            break;
        }
        m_ePrestate = m_eCurstate;
        m_fFrame = 0.f;
    }
}

void CTallbird::Set_ObjStat()
{
    m_Stat.strObjName = L"키다리새";
    m_Stat.fHP = 150.f;
    m_Stat.fMxHP = 150.f;
    m_Stat.fSpeed = 2.5f;

    m_Stat.fATK = 25.f;
    m_Stat.bDead = false;
    m_Stat.fATKRange = 2.f;
    m_Stat.fAggroRange = 3.f;
}

void CTallbird::Set_Scale()
{
  
}

void CTallbird::First_Phase(const _float& fTimeDelta)
{
    if (m_ePrestate == SLEEP)
    {
        m_eCurstate = WAKE_UP;
    }
    else if (m_ePrestate == WAKE_UP)
    {
        if (m_fFrameEnd < m_fFrame)
        {
            m_eCurstate = TAUNT;
        }
    }
    else if (m_ePrestate == TAUNT)
    {
        if (m_fFrameEnd < m_fFrame)
        {
            m_eCurstate = IDLE;
        }
    }
    else if (m_ePrestate == IDLE)
    {
        if (m_fFrameEnd < m_fFrame)
        {
            m_eCurstate = WALK;
            m_bStatChange[1] = true;
        }
    }

    if (m_fFrameEnd < m_fFrame)
    {
        m_fFrame = 0.f;
    }

}

void CTallbird::Second_Phase(const _float& fTimeDelta)
{

    if (!m_bHit)
    {
        if (Collision_Circle(Get_Player_Pointer()))
        {
            m_eCurstate = ATTACK;
        }
        else
        {
            if (m_ePrestate == ATTACK && m_fFrameEnd < m_fFrame)
            {
                m_eCurstate = WALK;
            }
        }



        if (m_ePrestate == ATTACK)
        {
            if (m_fFrame > 5.f)
            {
                if (!m_bAttacking && Collision_Circle(Get_Player_Pointer()))
                {
                    dynamic_cast<CPlayer*>(Get_Player_Pointer())->Set_Attack(m_Stat.fATK);
                    m_bAttacking = true;
                }
                if (!m_bSound)
                {
                    Engine::PlaySound_W(L"Obj_TallBird_Attack.mp3", SOUND_TALLBIRD, m_fVolume);
                    m_bSound = true;
                }
            }
            else if (m_fFrameEnd < m_fFrame && !Collision_Circle(Get_Player_Pointer()))
            {
                m_eCurstate = WALK;
            }
        }
        else if (m_ePrestate == WALK)
        {
            Player_Chase(fTimeDelta);
            Collision_EachOther(fTimeDelta);
        }

    }
   
    if (m_ePrestate == HIT)
    {
        if (m_fFrameEnd < m_fFrame)
            m_eCurstate = WALK;
    }

    if (m_fFrameEnd < m_fFrame)
    {
        m_fFrame = 0.f;
        if(m_bAttacking)
            m_bAttacking = false;
        if (m_bHit)
            m_bHit = false;
        if (m_bSound)
            m_bSound = false;
    }
    
}


_int CTallbird::Die_Check()
{
    if (m_Stat.fHP <= 0 && m_ePrestate != DEAD && m_ePrestate != ERASE )
    {
        m_eCurstate = DEAD;
        m_eCurLook = LOOK_DOWN;
        m_Stat.bDead = true;
        m_fFrame = 0.f;
    }
    else if (m_ePrestate == DEAD)
    {
 
        if (m_fFrameEnd < m_fFrame)
        {

            CResObject::CreateItem(L"Meat_Monster", this, m_pGraphicDev);

            m_eCurstate = ERASE;
        }
    }
    else if (m_ePrestate == ERASE)
    {
        if (m_fFrameEnd-1 < m_fFrame)
        {
            m_bFrameStop = true;
            DeleteObject(this);
            return 0x80000000;
        }
    }
    
    return 0;
        
}

void CTallbird::Set_Hit()
{
    m_eCurstate = HIT;
    m_bHit = true;
}



CTallbird* CTallbird::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    CTallbird* pInstance = new CTallbird(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Tallbird Create Failed");
        return nullptr;
    }
    return pInstance;
}

void CTallbird::Free()
{
    __super::Free();
}
