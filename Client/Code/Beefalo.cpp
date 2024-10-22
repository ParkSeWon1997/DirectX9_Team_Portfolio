#include "stdafx.h"
#include "Beefalo.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include <ItemBasic.h>
#include"ResObject.h"
CBeefalo::CBeefalo(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
    :CMonster(pGraphicDev, _vPos)
    , m_eCurState(WALK)
    , m_ePreState(STATE_END)
    , m_bHit(false)
{
}

CBeefalo::CBeefalo(const CBeefalo& rhs)
    :CMonster(rhs)
{
}

CBeefalo::~CBeefalo()
{
}

HRESULT CBeefalo::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    m_pTransForm->m_vScale = { 3.7f, 3.7f, 3.7f };
    Set_ObjStat();
    Look_Change();
    m_fFrameEnd = 10;
    m_fFrameChange = rand() % 5;
    m_fCollisionRadius = 2.f;
    //m_pTransForm->Set_Scale(_vec3(1.f, 1.f, 1.f));
    return S_OK;
}

_int CBeefalo::Update_GameObject(const _float& fTimeDelta)
{
    Volume_Controll();

    if (!m_bFrameStop)
    {
        m_fFrame += m_fFrameSpeed * fTimeDelta;
        FrameCheckSound(fTimeDelta);
    }
    else
    {
        m_fFrame = m_fFrameEnd;
    }
    _int iResult = Die_Check();

    if (!m_Stat.bDead)      //죽은 상태가 아닐때 진입
    {
        if (m_Attacked)     //공격받았을때 진입
        {
            Attacking(fTimeDelta);
        }
        else                 //공격 받지 않은 상태
        {
            Patroll(fTimeDelta);
        }
        Collision_EachOther(fTimeDelta);
    }           //여기에 else 걸어서 사망 트리거 연결(아이템 드랍 테이블)



   //Engine::Update_Sound(_vec3{ 1,1,1 }, get<0>(Get_Info_vec()), get<1>(Get_Info_vec()), get<2>(Get_Info_vec()), get<3>(Get_Info_vec()), SOUND_EFFECT, 5.f);
   // Engine::Update_Sound(_vec3{ 1,1,1 }, get<0>(Get_Info_vec()), get<1>(Get_Info_vec()), get<2>(Get_Info_vec()), get<3>(Get_Info_vec()), SOUND_EFFECT_CONTINUE_CH1, 5.f);
    CGameObject::Update_GameObject(fTimeDelta);
    State_Change();
    Look_Change();
    //Set_Scale();
    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    
    return iResult;
}

void CBeefalo::LateUpdate_GameObject()
{
    m_pTransForm->BillBoard();
    __super::LateUpdate_GameObject();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
}

void CBeefalo::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    /* Set_Scale();*/


    if (m_bHit)
    {
        m_pTextureCom[m_ePreLook][m_ePreState]->Set_Texture(m_pShaderCom, "g_Texture", m_fFrame);
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


        m_pTextureCom[m_ePreLook][m_ePreState]->Set_Texture((_uint)m_fFrame);

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

void CBeefalo::Set_Attack_State()
{
    if (!m_Attacked)
    {
        m_Attacked = true;
    }
}

HRESULT CBeefalo::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

#pragma region TEXCOM
    //Graze
    pComponent = m_pTextureCom[LOOK_DOWN][GRAZE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_graze"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_graze", pComponent });

    //IDLE
    pComponent = m_pTextureCom[LOOK_DOWN][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_idle"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_idle", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_idle"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_idle", pComponent });

    //Walk
    pComponent = m_pTextureCom[LOOK_DOWN][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_walk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_walk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_walk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_walk_side", pComponent });

    //RUN
    pComponent = m_pTextureCom[LOOK_DOWN][MADRUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_run_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_run_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][MADRUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_run_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_run_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][MADRUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_run_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_run_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][MADRUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_run_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_run_side", pComponent });

    //ATTACK
    pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_atk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_atk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_atk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_atk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_atk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_atk_side", pComponent });

    //DEAD
    pComponent = m_pTextureCom[LOOK_LEFT][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_dead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_dead", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_dead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_dead", pComponent });

    //HIT
    pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_hit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_hit", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_hit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_hit", pComponent });


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


