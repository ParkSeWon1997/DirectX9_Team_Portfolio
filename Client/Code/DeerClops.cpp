#include "stdafx.h"
#include "DeerClops.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Scene.h"
#include "Player.h"
#include "BossEftDown.h"
#include "Layer.h"
#include "Circle.h"
#include "FallMark.h"
#include "Scene.h"
#include "SnowSplash.h"
#include "DynamicCamera.h"
#include "CRoadSceneCamera.h"
#include "Tallbird.h"
#include "SizemicL.h"
#include "SizemicR.h"

CDeerClops::CDeerClops(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
	: CMonster(pGraphicDev, _vPos)
	, m_eCurState(SLEEP)
	, m_ePreState(STATE_END)
	, m_fSkill(0.f)
	, m_fSkill2(0.f)
	, m_fAcctime2(0.f)
	, m_AttackPos(false)
	, m_bGimmick(false)
{
}

CDeerClops::CDeerClops(const CDeerClops& rhs)
	:CMonster(rhs)
{//
}

CDeerClops::~CDeerClops()
{//

}

HRESULT CDeerClops::Ready_GameObject()
{
	//true 면 위아래고 , false면 상하좌우
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Set_Pos(m_vPos);
	Set_ObjStat();
	m_fAcctime = float(rand() % 30);
	m_bFalldown = false;
	m_fCollisionRadius = 1.f;

	for (auto i = 0; i < DEER_PHASE::PHASE_END; ++i)
	{
		m_bPhase[i] = false;
	}

	return S_OK;
}

_int CDeerClops::Update_GameObject(const _float& fTimeDelta)
{
	IsBossStage();

	if (!m_bFrameStop)//
	{
		m_fFrame += m_fFrameSpeed * fTimeDelta;
	}

	When_Walking();
	When_IDLE();
	if (m_bPhase[FIRST]) //m_bPhase 불 변수의 첫번째 인덱스부터 true로 켜질때마다 페이즈가 변경됨
	{
		if (m_bPhase[SECOND])		//First Phase 에서 Second Phase 는 자동으로 연결됨
		{
			if (m_bPhase[THIRD])
			{
				if (m_bPhase[FOURTH])
				{
					if (m_bPhase[FIFTH])
					{
						if (m_bPhase[DIE])
							Boss_Die(fTimeDelta);
						else
							Fifth_Phase(fTimeDelta);
					}
					else
						Fourth_Phase(fTimeDelta);
				}
				else
					Third_Phase(fTimeDelta);
			}
			else
				Second_Phase(fTimeDelta);
		}
		else
			First_Phase(fTimeDelta);
	}
	else
		Sleep(fTimeDelta);

	if (KEY_TAP(DIK_9)) // 9번 누르면 깨움
	{
		Set_WakeUp();
	}

	if (KEY_TAP(DIK_6))		//6번 누르면 피깍음
	{
		Set_Hit();
		m_Stat.fHP -= 50;
	}

	State_Change();
	Look_Change();
	//Set_Scale();
	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CDeerClops::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	m_pTransForm->BillBoard();
	_vec3	vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	CGameObject::Compute_ViewZ(&vPos);
}

void CDeerClops::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


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

void CDeerClops::Set_WakeUp()
{
	m_bPhase[FIRST] = true;
	m_eCurState = SLEEP_PST;
}

HRESULT CDeerClops::Add_Component()
{
	CComponent* pComponent = nullptr;
	//d
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });


