#include"CScienceUI.h"
#include "Export_Utility.h"
#include "Export_System.h"





CScienceUI::CScienceUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType)
	:CToolUI(pGraphicDev, _UI_Name, _ItemToolType)

{
	m_eUIState = eUIState;
}

CScienceUI::CScienceUI(const CScienceUI& rhs)
	: CToolUI(rhs)
{
}

CScienceUI::~CScienceUI()
{
}

_int CScienceUI::Update_GameObject(const _float& fTimeDelta)
{

	__super::Update_GameObject(fTimeDelta);
	if (UI_Collision() && Engine::Get_DIMouseState(DIM_LB) & 0x80)
	{
		Pop_SidePanel();

	}
	return 0;
}

CScienceUI* CScienceUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType)
{
	CScienceUI* pInstance = new CScienceUI(pGraphicDev, _State, _UI_Name, _ItemToolType);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CScienceUI::Free()
{
	//추후 오브젝트 생성, 삭제 기능으로 변경되면 해당 릴리즈 필요 없음
	this->m_pBufferCom->Release();
	this->m_pTextureCom->Release();
	this->m_pTransForm->Release();
	__super::Free();
}
