#pragma once
#include "Component.h"
#include "CAnimation.h"

namespace Engine
{
	class ENGINE_DLL CAnimator :
		public CComponent
	{
	public:
		void SetCurAnimation(const _tchar* _strAnimKey) { m_pCurAnimation = Find_Animation(_strAnimKey);	m_pCurAnimation->ResetFrame(); }
		void SetCurAnimationFrame(const _tchar* _strAnimKey, _uint _iFrame);
		void ChangeAnimation(const _tchar* _strAnimKey)
		{
			if (nullptr!= m_pCurAnimation)
				m_pCurAnimation->ResetFrame();
			m_pCurAnimation = Find_Animation(_strAnimKey);
			m_pCurAnimation->ResetFrame();
		}
		//Ư�� �ִϸ��̼��϶��� ������ ������ //�ȴ»����϶��� Ư������������������
		_int	GetAnimFrame(const _tchar* _strAnimKey) { return Find_Animation(_strAnimKey)->m_iCurFrm; }
		_int	GetAnimMaxFrame(const _tchar* _strAnimKey) { return Find_Animation(_strAnimKey)->m_iMaxFrameCount-1; }

		void SetAnimTexture() { m_pCurAnimation->SetCurTexture(); }

		void SetLoopAnimator(_bool _bLoop) { m_bLoop = _bLoop; }
		_bool IsLoop() { return m_bLoop; }
		_bool IsFinish(const _tchar* _strAnimKey) { return Find_Animation(_strAnimKey)->m_bFinish; }

		void AddAnimation(const _tchar* _strAnimKey, Engine::CAnimation* _pAnimation) { m_mapAnimation.insert({ _strAnimKey, _pAnimation }); }

	public:
		virtual HRESULT		Ready_Animation();
		virtual _int		Update_Component(const _float& fTimeDelta);
		virtual void		LateUpdate_Component();
		virtual CComponent* Clone()override;

		static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	protected:
		explicit CAnimator() = delete;
		explicit CAnimator(LPDIRECT3DDEVICE9 pGraphicDev);
		explicit CAnimator(const CAnimator& rhs);
		virtual ~CAnimator();

	protected:
		virtual void		Free();
	private:
		CAnimation* Find_Animation(const _tchar* _key);

	private:
		map<const _tchar*, Engine::CAnimation*>	m_mapAnimation;
		Engine::CAnimation* m_pCurAnimation;
		_bool		m_bLoop;//����ִϸ��̼� ��������

	};

}