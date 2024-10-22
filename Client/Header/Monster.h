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
	void Set_Attack(_float _fAttack);		//���ݴ����� �� �Ҹ��� �Լ�

public:
	OBJSTAT Get_Monster_Stat() { return m_Stat; }
	_tchar* Get_Name() { return m_Stat.strObjName; }
	_bool		IsTarget_Approach(float _fDistance);	//�÷��̾���� �Ÿ��� ����ϱ� ���� �Լ�.
														//�Ű������� ���� float ���� �Ÿ����͸� ���Ѵ�
protected:
	virtual void Free() override;
	CGameObject* Get_Player_Pointer();								//�÷��̾��� �����͸� �޾ƿ��� ���� �Լ�
	void	Volume_Controll();
protected:
	virtual void		Set_ObjStat() PURE;
	virtual void		State_Change() PURE;
	virtual void		Set_Hit()PURE;


	void		Collision_EachOther(const _float& fTimeDelta);
	_bool		Collision_Transform(CTransform* _Src, CTransform* _Dst, const _float& fTimeDelta, const _float& fSpeed);

	void			Player_Chase(const _float& fTimeDelta);		//�÷��̾ �߰��԰� ���ÿ� ���⵵ �������ش�
	void		Look_Change();
	_vec3		Get_Player_Pos();									//�÷��̾��� ������ ���͸� �޾ƿ��� �Լ�
protected:
	_vec3		m_vPos;	//��ġ �����
	_vec3		m_vDir; //���� �̵����� ����
	LOOKDIR		m_eCurLook;			//���� �ٶ󺸴� ����
	LOOKDIR		m_ePreLook;
	_bool		m_Dirchange; //false �϶� ������ ����
	_bool		m_Attacked;	//���ݴ��ߴ��� �ƴ��� �˻��ϴ� ����
	_bool		m_bAttacking; //���� �ߴ��� ���޴��� �˻��ϴ� ����
	_bool		m_bHit;		//�¾����� Ȯ���ϴ� ����

	_bool				m_bSound;	//�Ҹ������



	//������
	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;
	_float				m_fAcctime;				//�ð� ������ ����
	_int				m_fFrameChange = 0;		//�������� �ٲ��� �ϴ� ��쿡 �����.
	_bool				m_bFrameStop;			//�������� ����� �� �� true�� �ٲ���
	_bool				m_bAttackCooltime;
	_float				m_fFrameSpeed;


	_float				m_fVolume;
	
	
};

