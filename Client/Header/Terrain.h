#pragma once

#include "Base.h"
#include "GameObject.h"

namespace Engine {

class CTerrainTex;
class CTexture;
class CTransform;

}

class CTerrain :	public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev, wstring pKey);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain();

public:
	Engine::CTerrainTex* Get_Buffer() { return m_pBufferCom; };
public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

private:
	Engine::CTerrainTex*		m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;

	wstring m_pKey;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9	pGraphicDev, wstring pKey);

private:
	virtual void Free() override;
};

