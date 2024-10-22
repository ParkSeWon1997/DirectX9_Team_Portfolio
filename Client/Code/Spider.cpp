#include "stdafx.h"
#include "Spider.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include"ResObject.h"

CSpider::CSpider(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
    :CMonster(pGraphicDev, _vPos), m_bModeChange(false), m_eCurstate(WALK), m_ePrestate(STATE_END)
    , m_bDetect(false)
{ 
}

CSpider::CSpider(const CSpider& rhs)
    :CMonster(rhs), m_bModeChange(rhs.m_bModeChange)
{
}

CSpider::~CSpider()
{
}

HRESULT CSpider::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    m_pTransForm->Set_Scale({ 2.f, 2.f, 2.f });
    m_fCollisionRadius = 0.4f;
    Set_ObjStat();
    m_fFrameEnd = 6;
    m_fFrameChange = rand() % 3;
    D3DXVec3Normalize(&m_vDir, &m_vDir);
    Look_Change();
    return S_OK;
}

_int CSpider::Update_GameObject(const _float& fTimeDelta)
{
    Volume_Controll();
    if (!m_bFrameStop)
    {
        m_fFrame += m_fFrameSpeed * fTimeDelta;
    }
    _int iResult = Die_Check();    //죽었는지 검사
    if (!m_Stat.bDead)      //죽지 않았을시 진입
    {
        Detect_Player();
        if (m_bDetect)     //플레이어와의 거리가 5보다 작으면 진입
        {
            Attacking(fTimeDelta);
        }
        else
        {
            Patroll(fTimeDelta);
        }
        Collision_EachOther(fTimeDelta);
    }
   

    State_Change();
    Look_Change();
   // Set_Scale();

  

    //Set_Scale();
    CGameObject::Update_GameObject(fTimeDelta);

    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    return iResult;
}

void CSpider::LateUpdate_GameObject()
{
    CGameObject::LateUpdate_GameObject();

    m_pTransForm->BillBoard();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);

}

