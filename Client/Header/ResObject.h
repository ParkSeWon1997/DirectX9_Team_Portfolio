#pragma once
#include "Export_Utility.h"

class CResObject :
    public CGameObject
{
protected:
    enum RESOBJECTSTATE { RES_IDLE, RES_HIT_1, RES_HIT_2, RES_DEAD, RES_DEAD2, RES_FINAL, RESOBJECT_END };

protected:
    explicit CResObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CResObject(const CResObject& rhs);
    virtual ~CResObject();

public:
    void Set_Attack() { if (!m_bHit && m_Stat.fHP > 0) m_Stat.fHP -= 1.f; } // ������Ʈ�� -1�� ����
    void Set_Attack_State(_bool _bAttack) { m_bHit = _bAttack; } // �÷��̾� ���� ���� �� true�� �������ְ� �÷��̾� ���� �������� ���� �� false�� �������ش�.
    void Set_Player_Look(LOOKDIR _eLook) { m_ePlayerLook = _eLook; }
    RESOBJID Get_Resourse_ID() { return m_eObject_id; }
    _bool Get_AttackState() { return m_bHit; }



public:
    //������Ʈ�� �������� �������� �� ����ϴ� �Լ�
   static void CreateItem(const _tchar* _itemName,CGameObject* _Type, LPDIRECT3DDEVICE9 pGraphicDev,int _iCount=3);

   
   
  

public:
    virtual HRESULT Ready_GameObject()						 override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject()					 override;
    virtual void Render_GameObject()					override;

private:
    virtual HRESULT			Add_Component() PURE;
    virtual void            Change_Frame_Event() PURE;
    virtual void			Check_FrameState() PURE;
    virtual void Ready_Stat() PURE;

protected:
    Engine::CRcTex* m_pBufferCom;
    Engine::CTexture* m_pTextureCom[RESOBJECT_END];

    RESOBJECTSTATE m_eCurState;
    RESOBJECTSTATE m_ePreState;

    LOOKDIR m_ePlayerLook;
    RESOBJID            m_eObject_id;
    _float				m_fFrame = 0.f;
    _float				m_fFrameEnd;
    _float              m_fFrameSpeed;
    _bool m_bHit;

protected:
    virtual void Free() override;
};