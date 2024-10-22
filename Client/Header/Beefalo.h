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
class CBeefalo : public CMonster
{
	enum BEEFALOSTATE { IDLE, GRAZE, WALK,ATTACK,RUN, MADRUN, SLEEP,HIT, DEAD,ERASE, STATE_END };
private:
	explicit CBeefalo(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CBeefalo(const CBeefalo& rhs);
	virtual ~CBeefalo();
public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	void		Set_Attack_State();
private:
	HRESULT			Add_Component();
	virtual void	Set_ObjStat()					override;
	virtual void	State_Change()					override;
	_int			 Die_Check();
	void			Attacking(const _float& fTimeDelta);
	void			Patroll(const _float& fTimeDelta);
	virtual void	Set_Hit()						override;

	
	void FrameCheckSound(const _float& fTimeDelta);
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][BEEFALOSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

	BEEFALOSTATE		m_eCurState;
	BEEFALOSTATE		m_ePreState;
	_bool				m_bHit;
	
	_bool				m_bStepSound[2];



	//float PerFrame = 0.0f;
public:
	static CBeefalo* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	virtual void Free() override;
};

