#pragma once
#include "UI.h"
#include "Slot.h"
#include "Engine_Macro.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CInven : public CUI
{
private:
	explicit CInven(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInven();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT			Add_Component() override;

public:
	static CInven* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};
