#pragma once
#include "ResObject.h"
class CObjectFireFlies :
    public CResObject
{
	enum FIREFLIESSTATE { FIREFLIES_PRE, FIREFLIES_LOOP,FIREFLIES_PST, FIREFLIES_END };

private:
	explicit CObjectFireFlies(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CObjectFireFlies(const CObjectFireFlies& rhs);
	virtual ~CObjectFireFlies();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;

private:
	virtual HRESULT			Add_Component() override;
	virtual void Check_FrameState() override;
	virtual void Ready_Stat() override {};
	virtual void Change_Frame_Event() override;



	BOOL Check_Night();
public:
	static CResObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

private:
	Engine::CTexture* m_pFireFliesTextureCom[FIREFLIES_END];

	FIREFLIESSTATE m_eFireFliesCurState;
	FIREFLIESSTATE m_eFireFliesPreState;




	BOOL m_bIsFrameStop = false;
};

