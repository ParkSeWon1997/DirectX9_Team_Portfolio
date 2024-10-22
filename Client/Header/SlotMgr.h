#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "Slot.h"
#include <SlideBox.h>

class CSlotMgr :
    public CBase
{
    DECLARE_SINGLETON(CSlotMgr)

private:
    explicit CSlotMgr();
    virtual ~CSlotMgr();
    
public:
    void Set_Create_Menu(int _iNum, CItem* pItem);
    void Move_InvenItem(CItem* pItem, _uint _iItemNum, _uint _iMoveNum) // �κ��丮 ������ �̵� 
    {
        m_pItemArr[_iMoveNum] = pItem;
        m_pItemArr[_iItemNum] = nullptr;
    }
    void Remove_InvenItem(_uint _iItemNum, _uint _iItemCount = 1);
    void Get_BoxPos(BOX_TYPE eType, int _iNum, _vec3* vPos) {
        vector< CSlot*> pBox = m_mapBox.find(eType)->second;
        vPos->x = pBox[_iNum]->Get_fX();
        vPos->y = pBox[_iNum]->Get_fY();
    }
    
    vector<CSlot*> Get_BoxList(BOX_TYPE eType) { return m_mapBox.find(eType)->second; }
    CItem* Get_InvenItem(_uint iNum) { return m_pItemArr[iNum]; }
    CItem* Get_ArmorItem(ARMOR_SLOT_TYPE eArmorSlotType) { return m_pArmorArr[eArmorSlotType - 15]; }
    void Get_Inven(CItem** pInvenItem) { memcpy(pInvenItem, m_pItemArr, sizeof(m_pItemArr)); }
    _bool Check_InvenItemCount(wstring strName, _int iNeedNum);
    void Remove_CreateItem(wstring strName, _uint iItemCount);

    // ������ ���� Ȯ��
    // ������ ������ ���ϰ� ���� �� L"" ���� �Ѿ��.
    void Set_ArmorItem(ARMOR_SLOT_TYPE eArmorSlotType, CItem* pItem, _uint _iItemNum);
    //wstring Get_ArmorItem(ARMOR_SLOT_TYPE eArmorSlotType) { return m_pArmorArr[eArmorSlotType - 15] != nullptr ? m_pArmorArr[eArmorSlotType - 15]->GetObjName() : L""; }
    void Change_ArmorItem(CItem* pItem, ARMOR_SLOT_TYPE eArmorSlotType, _uint _iItemNum, _vec3 vSlotPos);
    void Remove_ArmorItem(ARMOR_SLOT_TYPE eArmorSlotType);

    //�丮 ĭ
    void Set_CookItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring strKeyName, _vec3 vSlotPos, _uint iSlotNum);
    void Remove_CookItem();
    _bool Start_Cook();

public:
    HRESULT Add_InvenBoxList(LPDIRECT3DDEVICE9 pGraphicDev, BOX_TYPE eType, BOX_DIR eDir ,int _iNum);
    void Update_InvenBoxMgr(const _float& fTimeDelta, BOX_TYPE eType);
    void LateUpdate_InvenBoxMgr(BOX_TYPE eType);
    void Render_InvenBoxMgr(BOX_TYPE eType);
    
    _bool	AddItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring strItemKey, _vec3* vSlotPos);	//��ȯ����False�� �����۽������(�����۲���)
    _bool Check_AddItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring strItemKey, _vec3* vSlotPos);

    void Box_Release(BOX_TYPE eType);
private:
    virtual void Free();

private:
    map <BOX_TYPE, vector<CSlot*>> m_mapBox;

    CItem* m_pItemArr[INVENCNT]; // �κ��丮
    CItem* m_pArmorArr[3]; // ���� ������ 
    CItem* m_pCookArr[4]; // �丮ĭ�� ���� ������ 
};

