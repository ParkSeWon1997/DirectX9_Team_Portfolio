#include "PlayerStatus.h"


CPlayerStatus::CPlayerStatus(LPDIRECT3DDEVICE9 pGraphicDev)
	:CObjStatus(pGraphicDev), m_ePreState(STATE_END), m_eCurState(IDLE)
{
}

CPlayerStatus::CPlayerStatus(const CPlayerStatus& rhs)
	:CObjStatus(rhs), m_ePreState(rhs.m_ePreState), m_eCurState(rhs.m_eCurState)
{
}

CPlayerStatus::~CPlayerStatus()
{
}

HRESULT CPlayerStatus::Ready_PlayerStatus()
{
	
	return S_OK;
}

CPlayerStatus* CPlayerStatus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerStatus* pInstance = new CPlayerStatus(pGraphicDev);

	if (FAILED(pInstance->Ready_PlayerStatus()))
	{
		Safe_Release(pInstance);
		MSG_BOX("PlayerStatus Create Failed");
		return nullptr;
	}

	return pInstance;
	
}
