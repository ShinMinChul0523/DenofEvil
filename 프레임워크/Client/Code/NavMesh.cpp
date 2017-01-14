#include "stdafx.h"
#include "NavMesh.h"
#include "NavCell.h"

IMPLEMENT_SINGLETON(CNavMesh)

CNavMesh::CNavMesh(void)
	: m_pGraphicDev(NULL)
	, m_pContext(NULL)
	, m_uiReserveSize(0)
	, m_uiIdxCnt(0)
	, m_uiCurIndex(0)
{
}

CNavMesh::~CNavMesh(void)
{
}

void CNavMesh::Reserve_CellContainerSize(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const UINT& uiSize)
{
	m_pGraphicDev	= pGraphicDev;
	m_pContext		= pContext;
	m_uiReserveSize	= uiSize;

	m_vecNavCell.reserve(m_uiReserveSize);
}

HRESULT CNavMesh::Add_Cell(const XMFLOAT3* pPointA, const XMFLOAT3* pPointB, const XMFLOAT3* pPointC)
{
	if (m_uiReserveSize == 0) 
	{
		MSG_BOX("NavMesh Add_Cell Failed");
		return E_FAIL;
	}

	CNavCell* pCell = CNavCell::Create(m_pGraphicDev, m_pContext, pPointA, pPointB, pPointC, m_uiIdxCnt);
	++m_uiIdxCnt;

	m_vecNavCell.push_back(pCell);
	return S_OK;
}

void CNavMesh::Link_Cell(void)
{
	// **************************************************************************************
	vector<CNavCell*>::iterator		iter = m_vecNavCell.begin();

	if (iter == m_vecNavCell.end())
		return;

	for (; iter != m_vecNavCell.end(); ++iter)
	{
		vector<CNavCell*>::iterator		iter_Target = m_vecNavCell.begin();

		while (iter_Target != m_vecNavCell.end())
		{
			if (iter == iter_Target)
			{
				++iter_Target;
				continue;
			}

			if ((*iter_Target)->Compare_Point((*iter)->Get_Point(CNavCell::POINT_A)
				, (*iter)->Get_Point(CNavCell::POINT_B), (*iter)))
			{
				(*iter)->Set_Neighbor(CNavCell::NEIGHBOR_AB, (*iter_Target));
			}

			else if ((*iter_Target)->Compare_Point((*iter)->Get_Point(CNavCell::POINT_B)
				, (*iter)->Get_Point(CNavCell::POINT_C), (*iter)))
			{
				(*iter)->Set_Neighbor(CNavCell::NEIGHBOR_BC, (*iter_Target));
			}

			else if ((*iter_Target)->Compare_Point((*iter)->Get_Point(CNavCell::POINT_C)
				, (*iter)->Get_Point(CNavCell::POINT_A), (*iter)))
			{
				(*iter)->Set_Neighbor(CNavCell::NEIGHBOR_CA, (*iter_Target));
			}

			++iter_Target;
		}
	}
}

