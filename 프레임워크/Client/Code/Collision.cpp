#include "stdafx.h"
#include "Collision.h"
#include "Export_Resource.h"
#include "Export_Utility.h"
#include "BoundingBox.h"
#include "Monster.h"

IMPLEMENT_SINGLETON(CCollision)

CCollision::CCollision(void)
: m_pTerrainVtx(nullptr)
, m_pWeaponWorld(nullptr)
{
}

CCollision::~CCollision(void)
{
}

void CCollision::Update(FLOAT fTimeDelta)
{
	// Init
	//for (list<CGameObject*>::iterator iter = m_DrawList.begin(); iter != m_DrawList.end(); ++iter)
	//	Safe_Release(*iter);

	//m_DrawList.clear();

	// Check OBB
	//list<OBBOBJ*>::iterator iter_end = m_pObbCheckList.end();

	//if (m_pWeaponWorld != nullptr)
	//{
	//	for (list<OBBOBJ*>::iterator iter = m_pObbCheckList.begin(); iter != iter_end; ++iter)
	//	{
	//		if (Get_OBB(*Get_MeshMin(MESHNUM_BLADE), *Get_MeshMax(MESHNUM_BLADE), *m_pWeaponWorld, (*iter)->vMin, (*iter)->vMax, (*iter)->matWorld))
	//		{
	//			m_DrawList.push_back(CBoundingBox::Create(m_pContext, (*iter)->vMin, (*iter)->vMax, (*iter)->matWorld));
	//			//((CMonster*)(*iter)->pObj)->Attacked();
	//		}
	//	}
	//}

	// Box Render
	//for (list<CGameObject*>::iterator iter = m_DrawList.begin(); iter != m_DrawList.end(); ++iter)
	//	(*iter)->Update(0.f);

	//Reset_ObbCheckList();

	//m_pWeaponWorld = nullptr;
}

FLOAT CCollision::Get_TerrainColY(const XMFLOAT3& vPos)
{
	if (m_pTerrainVtx == nullptr)
		return 0.f;

	UINT uiIndex = ((UINT)(vPos.x / 2.f)) + (((UINT)(vPos.z / 2.f)) * MAPCX);

	if (uiIndex > MAPCX * MAPCZ || uiIndex < 0)
		return 0.f;

	FLOAT fRatioX = vPos.x - m_pTerrainVtx[uiIndex + MAPCX].vPos.x;
	FLOAT fRatioZ = m_pTerrainVtx[uiIndex + MAPCX].vPos.z - vPos.z;

	D3DXPLANE Plane;
	D3DXVECTOR3 vTerrainTexPos[4];

	memcpy(vTerrainTexPos[0], &m_pTerrainVtx[uiIndex].vPos, sizeof(D3DXVECTOR3));
	memcpy(vTerrainTexPos[1], &m_pTerrainVtx[uiIndex + 1].vPos, sizeof(D3DXVECTOR3));
	memcpy(vTerrainTexPos[2], &m_pTerrainVtx[uiIndex + MAPCX].vPos, sizeof(D3DXVECTOR3));
	memcpy(vTerrainTexPos[3], &m_pTerrainVtx[uiIndex + MAPCX + 1].vPos, sizeof(D3DXVECTOR3));

	if (fRatioX > fRatioZ)
		D3DXPlaneFromPoints(&Plane, &vTerrainTexPos[2], &vTerrainTexPos[3], &vTerrainTexPos[1]);	// Right Up

	else
		D3DXPlaneFromPoints(&Plane, &vTerrainTexPos[2], &vTerrainTexPos[1], &vTerrainTexPos[0]);	// Left Down

	return (-Plane.a * vPos.x - Plane.c * vPos.z - Plane.d) / Plane.b + 0.01f;		// Pivot 0.01f
}

