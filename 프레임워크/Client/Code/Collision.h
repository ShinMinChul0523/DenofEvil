#pragma once

#include "Define.h"

class CBoundingBox;
class CGameObject;
class CCollision
{
public:
	DECLARE_SINGLETON(CCollision)

private:
	CCollision(void);
	~CCollision(void);

private:
	typedef struct tagOBB
	{
		XMFLOAT3 vPoint[8];
		XMFLOAT3 vCenter;
		XMFLOAT3 vProjAxis[3];
		XMFLOAT3 vAxis[3];
	}OBB;

	typedef struct tagOBBObj
	{
		XMFLOAT3			vMin;
		XMFLOAT3			vMax;
		XMFLOAT4X4			matWorld;
		const VTXBONE*		pVertex;
		const UINT*			pIndex;
		UINT				pIdxCnt;
		const CGameObject*	pObj;
	}OBBOBJ;

public:
	void Set_TerrainVtxInCollision(const VTXTEX* pTerrainVtx);
	void Set_WeaponInCollision(const XMFLOAT4X4* pWeaponWorld);
	
public:
	void Reset_ObbCheckList(void);
	void Add_ObbCheckList(const XMFLOAT3& vMin, const XMFLOAT3& vMax, const XMFLOAT4X4& matWorld
		, const VTXBONE* pVertex, const UINT* pIndex, UINT uiIdxCnt, const CGameObject* pGameObject);
	void Update(FLOAT fTimeDelta);

public:
	FLOAT Get_TerrainColY(const XMFLOAT3& vPos);
	XMFLOAT3 Pick_Terrain(const XMFLOAT3& vPos, const XMFLOAT3& vDir);
	BOOL Get_OBB(const XMFLOAT3& vMinL, const XMFLOAT3& vMaxL, const XMFLOAT4X4& matWorldL
		, const XMFLOAT3& vMinR, const XMFLOAT3& vMaxR, const XMFLOAT4X4& matWorldR);

	XMFLOAT3 Get_OBBList(const XMFLOAT3& vMin, const XMFLOAT3& vMax
		, const XMFLOAT3 vPos, const XMFLOAT3 vDir, const XMFLOAT4X4& matWorld);

private:
	void Set_Points(OBB* pOBB, const XMFLOAT3& vMin, const XMFLOAT3& vMax);
	void Set_Axis(OBB* pOBB);

private:
	void Release(void);

public:
	void Set_Device(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);

private:
	ID3D11Device*			m_pGraphicDev;
	ID3D11DeviceContext*	m_pContext;

private:
	const VTXTEX*		m_pTerrainVtx;

private:
	const XMFLOAT4X4*	m_pWeaponWorld;
	list<OBBOBJ*>		m_pObbCheckList;

private:
	list<CGameObject*>	m_DrawList;
};