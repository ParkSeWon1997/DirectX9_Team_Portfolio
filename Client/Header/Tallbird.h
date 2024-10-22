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
class CTallbird : public CMonster
{
	
	enum TALLBIRD { IDLE, WALK, ATTACK, HIT, SLEEP , WAKE_UP,TAUNT, DEAD,ERASE , STATE_END};

private:
	explicit CTallbird(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CTallbird(const CTallbird& rhs);
	virtual ~CTallbird();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	void		Set_Tallbird_WakeUp() {
		if(!m_bWakeUp)
			m_bWakeUp = true; }
private:
	HRESULT					Add_Component();
	virtual void			State_Change();
	virtual void			Set_ObjStat()					override;
	void					Set_Scale();
	void					First_Phase(const _float& fTimeDelta);
	void					Second_Phase(const _float& fTimeDelta);

	_int			Die_Check();
	virtual void	Set_Hit()						override;
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][TALLBIRD::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

	TALLBIRD m_eCurstate;
	TALLBIRD m_ePrestate;
	_bool   m_bStatChange[2];
	_bool	m_bWakeUp;
public:
	static CTallbird* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	virtual void Free() override;
};
