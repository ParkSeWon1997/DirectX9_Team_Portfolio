#pragma once
#include "Base.h"
#include "Engine_Define.h"
class CPlayerMgr :
    public CBase
{
	DECLARE_SINGLETON(CPlayerMgr)

public:
	CGameObject* Get_Player_Pointer() {	return m_pPlayer; }
	void Set_Player_Pointer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
	void Set_Player_Scale(_vec3 _vScale);
	void	Initailize();
private:
	explicit CPlayerMgr();
	virtual ~CPlayerMgr();

	// CBase을(를) 통해 상속됨
	void Free() override;

	CGameObject* m_pPlayer;
};

