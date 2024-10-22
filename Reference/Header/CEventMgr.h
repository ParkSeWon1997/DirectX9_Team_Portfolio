#pragma once

#include "Base.h"
#include "Engine_Define.h"


namespace Engine
{
	class ENGINE_DLL CEventMgr : public CBase
	{
		DECLARE_SINGLETON(CEventMgr)
	public:
		HRESULT EventUpdate();
	public:
		void AddEvent(const tEvent& _eve) { mVecEvent.push_back(_eve); }

	private:
		explicit CEventMgr();
		virtual ~CEventMgr();
	private:
		vector<tEvent> mVecEvent;
		vector<CGameObject*> mVecDeadObj;
	private:
		void excute(const tEvent& _eve);

	private:
		virtual void		Free();

	};
}