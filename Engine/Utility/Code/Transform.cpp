#include "..\..\Header\Transform.h"


CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_vAngle(0.f, 0.f, 0.f), m_vScale(1.f, 1.f, 1.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs), m_vAngle(rhs.m_vAngle), m_vScale(rhs.m_vScale)
{
	for (size_t i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];
	//
	m_matWorld = rhs.m_matWorld;
}


CTransform::~CTransform()
{
}

HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_int i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

_int CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_int i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 크기 변환
	for (_int i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	// 회전 변환
	_matrix			matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_int i = 0; i < INFO_POS; ++i)
	{
		for (_int j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_int i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return 0;
}

void CTransform::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix		matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matTrans;

}

LOOKDIR CTransform::Chase_Target_Monster(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	LOOKDIR		eDir;
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];
	vDir.y = 0.f;
	_vec3		right, up, look;

	Get_Info(INFO_RIGHT, &right);
	Get_Info(INFO_UP, &up);
	Get_Info(INFO_LOOK, &look);

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Normalize(&look, &look);

	float lookDot = D3DXVec3Dot(&vDir, &look);
	float rightDot = D3DXVec3Dot(&vDir, &right);

	/*_float RAngle = CalculateAngleBetweenVectors(vDir, right);
	_float LAngle = CalculateAngleBetweenVectors(vDir, look);*/

	if (fabs(lookDot) > fabs(rightDot)) {
		if (lookDot > 0.0f) {
			eDir = LOOK_UP;
		}
		else {
			eDir = LOOK_DOWN;
		}
	}
	else {
		if (rightDot > 0.0f) {
			eDir = LOOK_RIGHT;
		}
		else {
			eDir = LOOK_LEFT;
		}
	}
	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix		matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matTrans;

	return eDir;
}

LOOKDIR Engine::CTransform::Patroll_LookChange(const _vec3* _vDir, const _float& fSpeed, const _float& fTimeDelta)
{
	LOOKDIR		eDir;
	_vec3		vDir = *_vDir;
	vDir.y = 0.f;

	_vec3		right, up, look;
	Get_Info(INFO_RIGHT, &right);
	Get_Info(INFO_UP, &up);
	Get_Info(INFO_LOOK, &look);

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Normalize(&look, &look);

	float lookDot = D3DXVec3Dot(&vDir, &look);
	float rightDot = D3DXVec3Dot(&vDir, &right);

	if (fabs(lookDot) > fabs(rightDot)) {
		if (lookDot > 0.0f) {
			eDir = LOOK_UP;
		}
		else {
			eDir = LOOK_DOWN;
		}
	}
	else {
		if (rightDot > 0.0f) {
			eDir = LOOK_RIGHT;
		}
		else {
			eDir = LOOK_LEFT;
		}
	}

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix		matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matTrans;

	return eDir;
}

LOOKDIR Engine::CTransform::For_Player_Direction(const _vec3* _vDir, const _float& fSpeed, const _float& fTimeDelta)
{
	LOOKDIR		eDir;
	_vec3		right, look, vDir;
	vDir = *_vDir;
	Get_Info(INFO_RIGHT, &right);
	Get_Info(INFO_LOOK, &look);

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Normalize(&look, &look);


	float lookDot = D3DXVec3Dot(&vDir, &look);
	float rightDot = D3DXVec3Dot(&vDir, &right);

	if (fabs(lookDot) > fabs(rightDot)) {
		if (lookDot > 0.0f) {
			eDir = LOOK_UP;
		}
		else {
			eDir = LOOK_DOWN;
		}
	}
	else {
		if (rightDot > 0.0f) {
			eDir = LOOK_RIGHT;
		}
		else {
			eDir = LOOK_LEFT;
		}
	}

	m_vInfo[INFO_POS] += vDir * fSpeed * fTimeDelta;

	return eDir;
}

const _matrix* Engine::CTransform::Compute_LookAtTarget(const _vec3* pTargetPos)
{
	_vec3	vDir = *pTargetPos - m_vInfo[INFO_POS];



	_vec3	vAxis, vUp;
	_matrix matRot;

	return D3DXMatrixRotationAxis(&matRot,
		D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir),
		acos(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
			D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}

void Engine::CTransform::BillBoard()
{
	_matrix	matWorld, matView, matBill;
	Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	Set_WorldMatrix(&(matBill * matWorld));

	/*_matrix matView;

	_vec3 vPos;
	vPos = Get_Pos();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	matView._41 = vPos.x;
	matView._42 = vPos.y;
	matView._43 = vPos.z;
	Set_WorldMatrix(&(matView));*/
}

float Engine::CTransform::CalculateAngleBetweenVectors(const D3DXVECTOR3& vec1, const D3DXVECTOR3& vec2)
{
	float dotProduct = D3DXVec3Dot(&vec1, &vec2);
	float magnitudeVec1 = D3DXVec3Length(&vec1);
	float magnitudeVec2 = D3DXVec3Length(&vec2);

	// 각도 계산 (라디안)
	float angleRad = acos(dotProduct / (magnitudeVec1 * magnitudeVec2));

	// 라디안을 0에서 360도로 변환
	float angleDeg = angleRad * (180.0f / D3DX_PI);
	if (vec1.y * vec2.x - vec1.x * vec2.y < 0) {
		angleDeg = 360.0f - angleDeg;
	}

	return angleDeg;
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Transform()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	__super::Free();
}
