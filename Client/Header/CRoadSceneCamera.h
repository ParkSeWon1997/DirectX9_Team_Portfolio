#pragma once

#include "Camera.h"

class CRoadSceneCamera : public Engine::CCamera
{
private:
	explicit CRoadSceneCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRoadSceneCamera();

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;

	_bool TurnAngle(const _float& fTimeDelta, _float TargetAngle);
public:
	void		SetTarget(CGameObject* _targetObj);
	_float		Get_Angle() { return m_fAngle; }
	_vec3		Get_Pos() { return m_vEye; }
	void ShakeCamera();
	void SetShakedCamera(_float _fIntensity, _float _fShakeTime, _bool _bLockWidth)
	{
		m_fShakeAccTime = 0.f;
		m_fIntensity = _fIntensity;
		m_fShakeTime = _fShakeTime;
		m_bLockWidth = _bLockWidth;
	}

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Fix();
	void		Mouse_Move();
	void		CalDiff(const _float& fTimeDelta);

public:
	static CRoadSceneCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

private:
	virtual void Free();
private:
	CGameObject* m_pTarget = nullptr;

	_float	m_fAngle;		//각도
	_float	m_fDistance;	//거리
	_float	m_fHeight;		//높이

	_bool	m_bkeyInput;

	_float	m_fIntensity;	//카메라 흔들림 강도
	_float	m_fShakeTime;	//진동 시간
	_float	m_fShakeAccTime;		//누적 시간
	_bool	m_bLockWidth;		//좌우 흔들림 적용

	_float	m_fDiff;		//차이값
	_vec3	m_vTargetEye;	//카메라 목적지
	_vec3	m_vPrevEye;		//카메라 이전 위치
	_bool	m_bMove;
	_float m_fCameraSpeed;

	_float m_fAccTime;

	_bool	m_bWake;

	eROAD_CAMERA_STATE	m_eCurState;


};

