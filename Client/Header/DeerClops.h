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
	void			Set_WakeUp();				//1������ ���� Ʈ����
	void			Set_Phase(_bool _first, _bool _second, _bool _third, _bool _fourth,_bool fifth, _bool sixth) {
		m_bPhase[FIRST] = _first;
		m_bPhase[SECOND] = _second;
		m_bPhase[THIRD] = _third;
		m_bPhase[FOURTH] = _fourth;
		m_bPhase[FIFTH] = fifth;
		m_bPhase[DIE] = sixth;
	}			//����� ���������� Ŭ���� �ۿ��� �������� �� �ִ� �Լ�

private:
	virtual HRESULT			Add_Component();
	virtual void	Set_Hit()						override;
	virtual void	Set_ObjStat();
	virtual void	State_Change()				override;



	void			Sleep(const _float& fTimeDelta);		//ù �����ϱ� �� �ڰ�����
	void			First_Phase(const _float& fTimeDelta);		//������ �� ����
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

	void			Generate_Fall_Mark();   //�÷��̾���ġ�� ������ �����ϴ� �Լ�
	void			Generate_Fall_Mark_Ex();	//������ ���� �����ϴ� �Լ�
	void			Generate_Attack_Effect();		//�⺻ ���� ����Ʈ ���� �Լ�
	void			Generate_Roaring(_int _iCount);				// ��ȿ ������ ����Ʈ ���� �Լ�
	void			Getnerate_SnowSplash();
	void			Generate_Sizemic();		//2����

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
	_bool		m_bPhase[DEER_PHASE::PHASE_END];		//������ ����� bool �迭

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

