#include "..\..\Header\LightMgr.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr() : m_fPreTime(0), m_eChangeIndex(MORNING), m_iDay(0), m_ePreIndex(MORNING)
{
}

CLightMgr::~CLightMgr()
{
	Free();
}

_bool Engine::CLightMgr::IsChangeTime()
{
	if (m_ePreIndex == m_eChangeIndex)
		return false;

	m_ePreIndex = m_eChangeIndex;
	return true;
}

HRESULT CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	CLight* pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_vecLight.push_back(pLight);

	return S_OK;
}

_int CLightMgr::Change_Light(const _float& fTimeDelta, _int _iIndex)
{
	m_fPreTime += fTimeDelta * 1000;

	if (m_eChangeIndex == MORNING)
	{
		if (120000 >= m_fPreTime)
			return m_eChangeIndex;

		m_eChangeIndex = AFTERNOON;
	}
	else if (m_eChangeIndex == AFTERNOON)
	{
		if (90000 >= m_fPreTime)
			return m_eChangeIndex;

		m_eChangeIndex = NIGHT;
	}
	else if (m_eChangeIndex == NIGHT)
	{
		if (60000 >= m_fPreTime)
			return m_eChangeIndex;

		m_eChangeIndex = MORNING;
		m_iDay += 1;
	}

	m_fPreTime = 0;
	return m_eChangeIndex;
}

void CLightMgr::Free()
{
	for_each(m_vecLight.begin(), m_vecLight.end(), CDeleteObj());
	m_vecLight.clear();
}