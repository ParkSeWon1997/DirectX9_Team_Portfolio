#pragma once

#include "Base.h"
#include "Engine_Define.h"

namespace Engine {

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer();

public:
	_float			Get_TimeDelta() { return m_fTimeDelta; }

public:
	HRESULT		Ready_Timer(void);
	void		Update_TimeDelta(void);

private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_CpuTick;

	_float					m_fTimeDelta;

public:
	static CTimer*	Create(void);

private:
	virtual void	Free();
};

}