void CBeefalo::Set_ObjStat()
{
    m_Stat.strObjName = L"버팔로";
    m_Stat.fHP = 100.f;
    m_Stat.fMxHP = 100.f;
    m_Stat.fSpeed = 1.f;
    m_Stat.fATK = 20.f;
    m_Stat.bDead = false;
    m_Stat.fATKRange = 1.3f;
    m_Stat.fAggroRange = 10.f;
    m_bStepSound[0] = false;
    m_bStepSound[1] = false;
}

void CBeefalo::State_Change()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case IDLE:
           // Engine::SpatialPlay_Sound(L"Obj_Beefalo_Chew_Voice_1.mp3",SOUND_BEEFALO);
            m_fFrameEnd = 16;
            m_fFrameSpeed = 16.f;
            if (m_eCurLook != LOOK_LEFT)
            {
            
                m_eCurLook = LOOK_DOWN;
            }
            break;
        case GRAZE:
            m_fFrameSpeed = 12.f;
            m_eCurLook = LOOK_DOWN;
            m_fFrameEnd = 10;
            break;
        case WALK:
            m_fFrameSpeed = 20.f;
            m_fFrameEnd = 20;
            
           // Engine::SpatialPlay_Sound(L"Obj_Beefalo_Chew_Voice_1.mp3", STEREO_EFFECT);
       
            break;
        case ATTACK:
            Engine::PlaySound_W(L"Obj_Beefalo_Angry.mp3", SOUND_BEEFALO, m_fVolume);
  
            
            m_fFrameSpeed = 10.f;
            m_fFrameEnd = 7;
            break;
        case MADRUN:
         

            m_fFrameSpeed = 14.f;
            m_fFrameEnd = 4;
            break;
        case HIT:
            if (m_eCurLook != LOOK_LEFT)
            {
                m_eCurLook = LOOK_RIGHT;
            }
            m_fFrameEnd = 5;
            m_fFrameSpeed = 11.f;
            break;
        case DEAD:
            m_fFrameEnd = 12;
            m_fFrameSpeed = 18.f;
            if (m_eCurLook != LOOK_LEFT)
            {
                m_eCurLook = LOOK_RIGHT;
            }
            break;
        case ERASE:
            m_fFrameSpeed = 8.f;
            m_fFrameEnd = 5;
            m_eCurLook = LOOK_DOWN;
            break;
        }
        m_fFrame = 0.f;
        m_ePreState = m_eCurState;
    }
}

_int CBeefalo::Die_Check()
{
    if (m_Stat.fHP <= 0 && m_ePreState != DEAD && m_ePreState != ERASE)
    {
       
        m_eCurState = DEAD;
        m_Stat.bDead = true;
    }
    else if (m_ePreState == DEAD)
    {
       
        if (m_fFrameEnd <= m_fFrame)
        {
            Engine::PlaySound_W(L"Obj_Beefalo_Yell.mp3", SOUND_BEEFALO, m_fVolume);
            CResObject::CreateItem(L"RawMeat",this,this->m_pGraphicDev);
           

            m_eCurState = ERASE;
        }
    }
    else if (m_ePreState == ERASE)
    {
        if ((m_fFrameEnd - 1) <= m_fFrame)
        {
            m_bFrameStop = true;
            DeleteObject(this);
            return 0x80000000;
        }
    }

    return 0;
}

void CBeefalo::Attacking(const _float& fTimeDelta)
{
    m_Stat.fSpeed = 5.f;
    if (!m_bHit)
    {
        if (Collision_Circle(Get_Player_Pointer()) && m_ePreState != ATTACK)
        {
            m_eCurState = ATTACK;
        }
        else if (m_ePreState == ATTACK)
        {
            if ((m_fFrameEnd < m_fFrame) && !Collision_Circle(Get_Player_Pointer()))
            { 
                m_eCurState = MADRUN;
            }
            else if ((3 < m_fFrame) )
            {
                if (!m_bSound)
                {
                    Engine::PlaySound_W(L"Obj_Beefalo_AttackVoice.mp3", SOUND_BEEFALO, m_fVolume);
                    m_bSound = true;
                }
                if (Collision_Circle(Get_Player_Pointer()) && !m_bAttacking)
                {
                    Engine::PlaySound_W(L"Obj_Beefalo_Attack.mp3", SOUND_BEEFALO, m_fVolume);
                    dynamic_cast<CPlayer*>(Get_Player_Pointer())->Set_Attack(m_Stat.fATK);
                    m_bAttacking = true;
                }
            }

        }
        else if (m_ePreState == MADRUN && !Collision_Circle(Get_Player_Pointer()))
        {
            Player_Chase(fTimeDelta);
        }
        else if (!Collision_Circle(Get_Player_Pointer()))
        {
            m_eCurState = MADRUN;
        }
    }
    else
    {
        if (m_fFrameEnd < m_fFrame)
        {
            Engine::PlaySound_W(L"Obj_Beefalo_Farting_1.mp3", SOUND_BEEFALO, m_fVolume);
            m_bHit = false;
        }

    }

    if (!IsTarget_Approach(10.f) || dynamic_cast<CPlayer*>(scenemgr::Get_CurScene()->GetPlayerObject())->IsPlayer_Dead())
    {
        m_Attacked = false;
    }


    if (m_fFrameEnd < m_fFrame)
    {
        m_fFrame = 0.f;
        if(m_bAttacking)
            m_bAttacking = false;
        if (m_bSound)
            m_bSound = false;
    }
}

