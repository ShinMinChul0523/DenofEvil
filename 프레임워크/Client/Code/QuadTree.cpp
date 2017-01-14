#include "stdafx.h"
#include "QuadTree.h"
#include "Frustum.h"
#include "StaticCamera.h"

IMPLEMENT_SINGLETON(CQuadTree)

CQuadTree::CQuadTree(void)
	: m_iCenter(0)
	, m_fRadius(0.f)
	, m_isIn(FALSE)
{
	ZeroMemory(m_pChild, sizeof(CQuadTree*) * CHILD_END);
	ZeroMemory(m_iCorner, sizeof(INT) * CORNER_END);
	ZeroMemory(m_pNeighbor, sizeof(CQuadTree*) * NEIGHBOR_END);
}

CQuadTree::~CQuadTree(void)
{
}

HRESULT CQuadTree::Ready_QuadTree(const VTXTEX* pTerrainVtx, const INT& iCntX, const INT& iCntZ)
{
	m_iCorner[CORNER_LT] = 0;
	m_iCorner[CORNER_RT] = iCntX - 1;
	m_iCorner[CORNER_LB] = iCntX * (iCntZ - 1);
	m_iCorner[CORNER_RB] = (iCntX * iCntZ) - 1;

	for (INT i = 0; i < CORNER_END; ++i)
		m_iCenter += m_iCorner[i];

	m_iCenter = m_iCenter >> 2;
	Set_Child(pTerrainVtx);

	return S_OK;
}

HRESULT CQuadTree::Ready_Neighbor(const VTXTEX* pTerrainVtxInfo, CQuadTree* pParentQuad, const INT& iVtxCntX, const INT& iVtxCntZ)
{
	INT iLT, iRT, iLB, iRB;

	for (INT i = 0; i < NEIGHBOR_END; ++i)
	{
		if (Make_Neighbor(NEIGHBOR(i), &iLT, &iRT, &iLB, &iRB, iVtxCntX, iVtxCntZ) == TRUE)
		{
			INT	iCenter = (iLT + iRT + iLB + iRB) >> 2;
			m_pNeighbor[i] = pParentQuad->Find_Neighbor(pTerrainVtxInfo, iLT, iRT, iLB, iRB, iCenter);
		}
		else
			m_pNeighbor[i] = NULL;
	}

	for (int i = 0; i < CHILD_END; ++i)
	{
		if (NULL != m_pChild[i])
			m_pChild[i]->Ready_Neighbor(pTerrainVtxInfo, pParentQuad, iVtxCntX, iVtxCntZ);
	}

	return S_OK;
}

void CQuadTree::Is_inFrustum(const VTXTEX* pTerrainVtx, const CFrustum* pFrustum)
{
	if (m_pChild[CHILD_LT] == NULL)
	{
		m_isIn = TRUE;
		return;
	}

	m_isIn = pFrustum->Sphere_InFrustum(&pTerrainVtx[m_iCenter].vPos, m_fRadius);

	if (m_isIn == TRUE)
	{
		for (INT i = 0; i < CHILD_END; ++i)
		{
			if (m_pChild[i] != NULL)
				m_pChild[i]->Is_inFrustum(pTerrainVtx, pFrustum);
		}
	}
}