#pragma region TEXCOM

	//IDLE
	pComponent = m_pTextureCom[LOOK_DOWN][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Idle_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Idle_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Idle_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Idle_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Idle_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Idle_side", pComponent });


	//WALK
	pComponent = m_pTextureCom[LOOK_DOWN][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_walk_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_walk_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_walk_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_walk_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_walk_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_walk_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_walk_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_walk_side", pComponent });

	//ATTACK
	pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Atk_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Atk_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Atk_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Atk_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Atk_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Atk_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Atk_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Atk_side", pComponent });

	//HIT
	pComponent = m_pTextureCom[LOOK_DOWN][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_hit_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_hit_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_hit_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_hit_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_hit_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_hit_side", pComponent });

	//Sleep Loop
	pComponent = m_pTextureCom[LOOK_DOWN][SLEEP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_sleep_loop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_sleep_loop", pComponent });

	//Sleep Pst
	pComponent = m_pTextureCom[LOOK_DOWN][SLEEP_PST] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_sleep_pst"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_sleep_pst", pComponent });

	//Taunt
	pComponent = m_pTextureCom[LOOK_DOWN][TAUNT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_taunt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_taunt", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][LONG_TAUNT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_long_taunt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_long_taunt", pComponent });


	//Fall down
	pComponent = m_pTextureCom[LOOK_DOWN][FALL_DOWN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Fall_Down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Fall_Down", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][WAKE_UP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Wake_Up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Wake_Up", pComponent });

	//Pattern
	pComponent = m_pTextureCom[LOOK_DOWN][PATTERN_PRE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Pattern_Pre"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Pattern_Pre", pComponent });


	pComponent = m_pTextureCom[LOOK_DOWN][PATTERN_LOOP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Pattern_Loop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Pattern_Loop", pComponent });


	//DEAD
	pComponent = m_pTextureCom[LOOK_DOWN][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_dead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_dead", pComponent });
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

void CDeerClops::Set_Hit()
{
	m_fFrame = 0.f;
	if (m_Stat.fHP > 0)
	{
		if (m_ePreState != ATTACK && !m_bGimmick && m_eCurState != LONG_TAUNT)
		{
			Hit_Sound();
			m_eCurState = HIT;
			m_fFrame = 0.f;
			m_bHit = true;
		}

		if (m_Stat.fHP < 800.f && !m_bPhase[FIFTH])
		{
			m_bPhase[FIFTH] = true;
			m_eCurState = LONG_TAUNT;
			m_bAttackCooltime = false;
			m_fAcctime = 0.f;
			m_bAttacking = false;
			m_bGimmick = false;
			m_fSkill2 = 0.f;
			m_fAcctime2 = 0.f;

		}
		else
			return;
	}
	else
	{
		if (!m_bPhase[DIE])
		{
			m_bPhase[DIE] = true;
			Engine::PlaySound_W(L"Obj_Deerclops_Death_1.mp3", SOUND_DEERCLOPS, 0.5f);
			m_eCurState = DEAD;
			m_Stat.bDead = true;
		}
		else
			return;
	}
}

void CDeerClops::Set_ObjStat()
{
	m_Stat.strObjName = L"외눈사슴";
	m_Stat.fHP = 1500.f;
	m_Stat.fMxHP = 1500.f;
	m_Stat.fSpeed = 2.f;
	m_Stat.fATK = 20.f;
	m_Stat.fATKRange = 3.f;
	m_Stat.fAggroRange = 10.f;
	m_Stat.bDead = false;
}

void CDeerClops::State_Change()
{

	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_fFrameSpeed = 19.f;
			m_fFrameEnd = 19;
			break;
		case WALK:
			m_fFrameSpeed = 16.f;
			m_fFrameEnd = 16;
			break;
		case ATTACK:
			m_fFrameSpeed = 18.f;
			m_fFrameEnd = 20;
			break;
		case SLEEP:
			m_fFrameSpeed = 23.f;
			m_fFrameEnd = 23;
			m_eCurLook = LOOK_DOWN;
			break;
		case SLEEP_PST:
			m_fFrameSpeed = 18.f;
			m_fFrameEnd = 18;
			m_eCurLook = LOOK_DOWN;
			break;
		case TAUNT:
			Engine::PlaySound_W(L"Obj_Deerclops_Taunt_2.mp3", SOUND_DEERCLOPS, 0.5f);
			m_fFrameSpeed = 17.f;
			Generate_Roaring(0.9);
			Camera_Shaking(1.f, 2.f, false);
			m_fFrameEnd = 17;
			m_eCurLook = LOOK_DOWN;
			break;
		case LONG_TAUNT:
			m_fFrameSpeed = 10.f;
			Camera_Shaking(1.f, 2.f, false);
			Generate_Roaring(3);
			m_fFrameEnd = 33;
			m_eCurLook = LOOK_DOWN;
			break;
		case FALL_DOWN:
			m_fFrameSpeed = 40.f;
			m_fFrameEnd = 37;
			m_eCurLook = LOOK_DOWN;
			break;
		case WAKE_UP:
			m_fFrameSpeed = 12.f;
			m_fFrameEnd = 11;
			m_eCurLook = LOOK_DOWN;
			break;
		case PATTERN_PRE:
			m_fFrameSpeed = 13.f;
			m_fFrameEnd = 12;
			m_eCurLook = LOOK_DOWN;
			break;
		case PATTERN_LOOP:
			m_fFrameSpeed = 13.f;
			m_fFrameEnd = 13;
			m_eCurLook = LOOK_DOWN;
			break;
		case HIT:
			m_fFrameSpeed = 9.f;
			m_fFrameEnd = 7;
			break;
		case DEAD:
			m_fFrameSpeed = 10.f;
			m_fFrameEnd = 24;
			m_eCurLook = LOOK_DOWN;
			break;
		}
		m_fFrame = 0.f;
		m_ePreState = m_eCurState;
	}
}