XMFLOAT3 CCollision::Pick_Terrain(const XMFLOAT3& vPos, const XMFLOAT3& vDir)
{
	if (m_pTerrainVtx == nullptr)
		return vPos;

	UINT uiIndex = ((UINT)(vPos.x / 2.f)) + (((UINT)(vPos.z / 2.f)) * MAPCX);

	if (uiIndex > MAPCX * MAPCZ || uiIndex < 0)
		return vPos;

	FLOAT fU, fV, fDist, fMinDist = 1000.f;
	D3DXVECTOR3 vTerrainTexPos[4];
	D3DXVECTOR3 vPosDX, vDirDX;

	memcpy(&vPosDX, &vPos, sizeof(D3DXVECTOR3));
	memcpy(&vDirDX, &vDir, sizeof(D3DXVECTOR3));

	D3DXVECTOR3 vMinResult, vResult;
	memcpy(&vMinResult, &vPos, sizeof(D3DXVECTOR3));
	memcpy(&vResult, &vPos, sizeof(D3DXVECTOR3));

	for (INT iX = INT(-10 * abs(vDir.x)) - 1; iX <= INT(10 * abs(vDir.x)) + 1; ++iX)
	{
		for (INT iZ = INT(-10 * abs(vDir.z)) - 1; iZ <= INT(10 * abs(vDir.z)) + 1; ++iZ)
		{
			uiIndex = (((UINT)(vPos.x / 2.f)) + iX) + ((((UINT)(vPos.z / 2.f)) + iZ) * MAPCX);

			if (uiIndex > (MAPCX * MAPCZ) - 1.f || uiIndex < 0)
				continue;

			memcpy(vTerrainTexPos[0], &m_pTerrainVtx[uiIndex].vPos, sizeof(D3DXVECTOR3));
			memcpy(vTerrainTexPos[1], &m_pTerrainVtx[uiIndex + 1].vPos, sizeof(D3DXVECTOR3));
			memcpy(vTerrainTexPos[2], &m_pTerrainVtx[uiIndex + MAPCX].vPos, sizeof(D3DXVECTOR3));
			memcpy(vTerrainTexPos[3], &m_pTerrainVtx[uiIndex + MAPCX + 1].vPos, sizeof(D3DXVECTOR3));

			if (D3DXIntersectTri(&vTerrainTexPos[3], &vTerrainTexPos[2], &vTerrainTexPos[1], &vPosDX, &vDirDX, &fU, &fV, &fDist))
			{
				vResult = vTerrainTexPos[3] + (vTerrainTexPos[2] - vTerrainTexPos[3]) * fU + (vTerrainTexPos[1] - vTerrainTexPos[3]) * fV;
				fDist = D3DXVec3Length(&(D3DXVECTOR3(vResult.x, vPosDX.y, vResult.z) - vPosDX));

				if (fMinDist > fDist)
				{
					fMinDist = fDist;
					vMinResult = vResult;
				}				
			}

			if (D3DXIntersectTri(&vTerrainTexPos[0], &vTerrainTexPos[1], &vTerrainTexPos[2], &vPosDX, &vDirDX, &fU, &fV, &fDist))
			{
				vResult = vTerrainTexPos[0] + (vTerrainTexPos[1] - vTerrainTexPos[0]) * fU + (vTerrainTexPos[2] - vTerrainTexPos[0]) * fV;
				fDist = D3DXVec3Length(&(D3DXVECTOR3(vResult.x, vPosDX.y, vResult.z) - vPosDX));

				if (fMinDist > fDist)
				{
					fMinDist = fDist;
					vMinResult = vResult;
				}			
			}
		}
	}

	return XMFLOAT3(vMinResult.x, vMinResult.y, vMinResult.z);
}

BOOL CCollision::Get_OBB(const XMFLOAT3& vMinL, const XMFLOAT3& vMaxL, const XMFLOAT4X4& matWorldL
	, const XMFLOAT3& vMinR, const XMFLOAT3& vMaxR, const XMFLOAT4X4& matWorldR)
{
	OBB tOBB[2];
	ZeroMemory(tOBB, sizeof(OBB) * 2);

	Set_Points(&tOBB[0], vMinL, vMaxL);
	Set_Points(&tOBB[1], vMinR, vMaxR);

	for (INT i = 0; i < 8; ++i)
	{
		XMStoreFloat3(&tOBB[0].vPoint[i], XMVector3TransformCoord(XMLoadFloat3(&tOBB[0].vPoint[i]), XMLoadFloat4x4(&matWorldL)));
		XMStoreFloat3(&tOBB[1].vPoint[i], XMVector3TransformCoord(XMLoadFloat3(&tOBB[1].vPoint[i]), XMLoadFloat4x4(&matWorldR)));
	}

	XMStoreFloat3(&tOBB[0].vCenter, XMVector3TransformCoord(XMLoadFloat3(&tOBB[0].vCenter), XMLoadFloat4x4(&matWorldL)));
	XMStoreFloat3(&tOBB[1].vCenter, XMVector3TransformCoord(XMLoadFloat3(&tOBB[1].vCenter), XMLoadFloat4x4(&matWorldR)));

	Set_Axis(&tOBB[0]);
	Set_Axis(&tOBB[1]);

	FLOAT fDistance[3];

	for (INT i = 0; i < 2; ++i)
	{
		for (INT j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&tOBB[0].vProjAxis[0]), XMLoadFloat3(&tOBB[i].vAxis[j]))))
				+ fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&tOBB[0].vProjAxis[1]), XMLoadFloat3(&tOBB[i].vAxis[j]))))
				+ fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&tOBB[0].vProjAxis[2]), XMLoadFloat3(&tOBB[i].vAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&tOBB[1].vProjAxis[0]), XMLoadFloat3(&tOBB[i].vAxis[j]))))
				+ fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&tOBB[1].vProjAxis[1]), XMLoadFloat3(&tOBB[i].vAxis[j]))))
				+ fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&tOBB[1].vProjAxis[2]), XMLoadFloat3(&tOBB[i].vAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&tOBB[0].vCenter) - XMLoadFloat3(&tOBB[1].vCenter), XMLoadFloat3(&tOBB[i].vAxis[j]))));

			if (fDistance[2] > fDistance[0] + fDistance[1])
				return FALSE;
		}
	}

	return TRUE;
}

