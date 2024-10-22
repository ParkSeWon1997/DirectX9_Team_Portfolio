#pragma once
#include "ResObject.h"
class CTeleporterWorm :
    public CResObject
{

	enum TELEPORTER_STATE { TELEPORTER_IDLE, TELEPORTER_OPEN, TELEPORTER_CLOSE,TELEPORTER_END};

private:
	explicit CTeleporterWorm(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTeleporterWorm(const CTeleporterWorm& rhs);
	virtual ~CTeleporterWorm();

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

	void ChangeScenePlayer(_float _fDistance);

	
	
	//decltype(auto) 	IsPlayerInRadius(int );
public:	//외부용
	bool IsPlayerInRadius(_vec3& _PlayerPos, _vec3& _MyPos);

private:
	//내부용
	tuple<_bool, _vec3,_bool> IsPlayerInRadius();
public:
	static CResObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();



private:
	Engine::CTexture* m_pTeleporterTextureCom[TELEPORTER_END];

	TELEPORTER_STATE m_eTelporterCurState;
	TELEPORTER_STATE m_eTelporterPreState;

	BOOL m_bFrameStop = false;


	BOOL m_bReverseFrame = false;


	_vec3 m_vOriginPos;
};

