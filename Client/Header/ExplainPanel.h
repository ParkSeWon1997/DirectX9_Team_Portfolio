#pragma once
#include "Export_Utility.h"
#include <Slot.h>
#include <ButtonUI.h>
#include "UI.h"

class CExplainPanel : public CUI
{
protected:
	explicit CExplainPanel(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, wstring strItemKey);
	explicit CExplainPanel(const CExplainPanel& rhs);
	virtual ~CExplainPanel();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

public:
	void Set_Show(bool _bShow) { m_bShow = _bShow; }
	_bool Get_Show() { return m_bShow; }

	void Set_m_bSlideBoxColl(bool _bColl) { m_bSlideBoxColl = _bColl; }
	_bool Get_m_bSlideBoxColl() { return m_bSlideBoxColl; }

private:
	HRESULT			Add_Component();

public:
	static CExplainPanel* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 vPos, wstring strItemKey);

private:
	virtual void Free();

private:
	_vec3 m_vPos;
	_bool m_bShow = false;
	_bool m_bSlideBoxColl;

	CREATEINFO m_tCreateInfo;
	wstring m_strItemKey;

	CSlot* m_pItem[3];
	CButtonUI* m_pButton;
};

