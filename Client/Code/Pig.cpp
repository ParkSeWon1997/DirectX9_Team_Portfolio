#include "stdafx.h"
#include "Pig.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include"ResObject.h"
CPig::CPig(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
	:CMonster(pGraphicDev, _vPos), m_eCurState(IDLE), m_ePreState(STATE_END)
{
}

CPig::CPig(const CPig& rhs)
	:CMonster(rhs)
{
}

CPig::~CPig()
{
}

HRESULT CPig::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Set_Pos(m_vPos);
	m_pTransForm->Set_Scale({ 1.5f,1.5f,1.5f });
	Set_ObjStat();
	m_fFrameEnd = 7;
	m_fFrameChange = rand() % 5;
	m_fCollisionRadius = 0.5f;
	Look_Change();
	return S_OK;
}

_int CPig::Update_GameObject(const _float& fTimeDelta)
{
	Volume_Controll();
	if (!m_bFrameStop)
		m_fFrame += m_fFrameSpeed * fTimeDelta;
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
	}

	CGameObject::Update_GameObject(fTimeDelta);
	State_Change();
	Look_Change(); 
	//Set_Scale();
	
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return iResult;
}

void CPig::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

	m_pTransForm->BillBoard();
	_vec3	vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CPig::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
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

		FAILED_CHECK_RETURN(SetUp_Material(), );
		m_pTextureCom[m_ePreLook][m_ePreState]->Set_Texture((_uint)m_fFrame);

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

HRESULT CPig::Add_Component()
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
	 pComponent = m_pTextureCom[LOOK_DOWN][HAPPY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_happy"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_happy", pComponent });


	 //IDLE
	 pComponent = m_pTextureCom[LOOK_DOWN][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_idle_down"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_idle_down", pComponent });

	 pComponent = m_pTextureCom[LOOK_UP][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_idle_up"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_idle_up", pComponent });

	 pComponent = m_pTextureCom[LOOK_LEFT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_idle_side"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_idle_side", pComponent });

	 pComponent = m_pTextureCom[LOOK_RIGHT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_idle_side"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_idle_side", pComponent });

	 //WALK
	 pComponent = m_pTextureCom[LOOK_DOWN][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_walk_down"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_walk_down", pComponent });

	 pComponent = m_pTextureCom[LOOK_UP][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_walk_up"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_walk_up", pComponent });

	 pComponent = m_pTextureCom[LOOK_LEFT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_walk_side"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_walk_side", pComponent });

	 pComponent = m_pTextureCom[LOOK_RIGHT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_walk_side"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_walk_side", pComponent });

	 //RUN
	 pComponent = m_pTextureCom[LOOK_DOWN][RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_run_down"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_run_down", pComponent });

	 pComponent = m_pTextureCom[LOOK_UP][RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_run_up"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_run_up", pComponent });

	 pComponent = m_pTextureCom[LOOK_LEFT][RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_run_side"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_run_side", pComponent });

	 pComponent = m_pTextureCom[LOOK_RIGHT][RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_run_side"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_run_side", pComponent });

	 //ATTACK
	 pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_atk_down"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_atk_down", pComponent });

	 pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_atk_up"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_atk_up", pComponent });

	 pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_atk_side"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_atk_side", pComponent });

	 pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_atk_side"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_atk_side", pComponent });

	 //EAT
	 pComponent = m_pTextureCom[LOOK_DOWN][EAT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_eat"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_eat", pComponent });

	 //HIT
	 pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_hit"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_hit", pComponent });

	 pComponent = m_pTextureCom[LOOK_DOWN][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_hit"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_hit", pComponent });

	 //DEAD
	 pComponent = m_pTextureCom[LOOK_DOWN][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_dead"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_dead", pComponent });

	 //ANGRY
	 pComponent = m_pTextureCom[LOOK_DOWN][ANGRY_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_angry"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_mapComponent[ID_STATIC].insert({ L"Proto_Pig_angry", pComponent });

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


void CPig::Set_ObjStat()
{
	m_Stat.strObjName = L"돼지";
	m_Stat.fHP = 100.f;
	m_Stat.fMxHP = 100.f;
	m_Stat.fSpeed = 1.f;
	m_Stat.fATK = 10.f;
	m_Stat.bDead = false;
	m_Stat.fAggroRange = 7.f;
	m_Stat.fATKRange = 1.5f;
}

void CPig::Player_Chase(const _float& fTimeDelta)
{
	_vec3 PlayerPos;
	PlayerPos = Get_Player_Pos();
	m_eCurLook = m_pTransForm->Chase_Target_Monster(&PlayerPos, m_Stat.fSpeed, fTimeDelta);
	Look_Change();
}

void CPig::State_Change()
{
	
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			//Engine::PlaySound_W
			//Engine::PlaySound_W(L"Obj_Pig_Oink_1.mp3",  SOUND_PIG,0.2f);
			m_fFrameSpeed = 8.f;
			m_fFrameEnd = 7;
			break;
		case HAPPY:
			m_fFrameEnd = 17;
			m_fFrameSpeed = 17.f;
			m_eCurLook = LOOK_DOWN;
			break;
		case ANGRY_IDLE:
			m_fFrameEnd = 17;
			m_fFrameSpeed = 17.f;
			m_eCurLook = LOOK_DOWN;
			break;
		case WALK:
			m_fFrameSpeed = 10.f;
			m_fFrameEnd = 8;
			break;
		case EAT:
			m_fFrameEnd = 16;
			m_fFrameSpeed = 14.f;
			m_eCurLook = LOOK_DOWN;
			break;
		case RUN:
			m_fFrameSpeed = 8.f;
			m_fFrameEnd = 6;
			break;
		case ATTACK:
			Oink_Sound();
			m_fFrameSpeed = 11.f;
			m_fFrameEnd = 10;
			break;
		case SLEEP:
			m_eCurLook = LOOK_DOWN;
			break;
		case HIT:
			Oink_Sound();
			if (m_eCurLook != LOOK_LEFT)
			{
				m_eCurLook = LOOK_DOWN;
			}
			m_fFrameEnd = 6;
			m_fFrameSpeed = 12.f;
			break;
		case DEAD:
			Dead_Sound();
			m_fFrameEnd = 11;
			m_fFrameSpeed = 12.f;
			m_eCurLook = LOOK_DOWN;
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

_int CPig::Die_Check()
{
	if (m_Stat.fHP <= 0 && m_ePreState != DEAD && m_ePreState != ERASE)
	{
		m_eCurState = DEAD;
		m_Stat.bDead = true;
		m_fFrame = 0.f;
	}
	else if (m_ePreState == DEAD)
	{
		

		if (m_fFrameEnd < m_fFrame)
		{
			
			CResObject::CreateItem(L"PigTail", this, m_pGraphicDev);

			m_eCurState = ERASE;
		}
	}
	else if (m_ePreState == ERASE)
	{
		if ((m_fFrameEnd-1) <= m_fFrame)
		{
			m_bFrameStop = true;
			DeleteObject(this);
			return 0x80000000;
		}
	}
	return 0;
}

void CPig::Attacking(const _float& fTimeDelta)
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
			if (5 < m_fFrame && Collision_Circle(Get_Player_Pointer()) && !m_bAttacking)
			{
				dynamic_cast<CPlayer*>(Get_Player_Pointer())->Set_Attack(m_Stat.fATK);
				m_bAttacking = true;
			}
			else if ((m_fFrameEnd < m_fFrame) && !Collision_Circle(Get_Player_Pointer()))
			{
				m_eCurState = RUN;
			}
		}
		else if (m_ePreState == RUN && !Collision_Circle(Get_Player_Pointer()))
		{
			Player_Chase(fTimeDelta);
			
		}
		else if (!IsTarget_Approach(m_Stat.fATKRange))
		{
			m_eCurState = RUN;
		}
	}
	else
	{
		if (m_fFrameEnd < m_fFrame)
		{
			Engine::PlaySound_W(L"Obj_Pig_Oink_2.mp3", SOUND_PIG, m_fVolume);
			m_eCurState = RUN;
			m_bHit = false;
		}
	}

	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0.f;
		if (m_bAttacking)
			m_bAttacking = false;
	}
}

