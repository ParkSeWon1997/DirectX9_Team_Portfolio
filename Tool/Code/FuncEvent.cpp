#include "framework.h"
#include "FuncEvent.h"

void CreateObject(CGameObject* _pObj, eOBJECT_GROUPTYPE _eGroup)
{
	tEvent evn = {};
	evn.eEven = eEVENT_TYPE::CREATE_OBJECT;

	evn.lParam = (DWORD_PTR)_pObj;
	evn.wParam = (DWORD_PTR)_eGroup;

	eventmgr::AddEvent(evn);
}

void CreateObject(eLAYER_TYPE _eLayer, eOBJECT_GROUPTYPE _eObjGroup, CGameObject* _pObj)
{
	tEvent evn = {};
	evn.eEven = eEVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;
	evn.pParam = (DWORD_PTR)_eLayer;
	evn.wParam = (DWORD_PTR)_eObjGroup;

	eventmgr::AddEvent(evn);
}

void DeleteObject(CGameObject* _pObj)
{
	tEvent evn = {};
	evn.eEven = eEVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;

	eventmgr::AddEvent(evn);
}

