#include "CAnimation.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strAnimName)
	:m_pGraphicDev(pGraphicDev)
	, m_strAnimName(_strAnimName)
	, m_iMaxFrameCount(0)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{

}

Engine::CAnimation::CAnimation(const CAnimation& rhs)
	:m_pGraphicDev(rhs.m_pGraphicDev)
	, m_strAnimName(rhs.m_strAnimName)
	, m_iMaxFrameCount(rhs.m_iMaxFrameCount)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
{
	m_vecFrameInfo.reserve(rhs.m_vecFrameInfo.size());

	m_vecFrameInfo = rhs.m_vecFrameInfo;

	for (size_t i = 0; i < m_vecFrameInfo.size(); i++)
		m_vecFrameInfo[i].Texture->AddRef();


}

CAnimation::~CAnimation()
{
}

void CAnimation::Ready_Animtion(const _tchar* _strTexturePath, _uint _iFrameCount, _float _fDuration)
{
	tFrameInfo FrameInfo;
	FrameInfo.Texture = nullptr;


	m_iMaxFrameCount = _iFrameCount;
	m_vecFrameInfo.reserve(m_iMaxFrameCount);

	for (_uint i = 0; i < _iFrameCount; ++i)
	{
		TCHAR	szFileName[128] = L"";

		wsprintf(szFileName, _strTexturePath, i);

		HRESULT Result = D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&FrameInfo.Texture);
		assert(SUCCEEDED(Result), const _tchar * d = L"이미지로드실패");

		FrameInfo.Duration = _fDuration;

		m_vecFrameInfo.push_back(FrameInfo);
	}


}

void CAnimation::AnimUpdate(const _float& fTimeDelta)
{

	if (m_bFinish)
		return;

	m_fAccTime += fTimeDelta;

	if (m_vecFrameInfo[m_iCurFrm].Duration < m_fAccTime)
	{
		m_fAccTime = 0.f;
		++m_iCurFrm;

		//최대프레임보다 값이 높아지면 
		if (m_iMaxFrameCount <= m_iCurFrm)
		{
			m_bFinish = true;
			m_iCurFrm = m_iMaxFrameCount - 1;
		}
	}



}



CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strAnimName, const _tchar* _strTexturePath, _uint _iFrameCount, _float _fDuration)
{
	CAnimation* anim = new CAnimation(pGraphicDev, _strAnimName);
	anim->Ready_Animtion(_strTexturePath, _iFrameCount, _fDuration);

	return anim;
}

void CAnimation::Free()
{
	for (size_t i = 0; i < m_vecFrameInfo.size(); i++)
	{
		Safe_Release(m_vecFrameInfo[i].Texture);
	}
}
