#pragma once
#include "ResObject.h"
class CCookingPot :
	public CResObject
{



	enum COOKINGPOT_STATE {
		COOKINGPOT_IDLE_EMPTY,
		COOKINGPOT_IDLE_FULL,
		COOKINGPOT_COOKING_LOOP,
		COOKINGPOT_BURNT,
		COOKINGPOT_PLACE,
		COOKINGPOT_HIT_EMPTY,
		COOKINGPOT_HIT_COOKING,
		COOKINGPOT_DEFAULT,
		COOKINGPOT_END
	};

	
private:
	explicit CCookingPot(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall);
	explicit CCookingPot(const CCookingPot& rhs);
	virtual ~CCookingPot();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;




	//요리를 시작하거나  강제로 멈추는 함수 (일정 시간이 지나면 자동으로 요리를 멈춤)
	void Set_Cooking(_In_ BOOL _IsCooking) { m_bIsCooking = _IsCooking; };
	
	//솥을 불태워버리는 함수 //솥 복구 불가
	void Set_Burnt() { m_bIsBurnt = true; };

	//솥을 가득 채우는 함수
	void Set_Full()
	{
	
		m_bIsFull = true;
		m_bIsEmpty = false;
	};

	//솥을 비우는 함수
	void Set_Empty()
	{ 
		m_bIsEmpty = true; 
		m_bIsFull = false;
	};

	//요리중일 때 솥이 맞는 함수 (솥이 요리 중이어야 함)
	void Set_Hit() 
	{
		m_bIsHit = true;
	};

	void Set_Success(_bool bSuccess) { m_bSuccess = bSuccess; }

private:
	virtual HRESULT			Add_Component() override;
	virtual void			Check_FrameState() override;
	virtual void			Ready_Stat() override {};
	virtual void			Change_Frame_Event() override;

	void Install_Obj();



public:
	static CCookingPot* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall = false);

private:
	virtual void Free();


private:
	Engine::CCalculator* m_pCalculatorCom;
	CTexture* m_pCookingpotTexCom[COOKINGPOT_END];

	COOKINGPOT_STATE m_eCookingpotCurState;
	COOKINGPOT_STATE m_eCookingpotPrevState;

	_float m_fTimeChek = 0.f;
	_float m_MaxfTimeChek = 5.f;

	BOOL m_bIsDrop = false;
	BOOL m_bIsFrameStop = false;
	BOOL m_bIsCooking = false;
	BOOL m_bIsBurnt = false;
	BOOL m_bIsFull = false;
	BOOL m_bIsEmpty = false;
	BOOL m_bIsHit = false;

	_bool m_bInstall = false;

	_bool m_bSuccess = false;

};

