#pragma once
#include "Base.h"
#include "Engine_Define.h"

#include "Light.h"

namespace Engine {

	class ENGINE_DLL CLightMgr : public CBase
	{
		DECLARE_SINGLETON(CLightMgr)

	private:
		explicit CLightMgr();
		virtual ~CLightMgr();

	public:
		CLight* Get_Light(_int _iIndex) { return m_vecLight[_iIndex]; }
		TIME_STATE Get_TimeIndex() { return m_eChangeIndex; }

		_int Get_Day() { return m_iDay; }
		_bool IsChangeTime();

	public:
		HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
			const D3DLIGHT9* pLightInfo,
			const _uint& iIndex);
		_int Change_Light(const _float& fTimeDelta, _int _iIndex);

	private:
		vector<CLight*>			m_vecLight;
		_float m_fPreTime;

		TIME_STATE m_eChangeIndex; // 0 : morning, 1 : afternoon, 2 : night
		TIME_STATE m_ePreIndex;
		_int m_iDay;

	private:
		virtual void	Free();

	};

}