void CSpider::Render_GameObject()
{

    //Test
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
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

HRESULT CSpider::Add_Component()
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

    //WALK
    pComponent = m_pTextureCom[LOOK_DOWN][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_side", pComponent });

    //ATTACK
    pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_atk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_atk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_atk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_atk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_atk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_atk_side", pComponent });


    //HIT
    pComponent = m_pTextureCom[LOOK_DOWN][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_hit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_hit", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_hit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_hit", pComponent });

    
    //DEAD
    pComponent = m_pTextureCom[LOOK_DOWN][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_dead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_dead", pComponent });


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



void CSpider::State_Change()
{
    if (m_ePrestate != m_eCurstate)
    {
        switch (m_eCurstate)
        {
        case IDLE:
            break;
        case WALK:
            m_fFrameSpeed = 7.f;
            m_fFrameEnd = 6.f;
            break;
        case ATTACK:
            m_fFrameSpeed = 10.f;
            m_fFrameEnd = 11.f;
            break;
        case SLEEP:
            break;
        case HIT:
            Engine::PlaySound_W(L"Obj_Spider_Hurt.mp3", SOUND_SPIDER, m_fVolume);
            m_fFrameEnd = 6;
            m_fFrameSpeed = 11.f;
            if (m_eCurLook != LOOK_LEFT)
            {
                m_eCurLook = LOOK_DOWN;
            }
            break;
        case DEAD:
            Dead_Sound();
            m_fFrameSpeed = 9.f;
            m_fFrameEnd = 9.f;
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

void CSpider::Set_ObjStat()
{
    m_Stat.strObjName = L"거미";
    m_Stat.fHP = 100.f;
    m_Stat.fMxHP = 100.f;
    m_Stat.fSpeed = 1.5f;

    m_Stat.fATK = 10.f;
    m_Stat.bDead = false;
    m_Stat.fATKRange = 1.3f;
    m_Stat.fAggroRange = 5.f;
}



void CSpider::Attacking(const _float& fTimeDelta)
{
    m_Stat.fSpeed = 5.5f;
   
    if (!m_bHit)
    {
        if (Collision_Circle(Get_Player_Pointer()) && m_ePrestate != ATTACK)
        {
            m_eCurstate = ATTACK;
        }
        else if (m_ePrestate == ATTACK)
        {
            
            if (6 < m_fFrame )
            {
                if (!m_bSound)
                {
                    Attack_Sound();
                    m_bSound = true;
                }
                

                if (Collision_Circle(Get_Player_Pointer()) && !m_bAttacking)
                {
                    dynamic_cast<CPlayer*>(Get_Player_Pointer())->Set_Attack(m_Stat.fATK);
                    m_bAttacking = true;
                }
            }
         
            else if (m_fFrameEnd < m_fFrame)
            {
                if (!Collision_Circle(Get_Player_Pointer()))
                {
                    if (!m_bSound)
                    {
                        Scream_Sound();
                        m_bSound = true;
                    }
                    m_eCurstate = WALK;
                }
            }
        }
        else if (m_ePrestate == WALK)
            Player_Chase(fTimeDelta);
    }
    else
    {
        
        if (m_fFrameEnd < m_fFrame)
        {
           /* Engine::PlaySound_W(L"Obj_Spider_Hurt.mp3", SOUND_SPIDER, 0.3f);*/
            m_bHit = false;
            m_eCurstate = WALK;
        }
    }
    if (m_fFrameEnd < m_fFrame)
    {
        m_fFrame = 0.f;
        if (m_bAttacking)
            m_bAttacking = false;
        if (m_bSound)
            m_bSound = false;
    }
}

void CSpider::Patroll(const _float& fTimeDelta)
{
    auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();
    CTerrainTex* pTerrainTex = dynamic_cast<CTerrainTex*>(pTerrain->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
    
    m_Stat.fSpeed = 1.5f;
  
    m_fAcctime +=  fTimeDelta;
    m_eCurstate = WALK;
    
    if (m_fFrameChange < m_fAcctime)
    {
        m_fAcctime = 0.f;
        m_fFrameChange = rand() % 3;
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
    else
    {
        _vec3 vCurPos = m_pTransForm->Get_Pos();
        if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
            m_vDir *= -1;
        else
            m_eCurLook = m_pTransForm->Patroll_LookChange(&m_vDir, m_Stat.fSpeed, fTimeDelta);
    }
    if (m_fFrameEnd < m_fFrame)
        m_fFrame = 0.f;
   
}

_int CSpider::Die_Check()
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
            Engine::PlaySound_W(L"Obj_Spider_Death_3.mp3", SOUND_EFFECT, m_fVolume);
           CResObject::CreateItem(L"Silk", this, m_pGraphicDev,2);
           CResObject::CreateItem(L"Meat_Monster", this, this->m_pGraphicDev);

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

void CSpider::Set_Hit()
{
    m_eCurstate = HIT;
    m_bHit = true;
}

void CSpider::Detect_Player()
{
    if (IsTarget_Approach(m_Stat.fAggroRange))
    {
        if (!m_bDetect && !dynamic_cast<CPlayer*>(Get_Player_Pointer())->IsPlayer_Dead())
            m_bDetect = true;
    }
    else
    {
        if (m_bDetect)
            m_bDetect = false;
    }
}

void CSpider::Attack_Sound()
{
    int randomvalue = rand() % 7;
    switch (randomvalue)
    {
    case 0:
        Engine::PlaySound_W(L"Obj_Spider_Attack_1.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 1:
        Engine::PlaySound_W(L"Obj_Spider_Attack_2.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 2:
        Engine::PlaySound_W(L"Obj_Spider_Attack_3.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 3:
        Engine::PlaySound_W(L"Obj_Spider_Attack_4.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 4:
        Engine::PlaySound_W(L"Obj_Spider_Attack_5.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 5:
        Engine::PlaySound_W(L"Obj_Spider_Attack_6.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 6:
        Engine::PlaySound_W(L"Obj_Spider_Attack_7.mp3", SOUND_EFFECT, m_fVolume);
        break;
    }
}

void CSpider::Dead_Sound()
{
    int randomvalue = rand() % 3;
    switch (randomvalue)
    {
    case 0:
        Engine::PlaySound_W(L"Obj_Spider_Death_1.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 1:
        Engine::PlaySound_W(L"Obj_Spider_Death_2.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 2:
        Engine::PlaySound_W(L"Obj_Spider_Death_3.mp3", SOUND_EFFECT, m_fVolume);
        break;
    }
}

void CSpider::Scream_Sound()
{
    int randomvalue = rand() % 7;
    switch (randomvalue)
    {
    case 0:
        Engine::PlaySound_W(L"Obj_Spider_Scream_1.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 1:
        Engine::PlaySound_W(L"Obj_Spider_Scream_2.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 2:
        Engine::PlaySound_W(L"Obj_Spider_Scream_3.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 3:
        Engine::PlaySound_W(L"Obj_Spider_Scream_4.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 4:
        Engine::PlaySound_W(L"Obj_Spider_Scream_5.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 5:
        Engine::PlaySound_W(L"Obj_Spider_Scream_6.mp3", SOUND_EFFECT, m_fVolume);
        break;
    case 6:
        Engine::PlaySound_W(L"Obj_Spider_Scream_7.mp3", SOUND_EFFECT, m_fVolume);
        break;
    }
}



CSpider* CSpider::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    CSpider* pInstance = new CSpider(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Spider Create Failed");
        return nullptr;
    }
    return pInstance;
}

void CSpider::Free()
{
    __super::Free();
}
