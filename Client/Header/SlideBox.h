#pragma once
#include "UI.h"
#include "Export_Utility.h"
#include <ExplainPanel.h>

class CSlideBox :public CUI
{

protected:
	explicit CSlideBox(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CSlideBox(const CSlideBox& rhs);
	virtual ~CSlideBox();

public:
	void Set_Item(CItem* pItem);
	void Set_StartfX(_float _fX) { m_fX = m_vPos.x - _fX; }
	
	_float Get_fX() { return m_fX; }
	_float Get_fY() { return m_fY; }

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT			Add_Component();
	void Input_Mouse();

public:
	static CSlideBox* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	virtual void Free();

private:
	_vec3 m_vPos;
	_bool m_bPanelShow;

	CItem* m_pItem;
	CExplainPanel* m_pPanel;
};

