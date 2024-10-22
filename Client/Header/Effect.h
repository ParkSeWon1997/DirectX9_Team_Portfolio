#pragma once

#include "Base.h"
#include "GameObject.h"

class CEffect :	public Engine::CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vPos);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect();

protected:
	virtual void	State_Change()	PURE;
	virtual void	Look_Change();
	void Check_Collision();
	_bool		IsTarget_Approach(float _fDistance);
	void	Volume_Controll();

	void Spike_Sound();
protected:
	_vec3 m_vPos;

	LOOKDIR		m_eCurLook;			//현재 바라보는 방향
	LOOKDIR		m_ePreLook;
	//프레임
	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;
	_float				m_fAcctime;		
	_float				m_bFrameSpeed;		//시간 누적용 변수
	_int				m_fFrameChange = 0;		//프레임이 바뀌어야 하는 경우에 사용함.
	_bool				m_bFrameStop;			//프레임을 멈춰야 할 때 true로 바꿔줘
	_bool				m_Dirchange;

	_float				m_fDamage;
	_float				m_fVolume;
	_bool				m_bSound;
protected:
	virtual void Free() override;


};

