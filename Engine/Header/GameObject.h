#pragma once

#include "Engine_Define.h"
#include "Base.h"

namespace Engine
{
	class CComponent;
	class CTransform;
	class CCalculator;
	class CShader;

	class ENGINE_DLL CGameObject : public CBase
	{
	public:
		_float				Get_ViewZ() { return m_fViewZ; }
		CTransform* GetTransForm() { return m_pTransForm; }
		HRESULT				SetUp_Material(D3DXCOLOR vColor = D3DXCOLOR(0.f, 0.f, 0.f, 0.f));
		void				Compute_ViewZ(const _vec3* pPos);
		virtual CComponent* Find_Component(COMPONENTID eID, const _tchar* pComponentTag);

		wstring	GetObjName() { return m_strObjName; }
		void	SetObjName(wstring _objName) { m_strObjName = _objName; }

		void SetDeleteObj() { m_bDelete = true; }
		_bool IsDelete() { return m_bDelete; }

		OBJSTAT Get_State() { return m_Stat; }

		void Set_SlotNum(_int iNum) { m_iSlotNum = iNum; }

		void Set_Shader(_bool bShader) { m_bShader = bShader; }

	public:
		virtual HRESULT		Ready_GameObject();
		virtual _int		Update_GameObject(const _float& fTimeDelta);
		virtual void		LateUpdate_GameObject();
		virtual void		Render_GameObject()PURE;
		_bool		Collision_Transform(CTransform* _Src, CTransform* _Dst);	//AABB�Լ�.
		_bool		Collision_Circle(CGameObject* _pTarget);	//�����浹.
		//ù��° ���� �ڽ�, �ι�° ���� ���

		//1.��° ��ȯ ��: PlyerPos,2. ��° ��ȯ ��: �ڱ� �ڽ��� ��ġ ���� 3. ��° ��ȯ ��: �ڱ� �ڽ��� Look����, 4. ��° ��ȯ ��: �ڱ� �ڽ��� Up����
		const tuple<_vec3, _vec3, _vec3, _vec3> Get_Info_vec();

		void SetCollisionRadius(_float _fColRadius) { m_fCollisionRadius = _fColRadius; }
		_float GetCollisionRadius() { return m_fCollisionRadius; }


	protected:
		explicit CGameObject() = delete;
		explicit CGameObject(LPDIRECT3DDEVICE9	pGraphicDev);
		explicit CGameObject(LPDIRECT3DDEVICE9	pGraphicDev, wstring _strName);
		explicit CGameObject(const CGameObject& rhs);
		virtual ~CGameObject();


		virtual void		Free() override;
	protected:
		LPDIRECT3DDEVICE9							m_pGraphicDev;

		multimap<const _tchar*, CComponent*>		m_mapComponent[ID_END];
		CTransform* m_pTransForm;
		CShader* m_pShaderCom;

		_float										m_fViewZ;	//z���� ����
		_bool										m_bDelete;	//���������ϰ�� True

		wstring										m_strObjName;
		OBJSTAT				m_Stat;
		_float				m_fCollisionRadius;	// ���� �ڽ��� �����浹����

		_int m_iSlotNum;  // ���� �� ������ ���°�� �ִ��� üũ�ϱ� ���ؼ� �ʿ�

		_bool m_bShader;
	};
}