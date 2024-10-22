#include "CSnow.h"
#include "Export_Utility.h"


CSnow::CSnow(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CParticle(pGraphicDev, _strObjName)
{
}

CSnow::CSnow(const CParticle& rhs)
	:CParticle(rhs)
{
}

CSnow::~CSnow()
{
}

void CSnow::resetParticle(Attribute* attribute)
{
	attribute->m_bAlive = true;


	//attribute->m_vPos = m_pTransForm->Get_Pos();

	GetRendomVector(&attribute->m_vPos, &m_vMinBox, &m_vMaxBox);


	attribute->m_vVelocity.x = GetRandomFloat(0.0f, 1.0f) * -5.0f;
	attribute->m_vVelocity.y = GetRandomFloat(0.0f, 1.0f) * -5.0f;
	attribute->m_vVelocity.z = 0.0f;
}

_int CSnow::Update_GameObject(const _float& fTimeDelta)
{

	std::list<Attribute>::iterator i;

	for (i = m_ParticleList.begin(); i != m_ParticleList.end(); i++)
	{
		i->m_vPos += i->m_vVelocity * fTimeDelta;

		// 포인트가 경계를 벗어났는가?  
		if (i->m_vPos.x < m_vMinBox.x || i->m_vPos.x > m_vMaxBox.x ||
			i->m_vPos.y < m_vMinBox.y || i->m_vPos.y > m_vMaxBox.y ||
			i->m_vPos.z < m_vMinBox.z || i->m_vPos.z > m_vMaxBox.z)
		{
			resetParticle(&(*i));
		}
	}


	__super::Update_GameObject(fTimeDelta);
	return 0;
}


CSnow* CSnow::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _vec3 MinBox, _vec3 MaxBox)
{
	CSnow* pInstance = new CSnow(pGraphicDev, _strObjName);

	pInstance->m_vbSize = 2048;
	pInstance->m_fSize = 0.05f;
	pInstance->m_vbOffset = 0;
	pInstance->m_vbBatchSize = 512;
	pInstance->m_vMinBox = MinBox;
	pInstance->m_vMaxBox = MaxBox;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	for (int i = 0; i < numparticles; i++)
	{
		pInstance->AddParticle();
	}

	return pInstance;
}
