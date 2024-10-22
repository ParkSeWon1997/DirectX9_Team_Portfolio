#include "..\..\Header\GameObject.h"

#include "ProtoMgr.h"
#include "Transform.h"
#include "Export_Utility.h"
#include"Export_System.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_mapComponent{}
	, m_fViewZ(0.f)
	, m_pTransForm(nullptr)
	, m_bDelete(false)
	, m_strObjName(L"NONE")
	, m_iSlotNum(0)
	, m_fCollisionRadius(1.f)
	, m_bShader(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
}

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strName)
	: m_pGraphicDev(pGraphicDev)
	, m_mapComponent{}
	, m_fViewZ(0.f)
	, m_pTransForm(nullptr)
	, m_bDelete(false)
	, m_strObjName(_strName)
	, m_iSlotNum(0)
	, m_fCollisionRadius(1.f)
	, m_bShader(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_fViewZ(0.f)
	, m_pTransForm(nullptr)
	, m_bDelete(false)
	, m_Stat(rhs.m_Stat)
	, m_iSlotNum(rhs.m_iSlotNum)
	, m_fCollisionRadius(rhs.m_fCollisionRadius)
	, m_bShader(rhs.m_bShader)
{
	m_mapComponent[ID_DYNAMIC] = rhs.m_mapComponent[ID_DYNAMIC];
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}

HRESULT CGameObject::Ready_GameObject()
{
	return S_OK;
}

_int CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Update_System();
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return 0;
}

void CGameObject::LateUpdate_GameObject()
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component();


}

_bool Engine::CGameObject::Collision_Transform(CTransform* _Src, CTransform* _Dst)
{
	_vec3 vSrc, vSrcScale, vDst, vDstScale;

	_Src->Get_Info(INFO_POS, &vSrc);
	vSrcScale = _Src->Get_Scale();

	_Dst->Get_Info(INFO_POS, &vDst);
	vDstScale = _Dst->Get_Scale();

	_float iDistanceX = fabs(vSrc.x - vDst.x);
	_float fRadCX = vSrcScale.x * 0.5f + vDstScale.x * 0.5f;

	_float fDistanceZ = fabs(vSrc.z - vDst.z);
	_float fRadCY = vSrcScale.y * 0.5f + vDstScale.y * 0.5f;

	if (fDistanceZ > fRadCY || iDistanceX > fRadCX)
	{
		return false;
	}
	else
	{
		return true;
	}
}

_bool Engine::CGameObject::Collision_Circle(CGameObject* _pTarget)
{
	_vec3 vTargetPos;
	_vec3 vMyPos;
	_vec3 vDistance;

	//상대방과 나자신의 크기를 더한다. 이길이보다 작으면 충돌
	_float fDistance = m_fCollisionRadius + _pTarget->m_fCollisionRadius;

	vMyPos = GetTransForm()->Get_Pos();
	vTargetPos = _pTarget->GetTransForm()->Get_Pos();

	vMyPos.y = 0.f;
	vTargetPos.y = 0.f;


	if (fDistance >= D3DXVec3Length(&(vTargetPos - vMyPos)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//
//_bool Engine::CGameObject::Collision_Circle(CTransform* _Src, CTransform* _Dst)
//{
//
//	_vec3 vSrcpos, vSrcScale, vDstpos, vDstScale, vCamera;
//
//	_vec3 vSrcDir;
//	_vec3 vDstDir;
//
//	_matrix matView;
//	_float fSrcDiff;
//	_float fDstDiff;
//	_float Distance;
//
//	_Src->Get_Info(INFO_POS, &vSrcpos);
//	_Dst->Get_Info(INFO_POS, &vDstpos);
//
//	vSrcScale = _Src->Get_Scale();
//	vDstScale = _Dst->Get_Scale();
//
//	fSrcDiff = vSrcScale.y * 0.5;
//	fDstDiff = vDstScale.y * 0.5;
//	vCamera = scenemgr::Get_CurScene()->GetCameraObject()->Get_Pos();
//	vSrcDir = vCamera - vSrcpos;
//	D3DXVec3Normalize(&vSrcDir, &vSrcDir);
//	vSrcpos += vSrcDir * fSrcDiff;
//
//	vDstDir = vCamera - vSrcpos;
//	D3DXVec3Normalize(&vDstDir, &vDstDir);
//	vDstpos += vDstDir * fDstDiff;
//
//	vSrcpos.y = 0.f;
//	vDstpos.y = 0.f;
//
//	Distance = D3DXVec3Length(&(vDstpos - vSrcpos));
//	if (Distance < fSrcDiff+ fDstDiff)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
const tuple<_vec3, _vec3, _vec3, _vec3> Engine::CGameObject::Get_Info_vec()
{
	if (scenemgr::Get_CurScene()->GetPlayerObject() == nullptr)
		return make_tuple(_vec3(0, 0, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), _vec3(0, 0, 0));

	decltype(auto) pPlayer = scenemgr::Get_CurScene()->GetPlayerObject();

	Engine::CTransform* pPlayerTransformCom = pPlayer->GetTransForm();

	_vec3 PlayerPos;
	_vec3 thisLook;
	_vec3 thisUp;
	this->GetTransForm()->Get_Info(INFO_LOOK, &thisLook);
	this->GetTransForm()->Get_Info(INFO_UP, &thisUp);
	pPlayerTransformCom->Get_Info(INFO_POS, &PlayerPos);



	return make_tuple(PlayerPos, this->GetTransForm()->Get_Pos(), thisLook, thisUp);
}



CComponent* CGameObject::Find_Component(COMPONENTID eID, const _tchar* pComponentTag)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

HRESULT Engine::CGameObject::SetUp_Material(D3DXCOLOR vColor)
{
	D3DMATERIAL9			tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	//tMtrl.Diffuse = D3DXCOLOR(CToolMgr::m_fMtrlDiffuseColor[0], CToolMgr::m_fMtrlDiffuseColor[1], CToolMgr::m_fMtrlDiffuseColor[2], 1.f);
	//tMtrl.Ambient = D3DXCOLOR(CToolMgr::m_fMtrlAmbientColor[0], CToolMgr::m_fMtrlAmbientColor[1], CToolMgr::m_fMtrlAmbientColor[2], 1.f);
	//tMtrl.Specular = D3DXCOLOR(CToolMgr::m_fMtrlSpecularColor[0], CToolMgr::m_fMtrlSpecularColor[1], CToolMgr::m_fMtrlSpecularColor[2], 1.f);
	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tMtrl.Emissive = vColor;
	tMtrl.Power = 0.f;
	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}

void Engine::CGameObject::Compute_ViewZ(const _vec3* pPos)
{
	_matrix		matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	_vec3 vPos = *pPos;
	vPos.y = 0.f;

	_vec3	vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - vPos));
}


void CGameObject::Free()
{
	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}