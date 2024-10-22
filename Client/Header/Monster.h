#pragma once
#include "Base.h"
#include "GameObject.h"
class CMonster :
    public Engine::CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	void Set_Attack(_float _fAttack);		//공격당했을 때 불리는 함수

public:
	OBJSTAT Get_Monster_Stat() { return m_Stat; }
	_tchar* Get_Name() { return m_Stat.strObjName; }
	_bool		IsTarget_Approach(float _fDistance);	//플레이어와의 거리를 계산하기 위한 함수.
														//매개변수로 받은 float 값과 거리벡터를 비교한다
protected:
	virtual void Free() override;
	CGameObject* Get_Player_Pointer();								//플레이어의 포인터를 받아오기 위한 함수
	void	Volume_Controll();
protected:
	virtual void		Set_ObjStat() PURE;
	virtual void		State_Change() PURE;
	virtual void		Set_Hit()PURE;


	void		Collision_EachOther(const _float& fTimeDelta);
	_bool		Collision_Transform(CTransform* _Src, CTransform* _Dst, const _float& fTimeDelta, const _float& fSpeed);

	void			Player_Chase(const _float& fTimeDelta);		//플레이어를 추격함과 동시에 방향도 변경해준다
	void		Look_Change();
	_vec3		Get_Player_Pos();									//플레이어의 포지션 벡터를 받아오는 함수
protected:
	_vec3		m_vPos;	//위치 저장용
	_vec3		m_vDir; //현재 이동중인 방향
	LOOKDIR		m_eCurLook;			//현재 바라보는 방향
	LOOKDIR		m_ePreLook;
	_bool		m_Dirchange; //false 일때 오른쪽 보기
	_bool		m_Attacked;	//공격당했는지 아닌지 검사하는 변수
	_bool		m_bAttacking; //공격 했는지 안햇는지 검사하는 변수
	_bool		m_bHit;		//맞았을때 확인하는 변수

	_bool				m_bSound;	//소리재생용



	//프레임
	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;
	_float				m_fAcctime;				//시간 누적용 변수
	_int				m_fFrameChange = 0;		//프레임이 바뀌어야 하는 경우에 사용함.
	_bool				m_bFrameStop;			//프레임을 멈춰야 할 때 true로 바꿔줘
	_bool				m_bAttackCooltime;
	_float				m_fFrameSpeed;


	_float				m_fVolume;
	
	
};

