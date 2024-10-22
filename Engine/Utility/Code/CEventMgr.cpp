
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CEventMgr)

CEventMgr::CEventMgr()
{
}

CEventMgr::~CEventMgr()
{
	Free();
}

HRESULT Engine::CEventMgr::EventUpdate()
{
	//	==============================================
	//	이전 프레임에서 등록해둔 DeadObject 들을 삭제한다.
	//	==============================================

	for (size_t i = 0; i < mVecDeadObj.size(); ++i)
	{
		Safe_Release(mVecDeadObj[i]);
	}
	mVecDeadObj.clear();

	//	========
	//	Event처리
	//	========

	for (size_t i = 0; i < mVecEvent.size(); ++i)
	{
		excute(mVecEvent[i]);
	}
	mVecEvent.clear();

	return 0;
}

void CEventMgr::excute(const tEvent& _eve)
{
	_eve.eEven;

	switch (_eve.eEven)
	{
	case eEVENT_TYPE::CREATE_OBJECT:
	{
		// lParam : object Adress
		// wParam : Obj Group TYPE
		// pParam : LayerType
		CGameObject* pNewObj = (CGameObject*)_eve.lParam;
		eLAYER_TYPE eLayerType = (eLAYER_TYPE)_eve.pParam;
		eOBJECT_GROUPTYPE eObjType = (eOBJECT_GROUPTYPE)_eve.wParam;

		CSceneMgr::GetInstance()->Get_CurScene()->AddGameObject(eLayerType, eObjType, pNewObj);
	}
	break;
	case eEVENT_TYPE::DELETE_OBJECT:
	{
		// lParam : object Adress
		// Object 를 Dead 상태로 변경
		// 삭제예정 오브젝트들을 모아둔다.

		CGameObject* pDeadObj = (CGameObject*)_eve.lParam;
		pDeadObj->SetDeleteObj();
		mVecDeadObj.push_back(pDeadObj);

	}
	break;
	case eEVENT_TYPE::SCENE_CHANGE:
		// lParam: pScene;
		(CScene*)_eve.lParam;
		scenemgr::Change_CurScene((CScene*)_eve.lParam);
		break;
	}

}
void CEventMgr::Free()
{

}