BOOL CNavMesh::MoveOnNavMesh(const XMFLOAT3* pPos, UINT& uiCurrentIdx, FLOAT* pResultY, XMFLOAT3* vColDir)
{
	if (m_vecNavCell.empty())
		return false;

	CNavCell* pNavCell = NULL;

	const XMFLOAT3* vNormal = m_vecNavCell[uiCurrentIdx]->Get_Normal(CNavCell::LINE_AB);
	const XMFLOAT3* vPoint = m_vecNavCell[uiCurrentIdx]->Get_Point(CNavCell::POINT_A);

	for (int iVecIdx = 0; iVecIdx < 3; ++iVecIdx)
	{
		XMVECTOR vDestDir = XMLoadFloat3(pPos) - XMLoadFloat3(&vPoint[iVecIdx]);

		XMVECTOR vDotResult = XMVector3Dot(vDestDir, XMLoadFloat3(&vNormal[iVecIdx]));
		BOOL bInNavMesh = FALSE;

		if (XMVectorGetX(vDotResult) > 0.f)
		{
			if (vColDir)
				*vColDir = vNormal[iVecIdx];

			for (int iNavIdx = 0; iNavIdx < 3; ++iNavIdx)
			{
				pNavCell = m_vecNavCell[uiCurrentIdx]->Get_Neighbor((CNavCell::NEIGHBOR)iNavIdx);

				if (pNavCell == NULL)
					continue;

				const XMFLOAT3* vNormalOther = pNavCell->Get_Normal(CNavCell::LINE_AB);
				const XMFLOAT3* vPointOther = pNavCell->Get_Point(CNavCell::POINT_A);

				XMVECTOR vDotResultOther;

				for (int iVecIdxOther = 0; iVecIdxOther < 3; ++iVecIdxOther)
				{
					XMVECTOR vDestDirOther = XMLoadFloat3(pPos) - XMLoadFloat3(&vPointOther[iVecIdxOther]);
					vDotResultOther = XMVector3Dot(vDestDirOther, XMLoadFloat3(&vNormalOther[iVecIdxOther]));

					if (XMVectorGetX(vDotResultOther) > 0.f)
						break;
				}

				if (XMVectorGetX(vDotResultOther) <= 0.f)
				{
					bInNavMesh = TRUE;
					break;
				}
			}

			if (bInNavMesh)
			{
				D3DXVECTOR3 vDXPoint[3];
				vDXPoint[0] = D3DXVECTOR3(pNavCell->Get_Point(CNavCell::POINT_A)->x, pNavCell->Get_Point(CNavCell::POINT_A)->y, pNavCell->Get_Point(CNavCell::POINT_A)->z);
				vDXPoint[1] = D3DXVECTOR3(pNavCell->Get_Point(CNavCell::POINT_B)->x, pNavCell->Get_Point(CNavCell::POINT_B)->y, pNavCell->Get_Point(CNavCell::POINT_B)->z);
				vDXPoint[2] = D3DXVECTOR3(pNavCell->Get_Point(CNavCell::POINT_C)->x, pNavCell->Get_Point(CNavCell::POINT_C)->y, pNavCell->Get_Point(CNavCell::POINT_C)->z);

				FLOAT fU, fV, fDist;

				D3DXVECTOR3 vRayPos;
				memcpy(&vRayPos, pPos, sizeof(D3DXVECTOR3));

				if (D3DXIntersectTri(&vDXPoint[0], &vDXPoint[1], &vDXPoint[2], &vRayPos, &D3DXVECTOR3(0.f, 1.f, 0.f), &fU, &fV, &fDist))
				{
					D3DXVECTOR3 vResult = vDXPoint[0] + (vDXPoint[1] - vDXPoint[0]) * fU + (vDXPoint[2] - vDXPoint[0]) * fV;
					if (pResultY != NULL) (*pResultY) = vResult.y;
				}

				else if (D3DXIntersectTri(&vDXPoint[0], &vDXPoint[1], &vDXPoint[2], &vRayPos, &D3DXVECTOR3(0.f, -1.f, 0.f), &fU, &fV, &fDist))
				{
					D3DXVECTOR3 vResult = vDXPoint[0] + (vDXPoint[1] - vDXPoint[0]) * fU + (vDXPoint[2] - vDXPoint[0]) * fV;
					if (pResultY != NULL) (*pResultY) = vResult.y;
				}

				uiCurrentIdx = pNavCell->Get_Index();
				m_uiCurIndex = uiCurrentIdx;
				return TRUE;
			}

			return FALSE;
		}
	}

	D3DXVECTOR3 vDXPoint[3];
	vDXPoint[0] = D3DXVECTOR3(m_vecNavCell[uiCurrentIdx]->Get_Point(CNavCell::POINT_A)->x, m_vecNavCell[uiCurrentIdx]->Get_Point(CNavCell::POINT_A)->y, m_vecNavCell[uiCurrentIdx]->Get_Point(CNavCell::POINT_A)->z);
	vDXPoint[1] = D3DXVECTOR3(m_vecNavCell[uiCurrentIdx]->Get_Point(CNavCell::POINT_B)->x, m_vecNavCell[uiCurrentIdx]->Get_Point(CNavCell::POINT_B)->y, m_vecNavCell[uiCurrentIdx]->Get_Point(CNavCell::POINT_B)->z);
	vDXPoint[2] = D3DXVECTOR3(m_vecNavCell[uiCurrentIdx]->Get_Point(CNavCell::POINT_C)->x, m_vecNavCell[uiCurrentIdx]->Get_Point(CNavCell::POINT_C)->y, m_vecNavCell[uiCurrentIdx]->Get_Point(CNavCell::POINT_C)->z);

	FLOAT fU, fV, fDist;

	D3DXVECTOR3 vRayPos;
	memcpy(&vRayPos, pPos, sizeof(D3DXVECTOR3));

	if (D3DXIntersectTri(&vDXPoint[0], &vDXPoint[1], &vDXPoint[2], &vRayPos, &D3DXVECTOR3(0.f, 1.f, 0.f), &fU, &fV, &fDist))
	{
		D3DXVECTOR3 vResult = vDXPoint[0] + (vDXPoint[1] - vDXPoint[0]) * fU + (vDXPoint[2] - vDXPoint[0]) * fV;
		if (pResultY != NULL) (*pResultY) = vResult.y;
	}

	else if (D3DXIntersectTri(&vDXPoint[0], &vDXPoint[1], &vDXPoint[2], &vRayPos, &D3DXVECTOR3(0.f, -1.f, 0.f), &fU, &fV, &fDist))
	{
		D3DXVECTOR3 vResult = vDXPoint[0] + (vDXPoint[1] - vDXPoint[0]) * fU + (vDXPoint[2] - vDXPoint[0]) * fV;
		if (pResultY != NULL) (*pResultY) = vResult.y;
	}

	return TRUE;
}

void CNavMesh::Render_NavMesh(void)
{
	size_t iSize = m_vecNavCell.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (i == m_uiCurIndex)
			m_vecNavCell[i]->Render(TRUE);

		else
			m_vecNavCell[i]->Render(FALSE);
	}
}

void CNavMesh::Release(void)
{
	for_each(m_vecNavCell.begin(), m_vecNavCell.end(), CRelease_Obj());

	vector<CNavCell*> vecTemp;
	m_vecNavCell = vecTemp;

	delete this;
}