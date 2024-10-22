#include "stdafx.h"
#include "..\Header\DynamicCamera.h"

#include "Export_System.h"
#include "Export_Utility.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_fAngle(180.f)
	, m_fDistance(7.f)
	, m_fHeight(5.4f)
	, m_fRoadDistance(5.f)
	, m_fRoadHeight(3.f)
	, m_bkeyInput(true)
	, m_fIntensity(3.f)
	, m_fShakeTime(0.3f)
	, m_fShakeAccTime(1.f)
	, m_bLockWidth(false)
	, m_fCameraSpeed(20.f)
{

}

CDynamicCamera::~CDynamicCamera()
{

}

HRESULT CDynamicCamera::Ready_GameObject(const _vec3* pEye,
	const _vec3* pAt, const _vec3* pUp,
	const _float& fFov, const _float& fAspect,
	const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

	return S_OK;
}

Engine::_int CDynamicCamera::Update_GameObject(const _float& fTimeDelta)
{

	Key_Input(fTimeDelta);

	Mouse_Move();

	if (KEY_TAP(DIK_U))
	{
		m_fHeight += 0.1f;
	}
	if (KEY_TAP(DIK_I))
	{
		m_fHeight -= 0.1f;
	}

	if (m_pTarget != nullptr && !m_pTarget->IsDelete())
	{
		_vec3 vTarget;
		m_pTarget->GetTransForm()->Get_Info(INFO::INFO_POS, &vTarget);
		m_vAt = vTarget;//바라볼곳
		m_vTargetEye.x = vTarget.x + cosf(m_fAngle) * m_fDistance;
		m_vTargetEye.y = vTarget.y + m_fHeight;
		m_vTargetEye.z = vTarget.z + sinf(m_fAngle) * m_fDistance;//사실상 목적지
	}
	if (m_fShakeTime > m_fShakeAccTime)
	{
		m_fShakeAccTime += fTimeDelta;
		ShakeCamera();
	}

	if (m_bMove)
	{
		CalDiff(fTimeDelta);
	}
	else
	{
		m_vEye = m_vTargetEye;
	}

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);



	return 0;
}

void CDynamicCamera::LateUpdate_GameObject()
{
	Engine::CCamera::LateUpdate_GameObject();
}

void CDynamicCamera::ShakeCamera()
{
	_vec3 vLook;
	_vec3 vRight;
	_vec3 vUp;
	D3DXVec3Normalize(&vLook, &(m_vAt - m_vEye));
	D3DXVec3Cross(&vRight, &vLook, &m_vUp);
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	// -1.5f ~ 1.5f 사이의값 구함
	float offsetY = ((rand() % 100 / 100.0f) * m_fIntensity) - (m_fIntensity * 0.5f);

	m_vEye += vUp * offsetY;
	m_vAt += vUp * offsetY;

	if (!m_bLockWidth)
	{
		float offsetX = ((rand() % 100 / 100.0f) * m_fIntensity) - (m_fIntensity * 0.5f);
		m_vEye += vRight * offsetX;
		m_vAt += vRight * offsetX;
	}
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix			matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (KEY_TAP(DIK_Q))
	{
		m_fAngle += 45.f;
		m_fCameraSpeed = 40.f;
		m_bMove = true;
	}
	if (KEY_TAP(DIK_E))
	{
		m_fAngle -= 45.f;
		m_fCameraSpeed = 40.f;
		m_bMove = true;
	}
}

void CDynamicCamera::Mouse_Fix()
{
	POINT	pt{ WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CDynamicCamera::Mouse_Move()
{
	_long	dwMouseMove(0);
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Z))
	{
		if (dwMouseMove > 0.f)
		{
			if (m_fDistance >= 3.f)
			{
				m_fDistance -= 2.f;
				m_fCameraSpeed = 10.f;
				m_bMove = true;

			}

		}
		else if (dwMouseMove < 0.f)
		{
			if (m_fDistance <= 9.f)
			{
				m_fDistance += 2.f;
				m_fCameraSpeed = 10.f;
				m_bMove = true;
			}


		}
	}

}

void CDynamicCamera::CalDiff(const _float& fTimeDelta)
{

	//카메라이동할 거리와 방향

	_vec3	vDir;
	_float	fDistance;

	D3DXVec3Normalize(&vDir, &(m_vTargetEye - m_vEye));
	fDistance = D3DXVec3Length(&(m_vEye - m_vTargetEye));
	if (fDistance <= 1.f)
	{
		m_bMove = false;
		m_fCameraSpeed = 20.f;
		return;
	}
	m_vEye += m_fCameraSpeed * vDir * fTimeDelta;

}

void CDynamicCamera::SetTarget(CGameObject* _targetObj)
{
	m_pTarget = _targetObj;
	_vec3 vTarget;
	m_pTarget->GetTransForm()->Get_Info(INFO::INFO_POS, &vTarget);
	m_vAt = vTarget;
}


void CDynamicCamera::Free()
{
	__super::Free();
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera* pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("dynamiccamera Create Failed");
		return nullptr;
	}

	return pInstance;
}