void CDeerClops::Sleep(const _float& fTimeDelta)
{
	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0.f;
	}
}

void CDeerClops::First_Phase(const _float& fTimeDelta)
{

	if (m_ePreState == SLEEP_PST && m_fFrameEnd < m_fFrame)
	{
		m_eCurState = IDLE;
	}
	else if (m_ePreState == IDLE && m_fFrameEnd < m_fFrame)	//포효 지르기
	{
		m_eCurState = TAUNT;		//여기서 카메라 쉐이킹

	}
	else if (m_ePreState == TAUNT && m_fFrameEnd < m_fFrame)
	{
		m_eCurState = WALK;
		m_bPhase[SECOND] = true;
		Engine::PlayBGM(L"RoadScene.mp3", 0.5f);
	}



	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0.f;
	}
}
void CDeerClops::Second_Phase(const _float& fTimeDelta)
{
	m_fCollisionRadius = 0.7f;
	m_fAcctime += fTimeDelta;
	m_Stat.fSpeed = 4.0f;

	if (3.f < m_fAcctime)
	{
		m_fAcctime = 0.f;
		if (m_bAttackCooltime)
			m_bAttackCooltime = false;
	}

	if (Collision_Circle(Get_Player_Pointer()) &&
		(m_Stat.fATKRange) &&
		!m_bAttackCooltime &&
		!dynamic_cast<CPlayer*>(Get_Player_Pointer())->IsPlayer_Dead())
	{
		m_eCurState = ATTACK;
		m_bAttackCooltime = true;
	}
	else if (Collision_Circle(Get_Player_Pointer()) && m_bAttackCooltime)
	{
		if (m_fFrameEnd < m_fFrame)
		{
			m_eCurState = IDLE;
		}
	}
	else if (m_ePreState == IDLE)
	{
		if (!IsTarget_Approach(m_Stat.fATKRange))
		{
			m_eCurState = WALK;
		}
	}
	else if (m_ePreState == HIT)
	{
		if (m_fFrameEnd < m_fFrame)
		{
			m_eCurState = WALK;
		}
	}
	else if (m_ePreState == ATTACK)
	{
		if (9 < m_fFrame && !m_bAttacking)
		{
			Attack_Sound();
			// 플레이어 피격
			if (IsTarget_Approach(2.f))
			{
				dynamic_cast<CPlayer*>(Get_Player_Pointer())->Set_Attack(200.f);
			}
			m_bAttacking = true;
		}

		if (!IsTarget_Approach(m_Stat.fATKRange))
		{
			if (m_fFrameEnd < m_fFrame)
			{
				m_eCurState = WALK;
			}
		}
	}
	else if (m_ePreState == WALK)
	{
		Player_Chase(fTimeDelta);
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

void CDeerClops::Third_Phase(const _float& fTimeDelta) //보스 스테이지에서 등장씬
{
	if (m_ePreState == FALL_DOWN)
	{
		if (!m_bFalldown)
		{
			m_pTransForm->Move_Pos(&m_vFallingDir, 60.f, fTimeDelta);
			if (Get_Pos().y < 2.5f)
			{

				m_pTransForm->Set_Pos(m_vPos);
				m_bFalldown = true;
				Generate_Roaring(0.9);
				Getnerate_SnowSplash();
				Camera_Shaking(3.f, 1.f, false);
				Engine::PlaySound_W(L"Obj_Deerclops_IceattackRand_1.mp3", SOUND_DEERCLOPS, 0.5f);
				Engine::PlayBGM(L"music [3].mp3", 0.5f);
				Awake_Tallbird();
				// 여기에 카메라 쉐이킹
			}
			else if (Get_Pos().y < 6.f)
			{
				m_bFrameStop = false;
			}
		}

		if (IsFrameEnd() && m_bFalldown)
		{
			m_eCurState = WAKE_UP;
		}

	}
	else if (m_ePreState == WAKE_UP)
	{
		if (IsFrameEnd())
		{
			m_eCurState = TAUNT;
		}
	}
	else if (m_ePreState == TAUNT)
	{
		if (IsFrameEnd())
		{
			m_eCurState = IDLE;
		}
	}
	else if (m_ePreState == IDLE)
	{
		if (IsFrameEnd())
		{
			m_fFrame = 0.f;
			m_fAcctime = 0.f;
			m_eCurState = WALK;
			m_bPhase[FOURTH] = true;
			if (m_bSound)
				m_bSound = false;
			if (m_bAttacking)
				m_bAttacking = false;
			if (m_bHit)
				m_bHit = false;
		}
	}

}

void CDeerClops::Fourth_Phase(const _float& fTimeDelta)		//따라오면서 일반 공격
{

	m_fAcctime += fTimeDelta;

	m_Stat.fSpeed = 1.5f;
	m_fCollisionRadius = 1.2;
	if (3.f < m_fAcctime)
	{
		m_fAcctime = 0.f;
		if (m_bAttackCooltime)
			m_bAttackCooltime = false;
	}

	if (Collision_Circle(Get_Player_Pointer()) && !m_bAttackCooltime)
	{
		m_eCurState = ATTACK;
		m_bAttackCooltime = true;
	}
	else if (Collision_Circle(Get_Player_Pointer()) && m_bAttackCooltime)
	{
		if (m_fFrameEnd < m_fFrame)
		{
			m_eCurState = IDLE;
		}
	}
	else if (m_ePreState == IDLE)
	{
		if (!Collision_Circle(Get_Player_Pointer()))
		{
			m_eCurState = WALK;
		}
	}
	else if (m_ePreState == HIT)
	{
		if (IsFrameEnd())
			m_eCurState = WALK;
	}
	else if (m_ePreState == ATTACK)
	{
		if (9 < m_fFrame)
		{
			if (!m_bSound)
			{
				Attack_Sound();
				m_bSound = true;
			}

			if (IsTarget_Approach(2.5) && !m_bAttacking)
			{
				dynamic_cast<CPlayer*>(Get_Player_Pointer())->Set_Attack((int)m_Stat.fATK);
				m_bAttacking = true;
			}
		}

		if (!Collision_Circle(Get_Player_Pointer()))
		{
			if (m_fFrameEnd < m_fFrame)
			{
				m_eCurState = WALK;
			}
		}
	}
	else if (m_ePreState == WALK)
	{
		Player_Chase(fTimeDelta);
	}

	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0.f;
		if (m_bAttacking)
			m_bAttacking = false;
		if (m_bSound)
			m_bSound = false;
		if (m_bHit)
			m_bHit = false;
	}

}

void CDeerClops::Fifth_Phase(const _float& fTimeDelta)		//제작한 패턴 나옴
{
	m_Stat.fSpeed = 3.5f;
	m_fCollisionRadius = 1.2f;
	m_fSkill2 += fTimeDelta;
	m_fAcctime2 += fTimeDelta;

	if (20.f < m_fSkill2 && !m_bGimmick)
	{
		m_bGimmick = true;
		int randomvalue = rand() % 2;
		switch (randomvalue)
		{
		case 0:
			m_eCurState = LONG_TAUNT;
			break;
		case 1:
			m_eCurState = PATTERN_PRE;
			break;

		}
		m_fSkill2 = 0.f;
	}

	if (m_fAcctime2 > 3.f)
	{
		m_fAcctime2 = 0.f;
		m_bAttackCooltime = false;
	}







	if (m_ePreState == LONG_TAUNT) //1패턴
	{
		m_fAcctime += fTimeDelta;
		m_fSkill += fTimeDelta;
		if (0.3 < m_fSkill)
		{
			Generate_Fall_Mark();
			m_fSkill = 0.f;
		}

		if (5 < m_fAcctime)
		{
			Generate_Fall_Mark_Ex();
			if (m_bGimmick)
				m_bGimmick = false;

			m_eCurState = WALK;
			m_fAcctime = 0.f;
		}
	}
	else if (m_ePreState == PATTERN_PRE)	//2패턴
	{
		if (IsFrameEnd())
		{
			m_eCurState = PATTERN_LOOP;
			Camera_Shaking(0.5f, 4.f, false);
			Engine::PlaySound_W(L"earthqu.mp3", SOUND_DEERCLOPS, 0.6f);
			//지진 소리를 구한다면 여기에 넣어줘
		}
	}
	else if (m_ePreState == PATTERN_LOOP)
	{

		if (IsFrameEnd())
		{
			Generate_Sizemic(); 
			Camera_Shaking(3.f, 0.5f, false);
			m_eCurState = WAKE_UP;
		}
	}
	else if (m_ePreState == WAKE_UP)
	{
		if (IsFrameEnd())
		{
			m_eCurState = WALK;
			if (m_bGimmick)
				m_bGimmick = false;
		}
	}


	else if (m_ePreState == WALK)
	{
		Player_Chase(fTimeDelta);
		if (Collision_Circle(Get_Player_Pointer()) && !m_bAttackCooltime)
		{
			m_eCurState = ATTACK;
			Attack_Sound();
			m_bAttackCooltime = true;
		}
		else if (Collision_Circle(Get_Player_Pointer()) && m_bAttackCooltime)
		{
			m_eCurState = IDLE;
		}
	}
	else if (m_ePreState == IDLE)
	{
		if (Collision_Circle(Get_Player_Pointer()) && !m_bAttackCooltime)
		{
			m_eCurState = ATTACK;
			m_bAttackCooltime = true;
		}
		else if (!Collision_Circle(Get_Player_Pointer()))
		{
			if (IsFrameEnd())
				m_eCurState = WALK;
		}
	}
	else if (m_ePreState == ATTACK)
	{
		if (9 < m_fFrame && !m_bAttacking)
		{
			Generate_Attack_Effect();
			m_bAttacking = true;
		}

	}
	else if (m_ePreState == HIT)
	{
		if (m_fFrameEnd < m_fFrame)
		{
			m_eCurState = WALK;
		}
	}



	if (IsFrameEnd())
	{
		m_fFrame = 0.f;
		if (m_bAttacking)
			m_bAttacking = false;
		if (m_bSound)
			m_bSound = false;
		if (m_bHit)
			m_bHit = false;
		if (m_ePreState == ATTACK)
		{
			m_eCurState = WALK;
		}
	}
}

void CDeerClops::Boss_Die(const _float& fTimeDelta)
{
	if (m_ePreState == DEAD && m_fFrameEnd - 1 < m_fFrame)
	{
		m_fFrame = m_fFrameEnd - 1;
		m_bFrameStop = true;

		// 엔딩 메소드 추가
	}

}

void CDeerClops::IsBossStage()
{
	if (scenemgr::Get_CurScene()->Get_Scene_Name() == L"BOSS" && !m_bPhase[THIRD])	//보스 스테이지 일 경우 1회진입
	{
		m_bPhase[THIRD] = true;
		_vec3 vPos = m_vPos;
		_vec3 vUp = { 0,1,0 };
		vPos += vUp * 100.f;
		m_vFallingDir = m_vPos - vPos;
		D3DXVec3Normalize(&m_vFallingDir, &m_vFallingDir);
		m_pTransForm->Set_Pos(vPos);
		m_eCurState = FALL_DOWN;
		m_bFrameStop = true;
	}
}

_vec3 CDeerClops::Get_Pos()
{
	_vec3 vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	return vPos;
}

_vec3 CDeerClops::Get_Right()
{
	_vec3 vRight;
	m_pTransForm->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);
	return vRight;
}

