#include "stdafx.h"
#include "CRoadSceneCamera.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "DeerClops.h"


CRoadSceneCamera::CRoadSceneCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_fAngle(180.f)
	, m_fDistance(6.f)
	, m_fHeight(4.f)
	, m_bkeyInput(true)
	, m_fIntensity(3.f)
	, m_fShakeTime(0.3f)
	, m_fShakeAccTime(1.f)
	, m_bLockWidth(false)
	, m_fCameraSpeed(20.f)
	, m_eCurState(eROAD_CAMERA_STATE::NONE)
	, m_fAccTime(0.f)
	, m_bWake(true)
{

}

CRoadSceneCamera::~CRoadSceneCamera()
{

}

HRESULT CRoadSceneCamera::Ready_GameObject(const _vec3* pEye,
	const _vec3* pAt, const _vec3* pUp,
	const _float& fFov, const _float& fAspect,
	const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vTargetEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);
	m_eCurState = eROAD_CAMERA_STATE::BOSS;

	return S_OK;
}

// 회전에 필요한 변수 설정
float totalTime = 2.f; // 회전하는 총 시간
float currentAngle = 0.0f; // 현재 회전된 각도
float diff = 5.f;

Engine::_int CRoadSceneCamera::Update_GameObject(const _float& fTimeDelta)
{



	if (m_pTarget != nullptr && !m_pTarget->IsDelete())
	{
		_vec3 vTarget;
		m_pTarget->GetTransForm()->Get_Info(INFO::INFO_POS, &vTarget);

		m_vAt.x = vTarget.x;
		m_vAt.y = vTarget.y;
		m_vAt.z = 3.56f;

		//카메라위치
		m_vTargetEye.x = m_vAt.x + -diff;//4.2 ~4.3
		
	}


	if (m_eCurState == eROAD_CAMERA_STATE::BOSS)
	{
		if (TurnAngle(fTimeDelta, 180.f))
		{
			auto& boss = (scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::BOSS).begin());
			if (nullptr != (*boss) && !(*boss)->IsDelete())
			{
				SetTarget((*boss));
				m_eCurState = eROAD_CAMERA_STATE::BOSS_TAUNT;
				totalTime = 0.1f;
			}
		}
	}

	if (m_eCurState == eROAD_CAMERA_STATE::BOSS_TAUNT)
	{
		m_fAccTime += fTimeDelta;
		_vec3 vTarget;
		_float fDistance;
		m_pTarget->GetTransForm()->Get_Info(INFO::INFO_POS, &vTarget);
		if (m_bWake)
		{
			dynamic_cast<CDeerClops*>(m_pTarget)->Set_WakeUp();
			m_bWake = false;
		}

		fDistance = m_vTargetEye.x - vTarget.x;
		//카메라위치
		diff = -fDistance;
	}

	if (3.f <= m_fAccTime)
	{
		if (TurnAngle(fTimeDelta, 180.f))
		{
			m_fAccTime = 0.f;
			SetTarget(scenemgr::Get_CurScene()->GetPlayerObject());
			diff = 5.f;
			m_eCurState = eROAD_CAMERA_STATE::NONE;
		}
	}

	m_vEye = m_vTargetEye;

	if (m_fShakeTime > m_fShakeAccTime)
	{
		m_fShakeAccTime += fTimeDelta;
		ShakeCamera();
	}

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}
void CRoadSceneCamera::LateUpdate_GameObject()
{
	Engine::CCamera::LateUpdate_GameObject();
}

_bool CRoadSceneCamera::TurnAngle(const _float& fTimeDelta, _float TargetAngle)
{
	float rotationSpeed = TargetAngle / totalTime; // 초당 회전 속도
	// 회전할 각도 계산
	float rotateAmount = rotationSpeed * fTimeDelta;

	// 현재 회전 각도 업데이트
	currentAngle += rotateAmount;

	// 180도 회전이 완료되었는지 확인
	if (currentAngle >= TargetAngle)
	{
		// 회전 완료 시 처리
		currentAngle = TargetAngle; // 목표 각도로 설정
		// 회전 완료 후 추가 작업 수행 가능
		currentAngle = 0.f;
		return true;
	}

	_vec3		vUp = { 0.f, 1.f, 0.f };
	_vec3	vLook = m_vAt - m_vTargetEye;

	_matrix		matRot;
	D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(currentAngle));
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
	m_vAt = m_vTargetEye + vLook;
	return false;
}

void CRoadSceneCamera::ShakeCamera()
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

void CRoadSceneCamera::Key_Input(const _float& fTimeDelta)
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

void CRoadSceneCamera::Mouse_Fix()
{
	POINT	pt{ WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CRoadSceneCamera::Mouse_Move()
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

void CRoadSceneCamera::CalDiff(const _float& fTimeDelta)
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

void CRoadSceneCamera::SetTarget(CGameObject* _targetObj)
{
	m_pTarget = _targetObj;
	_vec3 vTarget;
	m_pTarget->GetTransForm()->Get_Info(INFO::INFO_POS, &vTarget);
	m_vAt = vTarget;
}


void CRoadSceneCamera::Free()
{
	__super::Free();
}

CRoadSceneCamera* CRoadSceneCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CRoadSceneCamera* pInstance = new CRoadSceneCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("dynamiccamera Create Failed");
		return nullptr;
	}

	return pInstance;
}
