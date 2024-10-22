#pragma once
#include "Component.h"

namespace Engine {

	class ENGINE_DLL CTransform : public CComponent
	{
	private:
		explicit CTransform() = delete;
		explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
		explicit CTransform(const CTransform& rhs);
		virtual ~CTransform();

	public:
		HRESULT			Ready_Transform();
		virtual _int	Update_Component(const _float& fTimeDelta);
		virtual void	LateUpdate_Component() {}

	public:
		_matrix* Get_WorldMatrix() { return &m_matWorld; }
		void		Get_WorldMatrix(_matrix* pWorld) { *pWorld = m_matWorld; }
		void				Set_Pos(const _vec3& vPos)
		{
			m_vInfo[INFO_POS] = vPos;
		}

		void	Set_WorldMatrix(const _matrix* pWorld) { m_matWorld = *pWorld; }

		void				Set_Pos(_float fX, _float fY, _float fZ)
		{
			m_vInfo[INFO_POS].x = fX;
			m_vInfo[INFO_POS].y = fY;
			m_vInfo[INFO_POS].z = fZ;
		}

		_vec3 Get_Scale() { return m_vScale; }
		_vec3 Get_Pos() { return m_vInfo[INFO_POS]; }

		void		Rotation(ROTATION eType, const _float& fAngle)
		{
			*(((_float*)&m_vAngle) + eType) += fAngle;
		}

		void		Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
		{
			m_vInfo[INFO_POS] += *pDir * fSpeed * fTimeDelta;
		}

		void		Move_Terrain(const _vec3* pPos, const _float& fSpeed, const _float& fTimeDelta)
		{
			_vec3	vDir = *pPos - m_vInfo[INFO_POS];
			m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
		}

		void		Get_Info(INFO eType, _vec3* pInfo)
		{
			memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
		}

		void		Set_Info(INFO eType, _vec3* pInfo)
		{
			m_vInfo[eType] = *pInfo;
		}
		void		Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
		LOOKDIR		Chase_Target_Monster(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
		LOOKDIR		Patroll_LookChange(const _vec3* _vDir ,const _float& fSpeed, const _float& fTimeDelta);
		LOOKDIR		For_Player_Direction(const _vec3* _vTarget, const _float& fSpeed, const _float& fTimeDelta);
		const _matrix* Compute_LookAtTarget(const _vec3* pTargetPos);
		void Set_Scale(_vec3 _scale) { m_vScale = _scale; }

		void			BillBoard();


	public:
		_vec3		m_vInfo[INFO_END];
		_vec3		m_vScale;
		_vec3		m_vAngle;
		_matrix		m_matWorld;


	private:
		float CalculateAngleBetweenVectors(const D3DXVECTOR3& vec1, const D3DXVECTOR3& vec2);
	public:
		static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);
		virtual CComponent* Clone();

	private:
		virtual void		Free();
	};

}