_vec3 CDeerClops::Get_Look()
{
	_vec3 vLook;
	m_pTransForm->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	return vLook;
}

_vec3 CDeerClops::Get_Up()
{
	_vec3 vUp;
	m_pTransForm->Get_Info(INFO_UP, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);
	return vUp;
}

_bool CDeerClops::IsFrameEnd()
{
	if (m_fFrameEnd < m_fFrame)
		return true;
	else
		return false;

}

void CDeerClops::Generate_Fall_Mark()
{
	_vec3 pPos = Get_Player_Pos();
	pPos.y = 0.1f;
	CGameObject* pGameObject = FallMark::Create(m_pGraphicDev, pPos);
	CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObject);
}

void CDeerClops::Generate_Fall_Mark_Ex()
{
	srand(unsigned(time(NULL)));
	_vec3 vDir;

	for (int i = 0; i < 20; ++i)
	{
		_vec3 pPos = Get_Player_Pos();
		pPos.y = 0.1f;
		int randomValue = rand() % 10;
		int randomValue2 = rand() % 10;
		// 부호를 무작위로 선택 (-1 또는 1)
		int sign = (rand() % 2 == 0) ? 1 : -1;
		int sign2 = (rand() % 2 == 0) ? 1 : -1;

		// 랜덤값에 부호를 적용
		int result = randomValue * sign;
		int result2 = randomValue2 * sign2;

		pPos.x += (float)result;
		pPos.z += (float)result2;
		CGameObject* pGameObject = FallMark::Create(m_pGraphicDev, pPos);
		CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObject);
	}
}

