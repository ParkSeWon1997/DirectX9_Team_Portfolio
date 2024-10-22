#include "ResObject.h"
#include "ItemBasic.h"

CResObject::CResObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_eCurState(RES_DEAD),
	m_ePreState(RES_DEAD),
	m_bHit(false),
	m_fFrame(0),
	m_fFrameEnd(0),
	m_ePlayerLook(LOOK_END),
	m_fFrameSpeed(0.f)
{
}

CResObject::CResObject(const CResObject& rhs)
	: CGameObject(rhs.m_pGraphicDev),
	m_eCurState(rhs.m_eCurState),
	m_ePreState(rhs.m_ePreState),
	m_bHit(false),
	m_fFrame(rhs.m_fFrame),
	m_fFrameEnd(rhs.m_fFrameEnd),
	m_ePlayerLook(rhs.m_ePlayerLook),
	m_fFrameSpeed(rhs.m_fFrameSpeed)
{
}

CResObject::~CResObject()
{
}



HRESULT CResObject::Ready_GameObject()
{
	return S_OK;
}

_int CResObject::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CResObject::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CResObject::Render_GameObject()
{
}

void CResObject::Free()
{
	__super::Free();
}




void CResObject::CreateItem(const _tchar* _itemName, CGameObject* _Type, LPDIRECT3DDEVICE9 pGraphicDev, int _iCount)
{
	if (_iCount <= -1)
		_iCount = 3;

	srand(static_cast<unsigned int>(time(nullptr)));
	int iItemCount = rand() % 1 + _iCount;	//아이템 갯수용
	for (int i = 0; i < iItemCount; ++i)
	{

		_vec3 vPos;
		_Type->GetTransForm()->Get_Info(INFO_POS, &vPos);//m_pTransForm->Get_Info(INFO_POS, &vPos);
		CGameObject* pGameObj = CItemBasic::Create(pGraphicDev, _itemName);
		dynamic_cast<CItemBasic*>(pGameObj)->SetCreateByObject();
		//생성된 객체는 각 방향과 나아갈 축을 가지고 태어난다.
		pGameObj->GetTransForm()->Set_Pos(vPos.x, 0.f, vPos.z);
		scenemgr::Get_CurScene()->AddGameObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::ITEM, pGameObj);


	}


}

