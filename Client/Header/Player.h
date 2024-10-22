#pragma once

#include "Base.h"
#include "GameObject.h"
#include "ResObject.h"

namespace Engine
{
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
	class CPlayerStatus;
}

class CPlayer : public Engine::CGameObject
{
	
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strName);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	_uint Get_PlayerHp() { return m_Stat.fHP; }
	_uint Get_PlayerMaxHp() { return m_Stat.fMxHP; }
	_uint Get_PlayerHungry() { return m_Stat.fHungry; }
	_uint Get_PlayerMaxHangry() { return m_Stat.fMaxHungry; }
	_uint Get_PlayerMental() { return m_Stat.fMental; }
	_uint Get_PlayerMaxMental() { return m_Stat.fMaxMental; }
	
	void Set_PlayerHp(_int _iHp) { if (m_Stat.fHP + _iHp >= m_Stat.fMxHP) m_Stat.fHP = m_Stat.fMxHP; else m_Stat.fHP += _iHp; }
	void Set_PlayerMental(_int _iMental) { if (m_Stat.fMental + _iMental >= m_Stat.fMaxMental) m_Stat.fMental = m_Stat.fMaxMental; else m_Stat.fMental += _iMental; }
	void Set_PlayerHangry(_int _iHangry) { if (m_Stat.fHungry + _iHangry >= m_Stat.fMaxHungry) m_Stat.fHungry = m_Stat.fMaxHungry; else m_Stat.fHungry += _iHangry; }

	void Set_Tent() { m_bTent = !m_bTent; }

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	void			Set_Attack(int _Atk);			//공격당했을 때 호출되는 함수
		
	_bool			IsPlayer_Dead() { return m_Stat.bDead; }	//외부에서 죽었는지 확인하는 함수

	void			Set_Eat() {
		m_eCurState = EAT;
		m_fFrame = 0.f;
		Eat_Sound();
		m_vPlayerActing = true;
	}
	void		Set_Weapon_Equip(WEAPON _eWeaponType) { m_eCurWeapon = _eWeaponType; }	//무기 넣어줄때 호출
	void Set_Dialog() { m_eCurState = DIALOG; m_vPlayerActing = true; }

private:
	HRESULT			Add_Component();
	void			Key_Input(const _float& fTimeDelta);
	void			Ket_Input_Road(const _float& fTimeDelta);
	HRESULT			SetUp_Material();
	void			Check_State();				//상태 변경용 함수

	void			Set_Stat();					
	void			Weapon_Change();			//무기 변경에 따른 변화에 대한 함수
	void			Look_Change();				//바라보는 방향을 바꾸기 위한 함수
	void			ResObj_Mining(RESOBJID _ObjID , CGameObject* _Obj);	//Res오브젝트들을 채굴할 때 사용하는 함수
	_int			Die_Check();
	void			Rebirth();
	void			First_Start();
	

	HRESULT Ready_Light();
	void Fire_Light();
	
	void Update_State(const _float& fTimeDelta);

	CGameObject* Find_NeerObject(float _fRange, eOBJECT_GROUPTYPE _findTarget);
	void BillBoard();


	void Hit_Sound();
	void Eat_Sound();
	void Dialog_Sound();
	void Rock_Sound();
	void Tree_Sound();
	void Grass_Sound();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][PLAYERSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

	CGameObject* m_Ghost;

	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;

	PLAYERSTATE			m_eCurState;
	PLAYERSTATE			m_ePreState;

	WEAPON				m_eCurWeapon;			//현재 착용하고 있는 무기
	WEAPON				m_ePreWeapon;			// 추후에 무기 스위칭할때 메소드를 위해 생성해둠

	LOOKDIR				m_eCurLook;
	LOOKDIR				m_ePreLook;
	_bool				m_Dirchange;			//Look
	_bool				m_bAttack;
	_bool				m_bossAttack;
	_bool				m_bHit;

	_bool				m_KeyLock;				//Key Input 함수를 막는 용도	true 면 막힘
	_bool				m_bFrameLock;			//프레임 축적을 막는 용도 true 면 막힘

	_vec3				m_vTargetPos;			//오브젝트 대상의 위치벡터
	_vec3				m_vTargetDir;			//이동하는 방향
	_bool				m_vPlayerActing;		//오브젝트와 상호작용 할 때 true

	RESOBJID			m_TargetObject;			//Res오브젝트의 종류를 판별
	_bool				m_bIsRoadScene;			//Road 씬이면 true (임시)


	_uint m_iLightNum;

	_bool		m_bTent;
	_float		m_fFrameSpeed;

	_bool		m_bFirstStart;
	_bool		m_bWakeupTrig;

	_int m_iLightRange;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9	pGraphicDev, wstring _strName);
	static CPlayer* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};

