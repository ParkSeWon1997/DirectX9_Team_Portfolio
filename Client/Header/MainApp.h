#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "Export_System.h"
#include "Export_Utility.h"

namespace Engine 
{

class CGraphicDev;
class CSceneMgr;
class CEventMgr;
}

class CMainApp : public CBase
{
private:				// ������, �Ҹ��� ����
	explicit CMainApp();
	virtual ~CMainApp();

public:			//public �Լ�, ���� -> protected �Լ�, ���� -> private �Լ�, ����
	HRESULT		Ready_MainApp();
	int			Update_MainApp(const float& fTimeDelta);
	void		LateUpdate_MainApp();
	void		Render_MainApp();

private:
	HRESULT		Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CSceneMgr** ppManagement);
	HRESULT		Ready_Event(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CEventMgr** ppEventMgr);
	HRESULT		SetUp_Setting(LPDIRECT3DDEVICE9* ppGraphicDev);

private:
	Engine::CGraphicDev*		m_pDeviceClass;
	Engine::CSceneMgr*		m_pManagementClass;
	Engine::CEventMgr*		m_pEventMgrClass;

	LPDIRECT3DDEVICE9			m_pGraphicDev;

public:
	static _uint g_iLightNum; // ���� ��

public:
	static CMainApp*		Create();

private:
	virtual		void		Free();


	

};


// ���� ����� �ִ� ���ϵ��� ���۷��� ����� ���� �ٿ��ֱ�
// ���� bin ������ �ִ� dll�� lib ������ �����ؼ� ���۷��� ���̺귯���� Ŭ���̾�Ʈ�� bin ���� �ٿ��ֱ�

// ax + by + cz + d = 0
// D3DXPLANE Plane;
// D3DXPlaneFromPoints();

//D3DXIntersectTri()

// ���콺 ������ ��ǥ -> viewport
// �����̳� �����