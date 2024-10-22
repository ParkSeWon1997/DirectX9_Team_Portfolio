#pragma once
#include "UI.h"
#include "Export_Utility.h"
#include <ItemTool.h>

class CExplainPanel;
class CSlideBox;

class CSlideUI : public CUI
{
protected:
	explicit CSlideUI(LPDIRECT3DDEVICE9 pGraphicDevm, eITEMTOOL_TYPE eType);
	explicit CSlideUI(const CSlideUI& rhs);
	virtual ~CSlideUI();

public:
	void Set_ToolType(eITEMTOOL_TYPE eType) { m_eToolType = eType; }
	void Set_Show(_bool bShow) { 
		if (bShow)
		{
			for (auto& iter : m_vecSlideBox)
			{
				dynamic_cast<CSlideBox*>(iter)->Set_StartfX(100.f);
			}
		}

		m_bShow = bShow; }

	_bool Get_Show() { 
		return m_bShow; }

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

	//void SetSlideOnAndOff(bool _State) { m_bSlideOn = _State; };
	//BOOL GetSlideOn() { return m_bSlideOn; };

private:
	HRESULT Add_Component();
	 
public:
	static CSlideUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, eITEMTOOL_TYPE eType);

private:
	virtual void Free();

private:
	vector<CSlideBox*> m_vecSlideBox;

	eITEMTOOL_TYPE m_eToolType;

	_bool m_bShow;
};