XMFLOAT3 CCollision::Get_OBBList(const XMFLOAT3& vMin, const XMFLOAT3& vMax
	, const XMFLOAT3 vPos, const XMFLOAT3 vDir, const XMFLOAT4X4& matWorld)
{
	FLOAT fU, fV, fDist, fMinDist = 1.f;
	D3DXVECTOR3 vPosDX, vDirDX;
	D3DXVECTOR3 vResult;

	memcpy(&vPosDX, &vPos, sizeof(D3DXVECTOR3));
	memcpy(&vDirDX, &vDir, sizeof(D3DXVECTOR3));

	list<OBBOBJ*>::iterator iter_end = m_pObbCheckList.end();
	for (list<OBBOBJ*>::iterator iter = m_pObbCheckList.begin(); iter != iter_end; ++iter)
	{
		// OBB Ãæµ¹ ½Ã
		if (Get_OBB(vMin, vMax, matWorld, (*iter)->vMin, (*iter)->vMax, (*iter)->matWorld))
		{
			for (UINT uiIndex = 0; uiIndex < (*iter)->pIdxCnt; uiIndex += 3)
			{
				D3DXVECTOR3 vVertex[3];

				memcpy(vVertex[0], &(*iter)->pVertex[(*iter)->pIndex[uiIndex]].vPos, sizeof(D3DXVECTOR3));
				memcpy(vVertex[1], &(*iter)->pVertex[(*iter)->pIndex[uiIndex + 1]].vPos, sizeof(D3DXVECTOR3));
				memcpy(vVertex[2], &(*iter)->pVertex[(*iter)->pIndex[uiIndex + 2]].vPos, sizeof(D3DXVECTOR3));

				for (int i = 0; i < 3; ++i)
				{
					if (D3DXIntersectTri(&vVertex[0], &vVertex[1], &vVertex[2]
						, &D3DXVECTOR3(vPosDX.x, vPosDX.y + i, vPosDX.z), &vDirDX, &fU, &fV, &fDist))
					{
						if (fDist < fMinDist)
						{
							fMinDist = fDist;
							vResult = vVertex[0] + (vVertex[1] - vVertex[0]) * fU + (vVertex[2] - vVertex[0]) * fV;
							vResult -= vDirDX;
						}
					}

				}
			}
		}
	}

	//D3DXPLANE Plane;
	//D3DXVECTOR3 vVertex[4];

	//memcpy(vVertex[0], &(*iter)->pVertex[(*iter)->pIndex[uiIndex]].vPos, sizeof(D3DXVECTOR3));
	//memcpy(vVertex[1], &(*iter)->pVertex[(*iter)->pIndex[uiIndex + 1]].vPos, sizeof(D3DXVECTOR3));
	//memcpy(vVertex[2], &(*iter)->pVertex[(*iter)->pIndex[uiIndex + 2]].vPos, sizeof(D3DXVECTOR3));

	//D3DXPlaneFromPoints(&Plane, &vVertex[2], &vVertex[1], &vVertex[0]);

	//return (-Plane.a * vPos.x - Plane.c * vPos.z - Plane.d) / Plane.b + 0.01f;		// Pivot 0.01f

	if (fMinDist == 1.f)
		return vPos;

	else
 		return XMFLOAT3(vResult.x, vPos.y, vResult.z);
}

void CCollision::Release(void)
{
	list<OBBOBJ*>::iterator iter_end = m_pObbCheckList.end();
	for (list<OBBOBJ*>::iterator iter = m_pObbCheckList.begin(); iter != iter_end; ++iter)
		Safe_Delete(*iter);

	for (list<CGameObject*>::iterator iter = m_DrawList.begin(); iter != m_DrawList.end(); ++iter)
		Safe_Release(*iter);

	m_DrawList.clear();
	m_pObbCheckList.clear();

	delete this;
}