void CPig::Patroll(const _float& fTimeDelta)
{
	auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();
	CTerrainTex* pTerrainTex = dynamic_cast<CTerrainTex*>(pTerrain->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	m_fAcctime += fTimeDelta;
	m_Stat.fSpeed = 1.f;
	if (m_fFrameChange < m_fAcctime)
	{
		m_fAcctime = 0.f;
		m_fFrameChange = rand() % 16;
		int RandomPattern = rand() % 3;
		m_eCurState = (PiGSTATE)RandomPattern;
		if (m_eCurState == WALK)
		{
			int randomValue = rand() % 360;
			int randomValue2 = rand() % 360;
			int sign = (rand() % 2 == 0) ? 1 : -1;
			int sign2 = (rand() % 2 == 0) ? 1 : -1;
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

void CPig::Set_Hit()
{
	m_eCurState = HIT;
	m_bHit = true;
}

void CPig::Dead_Sound()
{
	int randomvalue = rand() % 2;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"Obj_Pig_Death_1.mp3", SOUND_PIG, m_fVolume);
		break;
	case 1:
		Engine::PlaySound_W(L"Obj_Pig_Death_2.mp3", SOUND_PIG, m_fVolume);
		break;
	}
}

void CPig::Oink_Sound()
{
	int randomvalue = rand() % 2;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"Obj_Pig_Oink_1.mp3", SOUND_PIG, m_fVolume);
		break;
	case 1:
		Engine::PlaySound_W(L"Obj_Pig_Oink_2.mp3", SOUND_PIG, m_fVolume);
		break;
	}
}


CPig* CPig::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
	CPig* pInstance = new CPig(pGraphicDev, _vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Pig Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPig::Free()
{
	__super::Free();
}
