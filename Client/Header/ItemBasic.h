#pragma once
#include "CItem.h"
class CItemBasic : // 기본 아이템. 월드상에 존재하는 아이템
    public CItem
{
private:
    explicit CItemBasic(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CItemBasic(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
    explicit CItemBasic(const CItem& rhs);
    virtual ~CItemBasic();

public:
    void Pickup_Item(_vec3 vSlotPos);
    _bool Get_ChangeRender() { return m_bChangeRander; }

public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float& fTimeDelta) override;
    void LateUpdate_GameObject() override;
    void Render_GameObject() override;

    //오브젝트에 의해 생성된 객체들만 사용할 것
    void SetCreateByObject( ) 
    {
        m_bIsCreateByObject = true;
    }
    void DropMotion(const _float& fTimeDelta);
    void CheckSign(int _iSign, int _Dir);

    void PickUp_Sound();


public:
    static CItem* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _szName);

private:
    virtual void Free();


    //오브젝트에 의해 생성된 객체들만 사용할 것
    BOOL m_bIsCreateByObject = false;
    BOOL m_bHowToDir = false;
    _float fItemSpeed = 0.0f;
private:
    _bool m_bChangeRander;

    _vec3 m_vPos = {};
    _vec3 m_vSlotPos = {};

    _vec3 m_vDir;
    _vec3 m_vUp;
};

