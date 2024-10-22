#include "stdafx.h"
#include "CTreeLeafFall.h"
#include "Export_Utility.h"


CTreeLeafFall::CTreeLeafFall(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CParticle(pGraphicDev, _strObjName)
	, m_fLifeTime(1.f)
	, m_fAccTime(0.f)
{
}

CTreeLeafFall::CTreeLeafFall(const CParticle& rhs)
	:CParticle(rhs)
	, m_fLifeTime(1.f)
	, m_fAccTime(0.f)
{
}

CTreeLeafFall::~CTreeLeafFall()
{
}

void CTreeLeafFall::resetParticle(Attribute* attribute)
{

	//attribute->m_vPos = m_pTransForm->Get_Pos();
	GetRendomVector(&attribute->m_vPos, &m_vMinBox, &m_vMaxBox);

	attribute->m_vVelocity.x = GetRandomFloat(0.0f, 1.0f) * -1.0f;
	attribute->m_vVelocity.y = GetRandomFloat(0.2f, 1.0f) * -4.0f;
	attribute->m_vVelocity.z = GetRandomFloat(0.0f, 1.0f) * -1.0f;
}
void CTreeLeafFall::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3 vPos = GetTransForm()->Get_Pos();

	m_fViewZ = 4.5f;
}

_int CTreeLeafFall::Update_GameObject(const _float& fTimeDelta)
{
	if (m_fAccTime >= m_fLifeTime)
	{
		DeleteObject(this);
		return 0;
	}

	std::list<Attribute>::iterator i;

	for (i = m_ParticleList.begin(); i != m_ParticleList.end(); i++)
	{
		i->m_vPos += i->m_vVelocity * fTimeDelta;
		if (i->m_bAlive)
		{

			// 포인트가 경계를 벗어났는가?  
			if (i->m_vPos.y < m_vMinBox.y || i->m_vPos.y > m_vMaxBox.y)
			{
				resetParticle(&(*i));
				i->m_bAlive = false;
			}
		}
	}
	m_fAccTime += fTimeDelta;


	__super::Update_GameObject(fTimeDelta);
	return 0;
}


CTreeLeafFall* CTreeLeafFall::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _vec3 _vPos, _vec3 MinBox, _vec3 MaxBox)
{
	CTreeLeafFall* pInstance = new CTreeLeafFall(pGraphicDev, _strObjName);

	pInstance->m_vbSize = 2048;
	pInstance->m_fSize = 0.1f;
	pInstance->m_vbOffset = 0;
	pInstance->m_vbBatchSize = 512;
	pInstance->m_vMinBox = MinBox;
	pInstance->m_vMaxBox = MaxBox;
	//_vPos.x *= 0.5f;
	//_vPos.y *= 0.5f;
	//_vPos.z *= 0.5f;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->m_pTransForm->Set_Pos(_vPos);

	for (int i = 0; i < numparticles; i++)
	{
		pInstance->AddParticle();
	}

	return pInstance;
}