void CQuadTree::Cull_ToQuadTree(const VTXTEX* pTerrainVtx, const CFrustum* pFrustum, UINT* pIndex, DWORD* pdwTriCnt)
{
	if (m_isIn == TRUE)
	{
		if (m_pChild[CHILD_LT] == NULL || (LevelofDetail(pTerrainVtx)) == TRUE)
		{
			BOOL isRender[4] = { TRUE, TRUE, TRUE, TRUE };

			for (INT i = 0; i < NEIGHBOR_END; ++i)
			{
				if (NULL != m_pNeighbor[i])
					isRender[i] = m_pNeighbor[i]->LevelofDetail(pTerrainVtx);
			}

			if (isRender[NEIGHBOR_LEFT] && isRender[NEIGHBOR_TOP]
				&& isRender[NEIGHBOR_RIGHT] && isRender[NEIGHBOR_BOTTOM])
			{
				pIndex[((*pdwTriCnt) * 3)]		= m_iCorner[CORNER_LB];
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCorner[CORNER_RB];
				pIndex[((*pdwTriCnt) * 3) + 2]	= m_iCorner[CORNER_RT];
				++(*pdwTriCnt);

				pIndex[((*pdwTriCnt) * 3)]		= m_iCorner[CORNER_LB];
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCorner[CORNER_RT];
				pIndex[((*pdwTriCnt) * 3) + 2]	= m_iCorner[CORNER_LT];
				++(*pdwTriCnt);

				return;
			}

			if (isRender[NEIGHBOR_LEFT] == FALSE)
			{
				INT iLC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_LB]) >> 1;

				pIndex[((*pdwTriCnt) * 3)]		= m_iCorner[CORNER_LB];
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCenter;
				pIndex[((*pdwTriCnt) * 3) + 2]	= iLC;
				++(*pdwTriCnt);	

				pIndex[((*pdwTriCnt) * 3)]		= iLC;
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCenter;
				pIndex[((*pdwTriCnt) * 3) + 2]	= m_iCorner[CORNER_LT];
				++(*pdwTriCnt);
			}
			else
			{
				pIndex[((*pdwTriCnt) * 3)]		= m_iCorner[CORNER_LB];
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCenter;
				pIndex[((*pdwTriCnt) * 3) + 2]	= m_iCorner[CORNER_LT];
				++(*pdwTriCnt);
			}

			if (isRender[NEIGHBOR_TOP] == FALSE)
			{
				INT iTC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_RT]) >> 1;

				pIndex[((*pdwTriCnt) * 3)]		= m_iCorner[CORNER_LT];
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCenter;
				pIndex[((*pdwTriCnt) * 3) + 2]	= iTC;
				++(*pdwTriCnt);

				pIndex[((*pdwTriCnt) * 3)]		= iTC;
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCenter;
				pIndex[((*pdwTriCnt) * 3) + 2]	= m_iCorner[CORNER_RT];
				++(*pdwTriCnt);
			}
			else
			{
				pIndex[((*pdwTriCnt) * 3)]		= m_iCorner[CORNER_LT];
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCenter;
				pIndex[((*pdwTriCnt) * 3) + 2]	= m_iCorner[CORNER_RT];
				++(*pdwTriCnt);
			}

			if (isRender[NEIGHBOR_RIGHT] == FALSE)
			{
				INT iRC = (m_iCorner[CORNER_RT] + m_iCorner[CORNER_RB]) >> 1;

				pIndex[((*pdwTriCnt) * 3)]		= m_iCenter;
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCorner[CORNER_RB];
				pIndex[((*pdwTriCnt) * 3) + 2]	= iRC;
				++(*pdwTriCnt);

				pIndex[((*pdwTriCnt) * 3)]		= iRC;
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCorner[CORNER_RT];
				pIndex[((*pdwTriCnt) * 3) + 2]	= m_iCenter;
				++(*pdwTriCnt);
			}
			else
			{
				pIndex[((*pdwTriCnt) * 3)]		= m_iCenter;
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCorner[CORNER_RB];
				pIndex[((*pdwTriCnt) * 3) + 2]	= m_iCorner[CORNER_RT];
				++(*pdwTriCnt);
			}

			if (isRender[NEIGHBOR_BOTTOM] == FALSE)
			{
				INT iBC = (m_iCorner[CORNER_RB] + m_iCorner[CORNER_LB]) >> 1;

				pIndex[((*pdwTriCnt) * 3)]		= m_iCorner[CORNER_LB];
				pIndex[((*pdwTriCnt) * 3) + 1]	= iBC;
				pIndex[((*pdwTriCnt) * 3) + 2]	= m_iCenter;
				++(*pdwTriCnt);

				pIndex[((*pdwTriCnt) * 3)]		= m_iCenter;
				pIndex[((*pdwTriCnt) * 3) + 1]	= iBC;
				pIndex[((*pdwTriCnt) * 3) + 2]	= m_iCorner[CORNER_RB];
				++(*pdwTriCnt);
			}
			else
			{
				pIndex[((*pdwTriCnt) * 3)]		= m_iCenter;
				pIndex[((*pdwTriCnt) * 3) + 1]	= m_iCorner[CORNER_LB];
				pIndex[((*pdwTriCnt) * 3) + 2]	= m_iCorner[CORNER_RB];
				++(*pdwTriCnt);
			}
			return;
		}

		for (INT i = 0; i < CHILD_END; ++i)
		{
			if (m_pChild[i] != NULL)
				m_pChild[i]->Cull_ToQuadTree(pTerrainVtx, pFrustum, pIndex, pdwTriCnt);
		}
	}
}

