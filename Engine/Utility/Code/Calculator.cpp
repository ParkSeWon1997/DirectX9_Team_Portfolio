#include "..\..\Header\Calculator.h"
#include "Export_Utility.h"

CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CCalculator::~CCalculator()
{
}

HRESULT CCalculator::Ready_Calculator()
{
	return S_OK;
}

_float CCalculator::Compute_HeightOnTerrain(const _vec3 * pPos,
											const _vec3 * pTerrainVtxPos,
											const _ulong & dwCntX, 
											const _ulong & dwCntZ,
											const _ulong& dwVtxItv)
{

	_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);
	_float	fRatioX = _float(pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	_float	fRatioZ = _float(pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;

	D3DXPLANE	Plane;

	// 오른쪽 위
	if (fRatioX > fRatioZ)
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + dwCntX + 1],
			&pTerrainVtxPos[dwIndex + 1]);
	}
	 // 왼쪽 아래 
	else
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + 1],
			&pTerrainVtxPos[dwIndex]);
	}

	// Y = (-AX -CZ - D) / B


	return  (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;
}

_vec3 CCalculator::Picking_OnTerrain(HWND hWnd,
									CTerrainTex * pTerrainBufferCom,
									CTransform * pTerrainTransCom,
									const _ulong& dwCntX,
									const _ulong& dwCntZ)
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 뷰포트 -> 투영

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3		vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	
	vMousePos.z = 0.f;

	// 투영 -> 뷰스페이스
	_matrix	matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3	vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬
	_matrix		matWorld;
	pTerrainTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*		pTerrainVtxPos = pTerrainBufferCom->Get_VtxPos();

	_ulong	dwVtxIdx[3]{};
	_float	fU(0.f), fV(0.f), fDist(0.f);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwCntX + j;

			// 오른쪽 위

			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + dwCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]], 
			&pTerrainVtxPos[dwVtxIdx[0]],
			&pTerrainVtxPos[dwVtxIdx[2]],
			&vRayPos, &vRayDir, 
			&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtxPos[dwVtxIdx[1]].x + fU * (pTerrainVtxPos[dwVtxIdx[0]].x - pTerrainVtxPos[dwVtxIdx[1]].x),
							0.f,
							pTerrainVtxPos[dwVtxIdx[1]].z + fV * (pTerrainVtxPos[dwVtxIdx[2]].z - pTerrainVtxPos[dwVtxIdx[0]].z));
			}

			// V1 + U(V2 - V1) + V(V3 - V1)

			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[1]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtxPos[dwVtxIdx[2]].x + fU * (pTerrainVtxPos[dwVtxIdx[1]].x - pTerrainVtxPos[dwVtxIdx[2]].x),
							 0.f,
							 pTerrainVtxPos[dwVtxIdx[2]].z + fV * (pTerrainVtxPos[dwVtxIdx[0]].z - pTerrainVtxPos[dwVtxIdx[2]].z));
			}
		}
	}



	return _vec3(0.f, 0.f, 0.f);
}

_vec3 Engine::CCalculator::Picking_OnCubeTerrain(HWND hWnd, vector<CGameObject*> pTileBufferCom, CTransform* pTerrainTransCom, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 뷰포트 -> 투영

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3		vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;

	vMousePos.z = 0.f;

	// 투영 -> 뷰스페이스
	_matrix	matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3	vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix		matWorld;
	pTerrainTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	_vec3	dwVtxIdx[3]{};
	_float	fU(0.f), fV(0.f), fDist(0.f);
	_vec3 pPos;
	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			_ulong	dwIndex = i * dwCntX + j;
			_vec3* pTilePos = dynamic_cast<CTile*>(pTileBufferCom[dwIndex])->Get_CubeTex()->Get_Pos();
			pTileBufferCom[dwIndex]->GetTransForm()->Get_Info(INFO_POS, &pPos);

			// 오른쪽 위

			dwVtxIdx[0] = _vec3{ pPos.x + pTilePos[4].x, pTilePos[4].y, pPos.z + pTilePos[4].z };
			dwVtxIdx[1] = _vec3{ pPos.x + pTilePos[5].x, pTilePos[5].y, pPos.z + pTilePos[5].z };
			dwVtxIdx[2] = _vec3{ pPos.x + pTilePos[1].x, pTilePos[1].y, pPos.z + pTilePos[1].z };

			if (D3DXIntersectTri(&dwVtxIdx[1],
				&dwVtxIdx[0],
				&dwVtxIdx[2],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(dwVtxIdx[1].x + fU * (dwVtxIdx[0].x - dwVtxIdx[1].x),
					1.f,
					dwVtxIdx[1].z + fV * (dwVtxIdx[2].z - dwVtxIdx[0].z));
			}

			// V1 + U(V2 - V1) + V(V3 - V1)

			// 왼쪽 아래
			dwVtxIdx[0] = _vec3{ pPos.x + pTilePos[4].x, pTilePos[4].y, pPos.z + pTilePos[4].z };
			dwVtxIdx[1] = _vec3{ pPos.x + pTilePos[1].x, pTilePos[1].y, pPos.z + pTilePos[1].z };
			dwVtxIdx[2] = _vec3{ pPos.x + pTilePos[0].x, pTilePos[0].y, pPos.z + pTilePos[0].z };


			if (D3DXIntersectTri(&dwVtxIdx[2],
				&dwVtxIdx[1],
				&dwVtxIdx[0],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(dwVtxIdx[2].x + fU * (dwVtxIdx[1].x - dwVtxIdx[2].x),
					1.f,
					dwVtxIdx[2].z + fV * (dwVtxIdx[0].z - dwVtxIdx[2].z));
			}
		}
	}

	return  _vec3();
}

