#include "CAnimator.h"
#include "Export_System.h"
#include "Export_Utility.h"

CAnimator::CAnimator(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
	, m_pCurAnimation(nullptr)
	, m_bLoop(false)
{

}

CAnimator::CAnimator(const CAnimator& rhs)
	:CComponent(rhs)
	, m_pCurAnimation(nullptr)
	, m_bLoop(false)
{

}

CAnimator::~CAnimator()
{

}


HRESULT CAnimator::Ready_Animation()
{
	return S_OK;
}

_int CAnimator::Update_Component(const _float& fTimeDelta)
{
	if (nullptr != m_pCurAnimation)
	{
		if (m_pCurAnimation->IsFinish() && this->IsLoop())
		{
			m_pCurAnimation->ResetFrame();
		}
	}


	return 0;
}
void CAnimator::LateUpdate_Component()
{
	if (nullptr != m_pCurAnimation)
	{
		//현재선택된 애니메이션이 끝났지만 
		//애니메이터가 루프로 설정되어있다면 프레임리셋 (ex:IDLE모션 )

		m_pCurAnimation->AnimUpdate(Engine::Get_TimeDelta(L"Timer_FPS60"));
	}

	return;
}

CAnimation* CAnimator::Find_Animation(const _tchar* _key)
{
	auto	iter = find_if(m_mapAnimation.begin(), m_mapAnimation.end(), CTag_Finder(_key));

	if (iter == m_mapAnimation.end())
	{
		assert(false, const _tchar * _error = L"애니메이션 찾을 수 없음");
		return nullptr;
	}

	return iter->second;
}

CComponent* CAnimator::Clone()
{
	return new CAnimator(*this);
}

CAnimator* CAnimator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAnimator* pInstance = new CAnimator(pGraphicDev);

	if (FAILED(pInstance->Ready_Animation()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pInstance;
}



void Engine::CAnimator::SetCurAnimationFrame(const _tchar* _strAnimKey, _uint _iFrame)
{
	Find_Animation(_strAnimKey)->m_iCurFrm = _iFrame;
	Find_Animation(_strAnimKey)->m_fAccTime = 0.f;
}

void CAnimator::Free()
{

	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_mapAnimation.begin(), m_mapAnimation.end(), CDeleteMap());
		m_mapAnimation.clear();
	}

	__super::Free();
}