void CDeerClops::Generate_Attack_Effect()
{
	Engine::PlaySound_W(L"Obj_Deerclops_Attack_4.mp3", SOUND_DEERCLOPS, 0.3f);
	_vec3 vThisPos, vRight, vLook; // 이펙트 생성 기점.
	m_pTransForm->Get_Info(INFO_POS, &vThisPos);
	m_pTransForm->Get_Info(INFO_LOOK, &vLook);
	m_pTransForm->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vThisPos.y = 0.5f;
	switch (m_ePreLook)
	{
	case LOOK_DOWN:
		vThisPos -= vLook * 2;
		vThisPos -= vRight * 3;
		break;
	case LOOK_UP:
		vThisPos += vLook * 3;
		vThisPos += vRight * 3;
		break;
	case LOOK_LEFT:
		vThisPos -= vRight * 3;
		break;
	case LOOK_RIGHT:
		vThisPos += vRight * 3;
		break;
	}

	CGameObject* pGameObject = CBossEftDown::Create(m_pGraphicDev, vThisPos);
	NULL_CHECK_RETURN(pGameObject, );
	CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObject);
	dynamic_cast<CBossEftDown*>(pGameObject)->Set_Look_Dir(m_ePreLook);
}

void CDeerClops::Generate_Roaring(_int _iCount)
{
	_vec3 vThisPos, vLook;
	vThisPos.y = 0.1f;
	vLook = Get_Look();
	vLook *= -1;
	vThisPos += vLook * (m_pTransForm->Get_Scale().y * 0.5);

	m_pTransForm->Get_Info(INFO_POS, &vThisPos);
	CGameObject* pGameObject = CCircle::Create(m_pGraphicDev, vThisPos);
	NULL_CHECK_RETURN(pGameObject, );
	CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObject);
	dynamic_cast<CCircle*>(pGameObject)->Set_Count(_iCount);
}