void CBeefalo::Patroll(const _float& fTimeDelta)
{
    auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();
    CTerrainTex* pTerrainTex = dynamic_cast<CTerrainTex*>(pTerrain->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
    m_fAcctime += fTimeDelta;
    m_Stat.fSpeed = 1.f;
    if (m_fFrameChange < m_fAcctime)        //엑셀타임 충족 트리거
    {
        m_fAcctime = 0.f;
        m_fFrameChange = rand() % 6;
        int RandomPattern = rand() % 3;         // 0 1 2 인덱스 중 랜덤 패턴 적용
        m_eCurState = (BEEFALOSTATE)RandomPattern;

        if (m_eCurState == WALK)            //걷기가 당첨되면 방향 변환
        {

            int randomValue = rand() % 360;
            int randomValue2 = rand() % 360;
            // 부호를 무작위로 선택 (-1 또는 1)
            int sign = (rand() % 2 == 0) ? 1 : -1;
            int sign2 = (rand() % 2 == 0) ? 1 : -1;

            // 랜덤값에 부호를 적용
            int result = randomValue * sign;
            int result2 = randomValue2 * sign2;
            m_vDir = { (float)result,0.f,(float)result2 };
            D3DXVec3Normalize(&m_vDir, &m_vDir);
        }
    }
    else if (m_ePreState == WALK)           //걷기일때 방향으로 이동.
    {
        /*const _vec3* _vDir, const _float& fSpeed, const _float& fTimeDelta*/
        _vec3 vCurPos = m_pTransForm->Get_Pos();
        if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
            m_vDir *= -1;
        else
            m_eCurLook = m_pTransForm->Patroll_LookChange(&m_vDir, m_Stat.fSpeed, fTimeDelta);
    }

    if (m_fFrameEnd < m_fFrame)
        m_fFrame = 0.f;

}

void CBeefalo::Set_Hit()
{
    m_eCurState = HIT;
    m_bHit = true;

    _vec3 vThispos;
    vThispos = m_pTransForm->Get_Pos();
    auto& vecObj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::MONSTER);
    for (auto& obj : vecObj)
    {
        _vec3  vTargetPos;
        vTargetPos = obj->GetTransForm()->Get_Pos();
        if (D3DXVec3Length(&(vTargetPos - vThispos)) <= m_Stat.fAggroRange)
        {
            if (obj->IsDelete() || obj == nullptr || dynamic_cast<CMonster*>(obj)->Get_Name() != m_Stat.strObjName)
                continue;
            else
                dynamic_cast<CBeefalo*>(obj)->Set_Attack_State();
        }
       
    }


}

void CBeefalo::FrameCheckSound(const _float& fTimeDelta)
{
    if (m_eCurState == WALK)
    {
        if (m_fFrame >3 && !m_bStepSound[0])
        {
             Engine::PlaySound_W(L"Obj_Beefalo_Walk_1.mp3", SOUND_BEEFALO, m_fVolume);
             m_bStepSound[0] = true;
        }
        if (m_fFrame > 13 && !m_bStepSound[1])
        {
            Engine::PlaySound_W(L"Obj_Beefalo_Walk_1.mp3", SOUND_BEEFALO, m_fVolume);
            m_bStepSound[1] = true;
        }
 
    }

    if (m_fFrame > m_fFrameEnd)
    {
        m_bStepSound[0] = false;
        m_bStepSound[1] = false;
    }
   
}


CBeefalo* CBeefalo::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    CBeefalo* pInstance = new CBeefalo(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Beefalo Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CBeefalo::Free()
{
    __super::Free();
}