#include "stdafx.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Monster.h"
#include "Scene.h"


CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: Engine::CGameObject(pGraphicDev), m_vPos(vPos)
	, m_Dirchange(false)
	, m_eCurLook(LOOK_DOWN)
	, m_ePreLook(LOOK_END)
	, m_fAcctime(0.f)
	, m_vDir(0.f, 0.f, 0.f)
	, m_Attacked(false)
	, m_bFrameStop(false)
	, m_bHit(false)
	, m_bAttacking(false)
	, m_bAttackCooltime(false)
	, m_fFrameSpeed(0.f)
	, m_bSound(false)
	, m_fVolume(0.f)
{
}

CMonster::CMonster(const CMonster& rhs)
	: Engine::CGameObject(rhs)
	, m_vPos(rhs.m_vPos)
	, m_Dirchange(rhs.m_Dirchange)
	, m_eCurLook(rhs.m_eCurLook)
	, m_ePreLook(rhs.m_ePreLook)
	, m_fAcctime(rhs.m_fAcctime)
	, m_vDir(0.f, 0.f, 0.f)
	, m_Attacked(rhs.m_Attacked)
	, m_bFrameStop(rhs.m_bFrameStop)
	, m_bHit(rhs.m_bHit)
	, m_bAttacking(rhs.m_bAttacking)
	, m_bAttackCooltime(rhs.m_bAttackCooltime)
	, m_fFrameSpeed(rhs.m_fFrameSpeed)
	, m_bSound(rhs.m_bSound)
	, m_fVolume(rhs.m_fVolume)
{
}

CMonster::~CMonster()
{
}

void CMonster::Set_Attack(_float _fAttack)
{
	if (!m_Stat.bDead)
	{
		m_Stat.fHP -= _fAttack;
		if (!m_Attacked)
		{
			m_Attacked = true;		
		}
		Set_Hit();
	}
	else
		return;
}

void CMonster::Free()
{
	__super::Free();
}

CGameObject* CMonster::Get_Player_Pointer()
{
	CGameObject* pPlayer = scenemgr::Get_CurScene()->GetPlayerObject();
	return pPlayer;
}

void CMonster::Volume_Controll()
{
	if (IsTarget_Approach(15.f))
		m_fVolume = 0.15;
	else
	{
		m_fVolume = 0.f;
		return;
	}

	if (IsTarget_Approach(10.f))
		m_fVolume = 0.17;
	//
	if (!IsTarget_Approach(5.f))
		m_fVolume = 0.2f;
}

void CMonster::Collision_EachOther(const _float& fTimeDelta)
{
	auto iter = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::MONSTER);
	for (auto find : iter)
	{
		if (find == nullptr || find->IsDelete())
			continue;
		Collision_Transform(m_pTransForm, find->GetTransForm(), fTimeDelta, m_Stat.fSpeed);
	}
}

_bool CMonster::Collision_Transform(CTransform* _Src, CTransform* _Dst, const _float& fTimeDelta, const _float& fSpeed)
{
	_vec3 vSrc, vSrcScale, vDst, vDstScale,  vRDir;

	_Src->Get_Info(INFO_POS, &vSrc);
	vSrcScale = _Src->Get_Scale();

	_Dst->Get_Info(INFO_POS, &vDst);
	vDstScale = _Dst->Get_Scale();

	_float iDistanceX = fabs(vSrc.x - vDst.x);		
	_float fRadCX = vSrcScale.x * 0.5f + vDstScale.x * 0.5f;		

	_float fDistanceZ = fabs(vSrc.z - vDst.z);	//거리
	_float fRadCY = vSrcScale.y * 0.5f + vDstScale.y * 0.5f; // 스케일의 반

	vDst.y = 0;
	vSrc.y = 0;
	
	vRDir = vSrc - vDst;
	D3DXVec3Normalize(&vRDir, &vRDir);

	if (fDistanceZ > fRadCY || iDistanceX > fRadCX)
	{
		return false;
	}
	else
	{
		_Src->Move_Pos(&vRDir, fSpeed, fTimeDelta);
		return true;
	}
	
}

void CMonster::Player_Chase(const _float& fTimeDelta)
{
	_vec3 PlayerPos;
	PlayerPos = Get_Player_Pos();
	m_eCurLook = m_pTransForm->Chase_Target_Monster(&PlayerPos, m_Stat.fSpeed, fTimeDelta);
}

_bool CMonster::IsTarget_Approach(float _fDistance)
{
	_vec3 vTargetPos, vPos,vDir;
	vTargetPos = Get_Player_Pos();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	vTargetPos.y = 0.f;
	vPos.y = 0.f;
	if (D3DXVec3Length(&(vTargetPos - vPos)) < _fDistance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CMonster::Look_Change()
{
	if (m_ePreLook != m_eCurLook)
	{
		if (m_eCurLook == LOOK_LEFT)
		{
			m_Dirchange = true;
		}
		else
		{
			m_Dirchange = false;
		}

		m_ePreLook = m_eCurLook;
	}

}

_vec3 CMonster::Get_Player_Pos()
{
	auto pPlayer = dynamic_cast<CScene*>(scenemgr::Get_CurScene())->GetPlayerObject();
	Engine::CTransform* pPlayerTransformCom = pPlayer->GetTransForm();
	NULL_CHECK_RETURN(pPlayerTransformCom, _vec3());

	_vec3 PlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &PlayerPos);

	return PlayerPos;
}

