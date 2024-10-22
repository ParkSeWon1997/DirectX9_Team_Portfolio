#pragma once

#include "GameObject.h"

namespace Engine {

class ENGINE_DLL CRenderer : public CBase
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
	void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Clear_RenderGroup();

public:
	void		Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);
	//void BeginOrtho(LPDIRECT3DDEVICE9& pGraphicDev)
	//{
	//	_matrix matOrtho;
	//	_matrix matOrthoView;

	//	_vec3 vEye, vAt, vUp;
	//	vEye = _vec3(0.f, 0.f, -1.f);  // 눈의 위치
	//	vAt = _vec3(0.f, 0.f, 0.f);    // 바라보는 지점
	//	vUp = _vec3(0.f, 1.f, 0.f);    // 위쪽 방향
	//	ZeroMemory(&matOrtho, sizeof(_matrix));

	//	D3DXMatrixLookAtLH(&matOrthoView, &vEye, &vAt, &vUp);
	//	pGraphicDev->SetTransform(D3DTS_VIEW, &matOrthoView);

	//	D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, m_fNear, m_fFar);
	//	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOrtho);

	//}

	//void EndOrtho(LPDIRECT3DDEVICE9& pGraphicDev)
	//{
	//	pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	//	pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	//}

private:
	list<CGameObject*>			m_RenderGroup[RENDER_END];

private:
	virtual void		Free();
};

}