void CCollision::Set_TerrainVtxInCollision(const VTXTEX* pTerrainVtx)
{
	m_pTerrainVtx = pTerrainVtx;
}

void CCollision::Set_WeaponInCollision(const XMFLOAT4X4* pWeaponWorld)
{
	m_pWeaponWorld = pWeaponWorld;
}

void CCollision::Add_ObbCheckList(const XMFLOAT3& vMin, const XMFLOAT3& vMax, const XMFLOAT4X4& matWorld
	, const VTXBONE* pVertex, const UINT* pIndex, UINT uiIdxCnt, const CGameObject* pGameObject)
{
	OBBOBJ* pObbObj = new OBBOBJ;

	pObbObj->vMin = vMin;
	pObbObj->vMax = vMax;
	pObbObj->matWorld = matWorld;

	pObbObj->pVertex = pVertex;
	pObbObj->pIndex = pIndex;
	pObbObj->pIdxCnt = uiIdxCnt;

	pObbObj->pObj = pGameObject;

	m_pObbCheckList.push_back(pObbObj);
}

void CCollision::Set_Points(OBB* pOBB, const XMFLOAT3& vMin, const XMFLOAT3& vMax)
{
	pOBB->vPoint[0] = XMFLOAT3(vMin.x, vMax.y, vMin.z);
	pOBB->vPoint[1] = XMFLOAT3(vMax.x, vMax.y, vMin.z);
	pOBB->vPoint[2] = XMFLOAT3(vMax.x, vMin.y, vMin.z);
	pOBB->vPoint[3] = XMFLOAT3(vMin.x, vMin.y, vMin.z);

	pOBB->vPoint[4] = XMFLOAT3(vMin.x, vMax.y, vMax.z);
	pOBB->vPoint[5] = XMFLOAT3(vMax.x, vMax.y, vMax.z);
	pOBB->vPoint[6] = XMFLOAT3(vMax.x, vMin.y, vMax.z);
	pOBB->vPoint[7] = XMFLOAT3(vMin.x, vMin.y, vMax.z);

	pOBB->vCenter.x = (vMin.x + vMax.x) * 0.5f;
	pOBB->vCenter.y = (vMin.y + vMax.y) * 0.5f;
	pOBB->vCenter.z = (vMin.z + vMax.z) * 0.5f;
}

void CCollision::Set_Axis(OBB* pOBB)
{
	XMStoreFloat3(&pOBB->vProjAxis[0], (XMLoadFloat3(&pOBB->vPoint[1]) + XMLoadFloat3(&pOBB->vPoint[6])) * 0.5f - XMLoadFloat3(&pOBB->vCenter));
	XMStoreFloat3(&pOBB->vProjAxis[1], (XMLoadFloat3(&pOBB->vPoint[0]) + XMLoadFloat3(&pOBB->vPoint[5])) * 0.5f - XMLoadFloat3(&pOBB->vCenter));
	XMStoreFloat3(&pOBB->vProjAxis[2], (XMLoadFloat3(&pOBB->vPoint[7]) + XMLoadFloat3(&pOBB->vPoint[5])) * 0.5f - XMLoadFloat3(&pOBB->vCenter));

	XMStoreFloat3(&pOBB->vAxis[0], XMLoadFloat3(&pOBB->vPoint[2]) - XMLoadFloat3(&pOBB->vPoint[3]));
	XMStoreFloat3(&pOBB->vAxis[1], XMLoadFloat3(&pOBB->vPoint[0]) - XMLoadFloat3(&pOBB->vPoint[3]));
	XMStoreFloat3(&pOBB->vAxis[2], XMLoadFloat3(&pOBB->vPoint[7]) - XMLoadFloat3(&pOBB->vPoint[3]));

	for (INT i = 0; i < 3; ++i)
		XMStoreFloat3(&pOBB->vAxis[i], XMVector3Normalize(XMLoadFloat3(&pOBB->vAxis[i])));
}

void CCollision::Set_Device(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
{
	m_pGraphicDev = pGraphicDev;
	m_pContext = pContext;
}

void CCollision::Reset_ObbCheckList(void)
{
	// Reset
	list<OBBOBJ*>::iterator iter_end = m_pObbCheckList.end();
	for (list<OBBOBJ*>::iterator iter = m_pObbCheckList.begin(); iter != iter_end; ++iter)
		Safe_Delete(*iter);

	m_pObbCheckList.clear();
}