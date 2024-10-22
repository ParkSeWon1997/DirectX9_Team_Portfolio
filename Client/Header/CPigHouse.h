#pragma once
#include "GameObject.h"
#include "ResObject.h"

class CPigHouse :
	public CResObject
{

	enum PIGHOUSE { STANDARD, ONLIGHT, RUNDOWN, PH_END };
private:
	explicit CPigHouse(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPigHouse(const CPigHouse& rhs);
	virtual ~CPigHouse();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;
	void Set_Pig_Angry();

private:
	HRESULT	Add_Component() override;
	void Change_Frame_Event() override;
	void Check_FrameState() override;
	void Ready_Stat() override;

	HRESULT Ready_Light();	//Α¶Έν
	void Change_Light();

private:
	PIGHOUSE  m_eHouseState;
	_int m_iPointNum;

public:
	static CResObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();


	BOOL m_bIsDropItem = false;
};