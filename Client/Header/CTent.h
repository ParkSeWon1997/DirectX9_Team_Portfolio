#pragma once
#include "ResObject.h"
class CTent :
    public CResObject
{



	enum CTENT_STATE { 
		CTENT_IDLE, 
		CTENT_HIT,
		CTENT_ENTER,
		CTENT_DESTROY,
		CTENT_BURNT,
		CTENT_PLACE,
		CTENT_DEFAULT,
		CTENT_END
	};

private:
	explicit CTent(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall);
	explicit CTent(const CTent& rhs);
	virtual ~CTent();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;





	void Set_Enter() { m_bIsEnter = true; };
	void Set_Destroy() { m_bIsDestroy = true; };
	void Set_Hit() { m_bIsHit = true; };
	void Set_Burnt() { m_bIsBurnt = true; };



private:
	virtual HRESULT			Add_Component() override;
	virtual void			Check_FrameState() override;
	virtual void			Ready_Stat() override {};
	virtual void			Change_Frame_Event() override ;

	void Install_Obj();



public:
	static CTent* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall = false);

private:
	virtual void Free();


private:
	Engine::CCalculator* m_pCalculatorCom;
	CTexture* m_pTentTexCom[CTENT_END];

	CTENT_STATE m_eTentCurState;
	CTENT_STATE m_eTentPrevState;

	BOOL m_bIsDrop = false;
	BOOL m_bIsFrameStop = false;
	BOOL m_bIsEnter = false;
	BOOL m_bIsDestroy = false;

	_bool m_bInstall = false;



	BOOL m_bIsHit = false;
	BOOL m_bIsBurnt = false;

};

