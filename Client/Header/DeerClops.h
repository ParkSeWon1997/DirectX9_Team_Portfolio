#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Monster.h"

namespace Engine 
{
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}

class CDeerClops :
	public CMonster
{
	enum DEERSTATE { IDLE, WALK, ATTACK, SLEEP,SLEEP_PST, TAUNT,LONG_TAUNT,FALL_DOWN, WAKE_UP,
		PATTERN_PRE, PATTERN_LOOP, HIT, DEAD, STATE_END };
	enum DEER_PHASE { FIRST, SECOND, THIRD, FOURTH,FIFTH, DIE, PHASE_END};
private:
	explicit CDeerClops(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CDeerClops(const CDeerClops& rhs);
	virtual ~CDeerClops();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;



public:
	void			Set_WakeUp();				//1페이즈 시작 트리거
	void			Set_Phase(_bool _first, _bool _second, _bool _third, _bool _fourth,_bool fifth, _bool sixth) {
		m_bPhase[FIRST] = _first;
		m_bPhase[SECOND] = _second;
		m_bPhase[THIRD] = _third;
		m_bPhase[FOURTH] = _fourth;
		m_bPhase[FIFTH] = fifth;
		m_bPhase[DIE] = sixth;
	}			//페이즈를 임의적으로 클래스 밖에서 조정해줄 수 있는 함수

private:
	virtual HRESULT			Add_Component();
	virtual void	Set_Hit()						override;
	virtual void	Set_ObjStat();
	virtual void	State_Change()				override;



	void			Sleep(const _float& fTimeDelta);		//첫 조우하기 전 자고있음
	void			First_Phase(const _float& fTimeDelta);		//페이즈 별 패턴
	void			Second_Phase(const _float& fTimeDelta);
	void			Third_Phase(const _float& fTimeDelta);
	void			Fourth_Phase(const _float& fTimeDelta);
	void			Fifth_Phase(const _float& fTimeDelta);
	void			Boss_Die(const _float& fTimeDelta);
	void			IsBossStage();
	_vec3			Get_Pos();
	_vec3			Get_Right();
	_vec3			Get_Look();
	_vec3			Get_Up();
	_bool			IsFrameEnd();

	void			Generate_Fall_Mark();   //플레이어위치에 마법진 생성하는 함수
	void			Generate_Fall_Mark_Ex();	//마법진 많이 생성하는 함수
	void			Generate_Attack_Effect();		//기본 공격 이펙트 생성 함수
	void			Generate_Roaring(_int _iCount);				// 포효 지르기 이펙트 생성 함수
	void			Getnerate_SnowSplash();
	void			Generate_Sizemic();		//2패턴

	void			When_Walking();
	void			When_IDLE();
	void			Camera_Shaking(_float _power, _float _time, _bool _dir);
	void			Hit_Sound();
	void			Attack_Sound();
	void			Step_Sound();
	void			IDLE_Sound();

	void			Awake_Tallbird();

public:
	static CDeerClops* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][DEERSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;
private:
	virtual void Free() override;
private:
	DEERSTATE		m_eCurState;
	DEERSTATE		m_ePreState;
	_bool		m_bPhase[DEER_PHASE::PHASE_END];		//페이즈 변경용 bool 배열

	_float		m_fSkill;
	_float		m_fSkill2;
	_float		m_fAcctime2;

	_bool		m_bFalldown;
	_vec3		m_vFallingDir;

	_vec3		m_vAttackPos;
	_vec3		m_vTauntPos;

	_bool		m_AttackPos;
	_bool		m_bGimmick;

};