void CDeerClops::Getnerate_SnowSplash()
{
	srand(unsigned(time(NULL)));
	for (int i = 0; i < 10; ++i)
	{
		_vec3 pPos = Get_Pos();
		pPos.y = 0.2f;
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

void CDeerClops::Generate_Sizemic()
{
	_vec3 vThisPos, vRight, vLeft, vDown, vUp; // 이펙트 생성 기점.
	_vec3 vDotRD, vDotRU, vDotLD, vDotLU;
	vThisPos = Get_Pos();
	vThisPos.y = 1.f;
	vRight = Get_Right();
	vUp = Get_Look();
	vLeft = vRight * -1;
	vDown = vUp * -1;

	vDotRD = vRight + vDown;
	vDotRU = vRight + vUp;
	vDotLD = vLeft + vDown;
	vDotLU = vLeft + vUp;
	D3DXVec3Normalize(&vDotRD, &vDotRD);
	D3DXVec3Normalize(&vDotRU, &vDotRU);
	D3DXVec3Normalize(&vDotLD, &vDotLD);
	D3DXVec3Normalize(&vDotLU, &vDotLU);


	for (int i = 0; i < 3; ++i)
	{
		//right
		_float fDistance = i * 4.f;
		if (i == 0)
			fDistance = 4.f;

		_vec3 rightpos = vThisPos + vRight * fDistance;
		CGameObject* pGameObjectR = SizemicR::Create(m_pGraphicDev, rightpos);
		CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObjectR);
		dynamic_cast<SizemicR*>(pGameObjectR)->Set_Number(i);

		_vec3 Rdpos = vThisPos + vDotRD * fDistance;
		CGameObject* pGameObjectRD = SizemicR::Create(m_pGraphicDev, Rdpos);
		CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObjectRD);
		dynamic_cast<SizemicR*>(pGameObjectRD)->Set_Number(i);

		_vec3 Rupos = vThisPos + vDotRU * fDistance;
		CGameObject* pGameObjectRU = SizemicR::Create(m_pGraphicDev, Rupos);
		CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObjectRU);
		dynamic_cast<SizemicR*>(pGameObjectRU)->Set_Number(i);



		//left
		_vec3 leftpos = vThisPos + vLeft * fDistance;
		CGameObject* pGameObjectL = SizemicL::Create(m_pGraphicDev, leftpos);
		CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObjectL);
		dynamic_cast<SizemicL*>(pGameObjectL)->Set_Number(i);

		_vec3 Ldpos = vThisPos + vDotLD * fDistance;
		CGameObject* pGameObjectLD = SizemicL::Create(m_pGraphicDev, Ldpos);
		CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObjectLD);
		dynamic_cast<SizemicL*>(pGameObjectLD)->Set_Number(i);

		_vec3 Lupos = vThisPos + vDotLU * fDistance;
		CGameObject* pGameObjectLU = SizemicL::Create(m_pGraphicDev, Lupos);
		CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObjectLU);
		dynamic_cast<SizemicL*>(pGameObjectLU)->Set_Number(i);
	}
	Getnerate_SnowSplash();
}

