#pragma once
#include "Base.h"
#include "Monster.h"

namespace Engine {

	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;

}
class CPig : public CMonster
{
	enum PiGSTATE { IDLE, EAT, WALK, HAPPY , ANGRY_IDLE, RUN, ATTACK, SLEEP, HIT,  DEAD, ERASE, STATE_END };

private:
	explicit CPig(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CPig(const CPig& rhs);
	virtual ~CPig();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

	void Set_AttackState() { m_Attacked = true; }

private:
	HRESULT			Add_Component();
	virtual void	Set_ObjStat()					override;
	void			Player_Chase(const _float& fTimeDelta);
	virtual void			State_Change()			override;
	_int			 Die_Check();
	void			Attacking(const _float& fTimeDelta);
	void			Patroll(const _float& fTimeDelta);
	virtual void	Set_Hit()						override;

	void			Dead_Sound();
	void			Oink_Sound();
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	/*Engine::CTransform* m_pTransForm;*/
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][PiGSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;


	PiGSTATE		m_eCurState;
	PiGSTATE		m_ePreState;
	_bool				m_bHit;
public:
	static CPig* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	virtual void Free() override;
};

