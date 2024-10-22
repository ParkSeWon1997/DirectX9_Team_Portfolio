#pragma once
#include "GameObject.h"
#include "Engine_Define.h"
#include "ResObject.h"

class CObjectRock :
	public CResObject
{
private:
	explicit CObjectRock(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CObjectRock(const CObjectRock& rhs);
	virtual ~CObjectRock();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT	Add_Component() override;
	void Change_Frame_Event() override;
	void Check_FrameState() override;
	void Ready_Stat() override;

public:
	static CResObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();


	_vec3 m_vOriginPos;

};