void CDeerClops::When_Walking()
{
	if (m_ePreState == WALK)
	{
		if (8 < m_fFrame && 13 > m_fFrame && !m_bSound)
		{
			Step_Sound();
			m_bSound = true;
		}

		if (m_fFrameEnd - 1 < m_fFrame && !m_bSound)
		{
			Step_Sound();
			m_bSound = true;
		}

	}
}

void CDeerClops::When_IDLE()
{
	if (10 < m_fFrame && !m_bSound)
	{
		IDLE_Sound();
		m_bSound = true;
	}
}

void CDeerClops::Camera_Shaking(_float _power, _float _time, _bool _dir)
{
	if (scenemgr::Get_CurScene()->Get_Scene_Name() == L"BOSS")
	{
		CDynamicCamera* pCamera = dynamic_cast<CDynamicCamera*>(scenemgr::Get_CurScene()->GetCameraObject());
		pCamera->SetShakedCamera(_power, _time, _dir);
	}
	else
	{
		CRoadSceneCamera* pCamera = dynamic_cast<CRoadSceneCamera*>(scenemgr::Get_CurScene()->GetCameraObject());
		pCamera->SetShakedCamera(_power, _time, _dir);
	}
}

void CDeerClops::Hit_Sound()
{
	int randomvalue = rand() % 6;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"Obj_Deerclops_Hurt_1.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 1:
		Engine::PlaySound_W(L"Obj_Deerclops_Hurt_2.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 2:
		Engine::PlaySound_W(L"Obj_Deerclops_Hurt_3.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 3:
		Engine::PlaySound_W(L"Obj_Deerclops_Hurt_4.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 4:
		Engine::PlaySound_W(L"Obj_Deerclops_Hurt_5.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 5:
		Engine::PlaySound_W(L"Obj_Deerclops_Hurt_6.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	}
}

void CDeerClops::Attack_Sound()
{
	int randomvalue = rand() % 5;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"Obj_Deerclops_Attack_1.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 1:
		Engine::PlaySound_W(L"Obj_Deerclops_Attack_2.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 2:
		Engine::PlaySound_W(L"Obj_Deerclops_Attack_3.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 3:
		Engine::PlaySound_W(L"Obj_Deerclops_Attack_4.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 4:
		Engine::PlaySound_W(L"Obj_Deerclops_Attack_5.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	}
}

void CDeerClops::Step_Sound()
{
	int randomvalue = rand() % 4;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"Obj_Deerclops_Step_1.mp3", SOUND_DEERCLOPS, 0.4f);
		break;
	case 1:
		Engine::PlaySound_W(L"Obj_Deerclops_Step_2.mp3", SOUND_DEERCLOPS, 0.4f);
		break;
	case 2:
		Engine::PlaySound_W(L"Obj_Deerclops_Step_3.mp3", SOUND_DEERCLOPS, 0.4f);
		break;
	case 3:
		Engine::PlaySound_W(L"Obj_Deerclops_Step_4.mp3", SOUND_DEERCLOPS, 0.4f);
		break;

	}
}

void CDeerClops::IDLE_Sound()
{
	int randomvalue = rand() % 5;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"Obj_Deerclops_Idle_1.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 1:
		Engine::PlaySound_W(L"Obj_Deerclops_Idle_2.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 2:
		Engine::PlaySound_W(L"Obj_Deerclops_Idle_3.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 3:
		Engine::PlaySound_W(L"Obj_Deerclops_Idle_4.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	case 4:
		Engine::PlaySound_W(L"Obj_Deerclops_Idle_5.mp3", SOUND_DEERCLOPS, 0.2f);
		break;
	}
}



void CDeerClops::Awake_Tallbird()
{
	_vec3 vThispos;
	vThispos = m_pTransForm->Get_Pos();
	auto& vecObj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::MONSTER);
	for (auto& obj : vecObj)
	{
		if (obj->IsDelete() || obj == nullptr || dynamic_cast<CMonster*>(obj)->Get_Name() != L"키다리새")
			continue;
		else
			dynamic_cast<CTallbird*>(obj)->Set_Tallbird_WakeUp();
	}
}


void CDeerClops::Free()
{
	__super::Free();
}

CDeerClops* CDeerClops::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
	CDeerClops* pInstance = new CDeerClops(pGraphicDev, _vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Boss Create Failed");
		return nullptr;
	}

	return pInstance;
}