void Engine::CCalculator::Change_MouseMatrix(HWND hWnd, _vec3 vMousePos, _vec3* _vRayPos, _vec3* _vRayDir)
{
	// 뷰포트 -> 투영

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = vMousePos.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = vMousePos.y / -(ViewPort.Height * 0.5f) + 1.f;

	//vMousePos.z = 0.f;

	// 투영 -> 뷰스페이스
	_matrix	matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3	vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	*(_vRayPos) = vRayPos;
	*(_vRayDir) = vRayDir;
}

//오브젝트 윈도우 좌표로 변환
void Engine::CCalculator::Change_OnObjectMatrix(HWND hWnd, _vec3* vObjPos)
{
	// 월드 -> 뷰 스페이스
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXVec3TransformCoord(vObjPos, vObjPos, &matView);

	// 뷰 스페이스 -> 투영
	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXVec3TransformCoord(vObjPos, vObjPos, &matProj);

	// 투영 -> 뷰포트
	D3DVIEWPORT9 matViewPort;
	m_pGraphicDev->GetViewport(&matViewPort);

	vObjPos->x = vObjPos->x * (WINCX * 0.5) + (WINCX * 0.5);
	vObjPos->y = vObjPos->y * (-WINCY * 0.5) + (WINCY * 0.5);

	vObjPos->z = 0.f;
}				

_ulong CCalculator::Picking_OnTerrain_Tool(HWND hWnd,
	CTerrainTex* pTerrainBufferCom,
	CTransform* pTerrainTransCom,
	const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 뷰포트 -> 투영

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3		vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;

	vMousePos.z = 0.f;

	// 투영 -> 뷰스페이스
	_matrix	matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3	vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix		matWorld;
	pTerrainTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3* pTerrainVtxPos = pTerrainBufferCom->Get_VtxPos();

	_ulong	dwVtxIdx[3]{};
	_float	fU(0.f), fV(0.f), fDist(0.f);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwCntX + j;

			// 오른쪽 위

			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + dwCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&pTerrainVtxPos[dwVtxIdx[2]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return dwIndex;
			}

			// V1 + U(V2 - V1) + V(V3 - V1)

			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[1]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return dwIndex;
			}
		}
	}

	return  -1;
}

_ulong Engine::CCalculator::Picking_OnTerrain_Cube_Tool(HWND hWnd, vector<CGameObject*> pTileBufferCom, CTransform* pTerrainTransCom, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 뷰포트 -> 투영

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3		vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;

	vMousePos.z = 0.f;

	// 투영 -> 뷰스페이스
	_matrix	matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3	vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix		matWorld;
	pTerrainTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	_vec3	dwVtxIdx[3]{};
	_float	fU(0.f), fV(0.f), fDist(0.f);
	_vec3 pPos;
	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			_ulong	dwIndex = i * dwCntX + j;
			_vec3* pTilePos = dynamic_cast<CTile*>( pTileBufferCom[dwIndex])->Get_CubeTex()->Get_Pos();
			pTileBufferCom[dwIndex]->GetTransForm()->Get_Info(INFO_POS, &pPos);
			
			// 오른쪽 위

			dwVtxIdx[0] = _vec3{pPos.x - pTilePos[0].x, pTilePos[0].y, pPos.z + pTilePos[0].z};
			dwVtxIdx[1] = _vec3{ pPos.x + pTilePos[5].x, pTilePos[5].y, pPos.z + pTilePos[5].z };
			dwVtxIdx[2] = _vec3{ pPos.x + pTilePos[6].x, pTilePos[6].y, pPos.z - pTilePos[6].z };

			if (D3DXIntersectTri(&dwVtxIdx[0],
				&dwVtxIdx[1],
				&dwVtxIdx[2],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return dwIndex;
			}

			// V1 + U(V2 - V1) + V(V3 - V1)

			// 왼쪽 아래
			dwVtxIdx[1] = _vec3{ pPos.x - pTilePos[2].x, pTilePos[2].y, pPos.z - pTilePos[2].z };

			if (D3DXIntersectTri(&dwVtxIdx[0],
				&dwVtxIdx[2],
				&dwVtxIdx[1],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return dwIndex;
			}
		}
	}

	return  -1;
}

_vec3* Engine::CCalculator::Picking_PosVec(HWND hWnd)
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3		vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;

	vMousePos.z = 0.f;

	// 투영 -> 뷰스페이스
	_matrix	matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3	vRayPos;
	vRayPos = { 0.f, 0.f, 0.f };

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	
	vRayPos.y = 0.f;



	return &vRayPos;
}

_bool CCalculator::Check_PlayerMoveIndex(const _vec3* pPos, const vector<_int> veciIndex, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);

	for (int i = 0; i < (int)veciIndex.size(); ++i)
	{
		if (dwIndex == veciIndex[i])
			return false;
	}

	return true;
}

BOOL CCalculator::Picking_OnUI(HWND hWnd, CRcTex* pUIBifferCom, CTransform* pUITransCom, const _float& dwCntX, const _float& dwCntZ, const _ulong& dwVtxItv)
{


	




	return false;
}

CCalculator * CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*		pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Calculator Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

CComponent * CCalculator::Clone(void)
{
	return new CCalculator(*this);
}

void CCalculator::Free()
{
	CComponent::Free();
}