void CQuadTree::Set_Child(const VTXTEX* pTerrainVtx)
{
	XMVECTOR vTerrainVtx01 = XMLoadFloat3(&(pTerrainVtx[m_iCorner[CORNER_LT]].vPos));
	XMVECTOR vTerrainVtx02 = XMLoadFloat3(&(pTerrainVtx[m_iCenter].vPos));

	XMVECTOR vLength = XMVector3Length(vTerrainVtx01 - vTerrainVtx02);

	XMFLOAT3 vConvertLength;
	XMStoreFloat3(&vConvertLength, vLength);

	m_fRadius = vConvertLength.x;

	for (int i = 0; i < CHILD_END; ++i)
	{
		m_pChild[i] = Make_Child(CHILD(i));

		if (m_pChild[i] != NULL)
			m_pChild[i]->Set_Child(pTerrainVtx);
	}
}

CQuadTree* CQuadTree::Make_Child(CHILD eChild)
{
	if (m_iCorner[CORNER_RT] - m_iCorner[CORNER_LT] == 1)
	{
		m_fRadius = 0.f;
		return NULL;
	}

	INT iLC, iTC, iRC, iBC;
	iLC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_LB]) >> 1;
	iTC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_RT]) >> 1;
	iRC = (m_iCorner[CORNER_RT] + m_iCorner[CORNER_RB]) >> 1;
	iBC = (m_iCorner[CORNER_LB] + m_iCorner[CORNER_RB]) >> 1;

	CQuadTree* pQuadTree = new CQuadTree;

	switch (eChild)
	{
	case CHILD_LT:
		pQuadTree->m_iCorner[CORNER_LT] = m_iCorner[CORNER_LT];
		pQuadTree->m_iCorner[CORNER_RT] = iTC;
		pQuadTree->m_iCorner[CORNER_LB] = iLC;
		pQuadTree->m_iCorner[CORNER_RB] = m_iCenter;
		break;

	case CHILD_RT:
		pQuadTree->m_iCorner[CORNER_LT] = iTC;
		pQuadTree->m_iCorner[CORNER_RT] = m_iCorner[CORNER_RT];
		pQuadTree->m_iCorner[CORNER_LB] = m_iCenter;
		pQuadTree->m_iCorner[CORNER_RB] = iRC;
		break;

	case CHILD_LB:
		pQuadTree->m_iCorner[CORNER_LT] = iLC;
		pQuadTree->m_iCorner[CORNER_RT] = m_iCenter;
		pQuadTree->m_iCorner[CORNER_LB] = m_iCorner[CORNER_LB];
		pQuadTree->m_iCorner[CORNER_RB] = iBC;
		break;

	case CHILD_RB:
		pQuadTree->m_iCorner[CORNER_LT] = m_iCenter;
		pQuadTree->m_iCorner[CORNER_RT] = iRC;
		pQuadTree->m_iCorner[CORNER_LB] = iBC;
		pQuadTree->m_iCorner[CORNER_RB] = m_iCorner[CORNER_RB];
		break;
	}

	for (INT i = 0; i < CORNER_END; ++i)
		pQuadTree->m_iCenter += pQuadTree->m_iCorner[i];

	pQuadTree->m_iCenter = pQuadTree->m_iCenter >> 2;

	return pQuadTree;
}

