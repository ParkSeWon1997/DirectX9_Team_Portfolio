#pragma once
#include "CUI.h"

class CSlideUI;

class CItemTool;
class CSlideUI;
class CLightUI;
class CScienceUI;
class CClothUI;
class CWeaponUI;
class CAliveUI;
class CEquiment;


class CToolUI :public CUI
{
	//이 클래스는 부모를 통해 생성되고 관리는 데이터매니져의 부모인 CUI를 통해 관리된다.
	//데이터 매니져는 레이어에 있는 CUI를 찾아 Get을 통해 해당 객체의 데이터를 관리한다.
	//이 객체는 각 탭에 대한 데이터를 뿌려주는 역할을 한다.

protected:
	explicit CToolUI(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType);
	explicit CToolUI(const CToolUI& rhs);
	virtual ~CToolUI();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	//각 자식들은 이 함수를 오버라이딩 해서 사용한다.
	//virtual void Pop_SidePanel();

	void SetGameData(CItemTool* _ItemTool, eITEMTOOL_TYPE _ItemType);


	const CAliveUI* GetAliveUI() { return m_pAliveUI; }
	const CEquiment* GetEquimentI() { return m_pEquimentUI; }
	const CLightUI* GetLightUI() { return m_pLightUI; }
	const CScienceUI* GetScienceUI() { return m_pScienceUI; }
	const CClothUI* GetClothUI() { return m_pClothUI; }
	const CWeaponUI* GetWeaponUI() { return m_pWeaponUI; }




	 static   CToolUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, const _tchar* _UI_Name);


	virtual void Free() override;
protected:
	//데이터를 관리하기 위한 키값, 키값은 각 슬롯이 생성이 되면 고유의 키값을 가진다.
	eITEMTOOL_TYPE m_eItemType;

	//각 슬롯에 데이터들은 이 클래스가 관리하고 생성
	//각 슬롯이 주기적으로 도는 로직들은 CUI의 함수들을 오버라이딩 해서 사용한다.
private:
	static  CAliveUI* m_pAliveUI;
	static  CEquiment* m_pEquimentUI;
	static  CLightUI* m_pLightUI;
	static  CScienceUI* m_pScienceUI;
	static  CClothUI* m_pClothUI;
	static  CWeaponUI* m_pWeaponUI;

private:
	static CSlideUI* m_pSlideUI[5];
};

