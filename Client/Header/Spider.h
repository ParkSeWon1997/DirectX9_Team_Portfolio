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
class CSpider : public CMonster
{
	enum SPIDERSTATE { IDLE, WALK,ATTACK, SLEEP,HIT, DEAD, ERASE,STATE_END };

private:
	explicit CSpider(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CSpider(const CSpider& rhs);
	virtual ~CSpider();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();
	virtual void			State_Change();
	virtual void			Set_ObjStat()					override;
	
	

	
	void			Attacking(const _float& fTimeDelta);
	void			Patroll(const _float& fTimeDelta);
	_int			Die_Check();
	virtual void	Set_Hit()						override;
	void			Detect_Player();

	void			Attack_Sound();
	void			Dead_Sound();
	void			Scream_Sound();



private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][SPIDERSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;


	SPIDERSTATE m_eCurstate;
	SPIDERSTATE m_ePrestate;
	_bool		m_bModeChange;
	_bool		m_bDetect;
	
public:
	static CSpider* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	virtual void Free() override;
};