BOOL CQuadTree::LevelofDetail(const VTXTEX* pTerrainVtx)
{
	XMVECTOR vDeterminant;

	XMMATRIX matView = XMLoadFloat4x4(CStaticCamera::Get_View());
	matView = XMMatrixInverse(&vDeterminant, matView);

	XMFLOAT4X4 matConvertView;
	XMStoreFloat4x4(&matConvertView, matView);

	XMFLOAT3 camera = XMFLOAT3(matConvertView._41, matConvertView._42, matConvertView._43);
	XMVECTOR vCamPos = XMLoadFloat3(&(XMFLOAT3(matConvertView._41, matConvertView._42, matConvertView._43)));
	XMVECTOR vTerainVtxPos = XMLoadFloat3(&(pTerrainVtx[m_iCenter].vPos));

	XMVECTOR vLength = XMVector3Length(vCamPos - vTerainVtxPos);

	XMFLOAT3 vConvertLength;
	XMStoreFloat3(&vConvertLength, vLength);

	FLOAT fDistance = vConvertLength.x;
	FLOAT fWidth = pTerrainVtx[m_iCorner[CORNER_RT]].vPos.x - pTerrainVtx[m_iCorner[CORNER_LT]].vPos.x;

	if (fDistance * 0.1f > fWidth)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CQuadTree::Make_Neighbor(NEIGHBOR eNeighbor, INT* pLT, INT* pRT, INT* pLB, INT* pRB, const INT& iVtxCntX, const INT& iVtxCntZ)
{
	INT iSizeX = m_iCorner[CORNER_RT] - m_iCorner[CORNER_LT];
	INT iSizeY = m_iCorner[CORNER_LB] - m_iCorner[CORNER_LT];

	switch (eNeighbor)
	{
	case NEIGHBOR_LEFT:
		*pLT = m_iCorner[CORNER_LT] - iSizeX;
		*pRT = m_iCorner[CORNER_LT];
		*pLB = m_iCorner[CORNER_LB] - iSizeX;
		*pRB = m_iCorner[CORNER_LB];
		if (m_iCorner[CORNER_LT] % iVtxCntX == 0)
			return FALSE;
		break;

	case NEIGHBOR_TOP:
		*pLT = m_iCorner[CORNER_LT] - iSizeY;
		*pRT = m_iCorner[CORNER_RT] - iSizeY;
		*pLB = m_iCorner[CORNER_LT];
		*pRB = m_iCorner[CORNER_RT];
		if (m_iCorner[CORNER_RT] < iVtxCntX)
			return FALSE;
		break;

	case NEIGHBOR_RIGHT:
		*pLT = m_iCorner[CORNER_RT];
		*pRT = m_iCorner[CORNER_RT] + iSizeX;
		*pLB = m_iCorner[CORNER_RB];
		*pRB = m_iCorner[CORNER_RB] + iSizeX;
		if (m_iCorner[CORNER_RT] % iVtxCntX == iVtxCntX - 1)
			return FALSE;
		break;

	case NEIGHBOR_BOTTOM:
		*pLT = m_iCorner[CORNER_LB];
		*pRT = m_iCorner[CORNER_RB];
		*pLB = m_iCorner[CORNER_LB] + iSizeY;
		*pRB = m_iCorner[CORNER_RB] + iSizeY;
		if (m_iCorner[CORNER_LB] >= iVtxCntX * (iVtxCntZ - 1))
			return FALSE;
		break;
	}

	return TRUE;
}

CQuadTree* CQuadTree::Find_Neighbor(const VTXTEX* pTerrainVtxInfo, const INT& iLT, const INT& iRT, const INT& iLB, const INT& iRB, const INT& iCenter)
{
	if (m_iCorner[CORNER_LT] == iLT
		&& m_iCorner[CORNER_RT] == iRT
		&& m_iCorner[CORNER_LB] == iLB
		&& m_iCorner[CORNER_RB] == iRB)
		return this;

	RECT	rcQuad;
	POINT	ptCenter;

	for (INT i = 0; i < CHILD_END; ++i)
	{
		if (NULL != m_pChild[i])
		{
			ZeroMemory(&rcQuad, sizeof(RECT));
			ZeroMemory(&ptCenter, sizeof(POINT));

			SetRect(&rcQuad, INT(pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_LT]].vPos.x)
				, INT(pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_LT]].vPos.z)
				, INT(pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_RB]].vPos.x)
				, INT(pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_RB]].vPos.z));

			ptCenter.x = LONG(pTerrainVtxInfo[iCenter].vPos.x);
			ptCenter.y = LONG(pTerrainVtxInfo[iCenter].vPos.z);

			if (PtInRect(&rcQuad, ptCenter))
				return m_pChild[i]->Find_Neighbor(pTerrainVtxInfo, iLT, iRT, iLB, iRB, iCenter);
		}
	}
	return NULL;
}

void CQuadTree::Release(void)
{
	for (INT i = 0; i < CHILD_END; ++i)
	{
		Safe_Release(m_pChild[i]);
	}

	delete this;
}
