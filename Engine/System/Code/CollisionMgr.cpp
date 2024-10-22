#include "CollisionMgr.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
	Free();
}

_bool CCollisionMgr::Collision_Monster(
	_vec3 vPlayerPos,
	_vec3* vPlayerAxis,
	_vec3 vMonsterPos,
	_vec3* vMonsterAxis,
	_vec3 vPlayerScale,
	_vec3 vMonsterScale)
{
	_float c[3][3];
	_float absC[3][3];
	_float d[3];
	_float r0, r1, r;
	int i;
	const _float cutoff = 0.9999f;
	bool existsParallelPair = false;
	_vec3 diff = vPlayerPos - vMonsterPos;

	for (i = 0; i < 3; ++i)
	{
		c[0][i] = D3DXVec3Dot(&vPlayerAxis[0], &vMonsterAxis[i]);
		absC[0][i] = fabs(c[0][i]);
		if (absC[0][i] > cutoff)
			existsParallelPair = true;
	}

	d[0] = D3DXVec3Dot(&diff, &vPlayerAxis[0]);
	r = fabs(d[0]);
	r0 = vPlayerScale.x;
	r1 = vMonsterScale.x * absC[0][0] + vMonsterScale.y * absC[0][1] + vMonsterScale.z * absC[0][2];
	if (r > r0 + r1)
		return false;

	for (i = 0; i < 3; ++i)
	{
		c[1][i] = D3DXVec3Dot(&vPlayerAxis[1], &vMonsterAxis[i]);
		absC[1][i] = fabs(c[1][i]);
		if (absC[1][i] > cutoff)
			existsParallelPair = true;
	}

	d[1] = D3DXVec3Dot(&diff, &vPlayerAxis[1]);
	r = fabs(d[1]);
	r0 = vPlayerScale.x;
	r1 = vMonsterScale.x * absC[1][0] + vMonsterScale.y * absC[1][1] + vMonsterScale.z * absC[1][2];
	if (r > r0 + r1)
		return false;

	for (i = 0; i < 3; ++i)
	{
		c[2][i] = D3DXVec3Dot(&vPlayerAxis[2], &vMonsterAxis[i]);
		absC[2][i] = fabs(c[2][i]);
		if (absC[2][i] > cutoff)
			existsParallelPair = true;
	}

	d[2] = D3DXVec3Dot(&diff, &vPlayerAxis[2]);
	r = fabs(d[2]);
	r0 = vPlayerScale.x;
	r1 = vMonsterScale.x * absC[2][0] + vMonsterScale.y * absC[2][1] + vMonsterScale.z * absC[2][2];
	if (r > r0 + r1)
		return false;

	r = fabs(D3DXVec3Dot(&diff, &vMonsterAxis[0]));
	r0 = vPlayerScale.x * absC[0][0] + vPlayerScale.y * absC[1][0] + vPlayerScale.z * absC[2][0];
	r1 = vMonsterScale.x;
	if (r > r0 + r1)
		return false;

	r = fabs(D3DXVec3Dot(&diff, &vMonsterAxis[1]));
	r0 = vPlayerScale.x * absC[0][1] + vPlayerScale.y * absC[1][1] + vPlayerScale.z * absC[2][1];
	r1 = vMonsterScale.y;
	if (r > r0 + r1)
		return false;

	r = fabs(D3DXVec3Dot(&diff, &vMonsterAxis[2]));
	r0 = vPlayerScale.x * absC[0][2] + vPlayerScale.y * absC[1][2] + vPlayerScale.z * absC[2][2];
	r1 = vMonsterScale.z;
	if (r > r0 + r1)
		return false;

	if (existsParallelPair)
		return true;

	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = vPlayerScale.y * absC[2][0] + vPlayerScale.z * absC[1][0];
	r1 = vMonsterScale.y * absC[0][2] + vMonsterScale.z * absC[0][1];
	if (r > r0 + r1)
		return false;

	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = vPlayerScale.y * absC[2][1] + vPlayerScale.z * absC[1][1];
	r1 = vMonsterScale.x * absC[0][2] + vMonsterScale.z * absC[0][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = vPlayerScale.y * absC[2][2] + vPlayerScale.z * absC[1][2];
	r1 = vMonsterScale.x * absC[0][1] + vMonsterScale.y * absC[0][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = vPlayerScale.x * absC[2][0] + vPlayerScale.z * absC[0][0];
	r1 = vMonsterScale.y * absC[1][2] + vMonsterScale.z * absC[1][1];
	if (r > r0 + r1)
		return false;

	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = vPlayerScale.x * absC[2][1] + vPlayerScale.z * absC[0][1];
	r1 = vMonsterScale.x * absC[1][2] + vMonsterScale.z * absC[1][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = vPlayerScale.x * absC[2][2] + vPlayerScale.z * absC[0][2];
	r1 = vMonsterScale.x * absC[1][1] + vMonsterScale.y * absC[1][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = vPlayerScale.x * absC[1][0] + vPlayerScale.y * absC[0][0];
	r1 = vMonsterScale.y * absC[2][2] + vMonsterScale.z * absC[2][1];
	if (r > r0 + r1)
		return false;

	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = vPlayerScale.x * absC[1][1] + vPlayerScale.y * absC[0][1];
	r1 = vMonsterScale.x * absC[2][2] + vMonsterScale.z * absC[2][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = vPlayerScale.x * absC[1][2] + vPlayerScale.y * absC[0][2];
	r1 = vMonsterScale.x * absC[2][1] + vMonsterScale.y * absC[2][0];
	if (r > r0 + r1)
		return false;

	return true;
}

//안씀
_bool CCollisionMgr::Collision_Item(_vec3 pPlayerPos, _vec3 pItemPos, _vec3 vPlayerScale, _vec3 vItemScale)
{
	_float iDistanceX = fabs(pPlayerPos.x - pItemPos.x);
	_float fRadCX = vPlayerScale.x * 0.5f + vItemScale.x * 0.5f;

	_float fDistanceZ = fabs(pPlayerPos.z - pItemPos.z);
	_float fRadCY = vPlayerScale.y * 0.5f + vItemScale.y * 0.5f;

	if (fDistanceZ > fRadCY || iDistanceX > fRadCX)
		return false;

	return true;
}

_bool Engine::CCollisionMgr::Collision_Mouse_Object(_vec3 vRayPos, _vec3 vRayDir, _vec3 vObjPos, _vec3 vObjScale)
{
	//컬라이더가 붙을 월드 위치
	float minValue = 0.0f, maxValue = FLT_MAX;

	_vec3 minPosition = _vec3{ vObjPos.x - (vObjScale.x * 0.5f), vObjPos.y - (vObjScale.y * 0.5f), vObjPos.z - (vObjScale.z * 0.5f) };
	_vec3 maxPosition = _vec3{ vObjPos.x + (vObjScale.x * 0.5f), vObjPos.y + (vObjScale.y * 0.5f), vObjPos.z + (vObjScale.z * 0.5f) };
	//X축 검사
	if (fabsf(vRayDir.x) >= 0.f)
	{
		float value = 1.0f / vRayDir.x;
		float minX = (minPosition.x - vRayPos.x) * value;
		float maxX = (maxPosition.x - vRayPos.x) * value;

		if (minX > maxX)
		{
			float temp = minX;
			minX = maxX;
			maxX = temp;
		}

		minValue = max(minX, minValue);
		maxValue = min(maxX, maxValue);

		if (minValue > maxValue)
			return false;
	}

	//Y축 검사
	if (fabsf(vRayDir.y) >= 0.f)
	{
		float value = 1.0f / vRayDir.y;
		float minY = (minPosition.y - vRayPos.y) * value;
		float maxY = (maxPosition.y - vRayPos.y) * value;

		if (minY > maxY)
		{
			float temp = minY;
			minY = maxY;
			maxY = temp;
		}

		minValue = max(minY, minValue);
		maxValue = min(maxY, maxValue);

		if (minValue > maxValue)
			return false;
	}

	//Z축 검사
	if (fabsf(vRayDir.z) >= 0.f)
	{
		float value = 1.0f / vRayDir.z;
		float minZ = (minPosition.z - vRayPos.z) * value;
		float maxZ = (maxPosition.z - vRayPos.z) * value;

		if (minZ > maxZ)
		{
			float temp = minZ;
			minZ = maxZ;
			maxZ = temp;
		}

		minValue = max(minZ, minValue);
		maxValue = min(maxZ, maxValue);

		if (minValue > maxValue)
			return false;
	}

	return true;
}

_bool Engine::CCollisionMgr::Collision_Mouse(_vec2 vMousePos, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	if (fX - (fSizeX) < vMousePos.x && vMousePos.x < fX + (fSizeX))
		if (fY - (fSizeY) < vMousePos.y && vMousePos.y < fY + (fSizeY))
			return true;

	return false;
}




void CCollisionMgr::Free()
{
}
