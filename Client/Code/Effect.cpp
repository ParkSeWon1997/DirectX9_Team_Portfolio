#include "stdafx.h"
#include "..\Header\Effect.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "Monster.h"
#include "Player.h"
#include "ResObject.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vPos)
	: Engine::CGameObject(pGraphicDev)
	, m_vPos(vPos)
	, m_fFrameEnd(0.f)
	, m_fAcctime(0.f)
	, m_bFrameStop(false)
	, m_Dirchange(false)
	, m_eCurLook(LOOK_DOWN)
	, m_ePreLook(LOOK_END)
	, m_fDamage(0.f)
	, m_bFrameSpeed(0.f)
	, m_fVolume(0.f)
	, m_bSound(false)
{
}

CEffect::CEffect(const CEffect& rhs)
	: Engine::CGameObject(rhs)
	, m_vPos(rhs.m_vPos)
	, m_fFrameEnd(rhs.m_fFrameEnd)
	, m_fAcctime(rhs.m_fAcctime)
	, m_bFrameStop(rhs.m_bFrameStop)
	, m_Dirchange(rhs.m_Dirchange)
	, m_eCurLook(rhs.m_eCurLook)
	, m_ePreLook(rhs.m_ePreLook)
	, m_fDamage(rhs.m_fDamage)
	, m_bFrameSpeed(rhs.m_bFrameSpeed)
	, m_fVolume(rhs.m_fVolume)
	, m_bSound(rhs.m_bSound)
{

}

CEffect::~CEffect()
{
}

void CEffect::Look_Change()
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

void CEffect::Check_Collision()
{
	auto iter = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::MONSTER);
	for (auto find : iter)
	{
		if (find != nullptr)
		{
			if (Collision_Transform(this->m_pTransForm, find->GetTransForm()))
			{
				dynamic_cast<CMonster*>(find)->Set_Attack(m_fDamage);
			}
		}
	}

	auto obj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::RESOURCE_OBJECT);
	for (auto find : obj)
	{
		if (find != nullptr)
		{
			if (Collision_Transform(this->m_pTransForm, find->GetTransForm()))
			{
				dynamic_cast<CResObject*>(find)->Set_Attack();
			}
		}
	}

	auto player = scenemgr::Get_CurScene()->GetPlayerObject();
	if (player != nullptr)
	{
		if (Collision_Transform(this->m_pTransForm, player->GetTransForm()))
		{
			dynamic_cast<CPlayer*>(player)->Set_Attack((int)m_fDamage);
		}
	}
}

_bool CEffect::IsTarget_Approach(float _fDistance)
{
	_vec3 vTargetPos, vPos, vDir;
	vTargetPos = scenemgr::Get_CurScene()->GetPlayerObject()->GetTransForm()->Get_Pos();
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

void CEffect::Volume_Controll()
{
	if (IsTarget_Approach(15.f))
		m_fVolume = 0.08f;
	else
	{
		m_fVolume = 0.f;
		return;
	}

	if (IsTarget_Approach(10.f))
		m_fVolume = 0.09f;
	//
	if (!IsTarget_Approach(5.f))
		m_fVolume = 0.1f;
}

void CEffect::Spike_Sound()
{
	int randomvalue = rand() % 7;
	switch (randomvalue)
	{
	case 0:
		Engine::PlaySound_W(L"Obj_Deerclops_Iceattack_1.mp3", SOUND_EFFECT, m_fVolume);
		break;
	case 1:
		Engine::PlaySound_W(L"Obj_Deerclops_Iceattack_2.mp3", SOUND_EFFECT, m_fVolume);
		break;
	case 2:
		Engine::PlaySound_W(L"Obj_Deerclops_Iceattack_3.mp3", SOUND_EFFECT, m_fVolume);
		break;
	case 3:
		Engine::PlaySound_W(L"Obj_Deerclops_Iceattack_4.mp3", SOUND_EFFECT, m_fVolume);
		break;
	case 4:
		Engine::PlaySound_W(L"Obj_Deerclops_Iceattack_5.mp3", SOUND_EFFECT, m_fVolume);
		break;
	case 5:
		Engine::PlaySound_W(L"Obj_Deerclops_Iceattack_6.mp3", SOUND_EFFECT, m_fVolume);
		break;
	case 6:
		Engine::PlaySound_W(L"Obj_Deerclops_Iceattack_7.mp3", SOUND_EFFECT, m_fVolume);
		break;
	}
}





void CEffect::Free()
{	
	__super::Free();
}




