#pragma once
#include "ObjStatus.h"

namespace Engine {

class ENGINE_DLL CPlayerStatus :
    public CObjStatus
{
private:
    explicit CPlayerStatus()= delete;
    explicit CPlayerStatus(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CPlayerStatus(const CPlayerStatus& rhs);
    virtual ~CPlayerStatus();
public:
    HRESULT		Ready_PlayerStatus();
    PLAYERSTATE Get_Player_State() { return m_ePreState; }
    static CPlayerStatus* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    void Set_Player_State(PLAYERSTATE _State) { m_eCurState = _State; }
    

private:
    PLAYERSTATE m_eCurState;
    PLAYERSTATE m_ePreState;
    
};

}