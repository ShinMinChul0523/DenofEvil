#pragma once

#include "Define.h"
#include "NavCell.h"

class CNavMesh
{
public:
	DECLARE_SINGLETON(CNavMesh)

private:
	CNavMesh(void);
	~CNavMesh(void);

public:
	void Reserve_CellContainerSize(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const UINT& uiSize);
	HRESULT Add_Cell(const XMFLOAT3* pPointA, const XMFLOAT3* pPointB, const XMFLOAT3* pPointC);
	void Link_Cell(void);
	BOOL MoveOnNavMesh(const XMFLOAT3* pPos, UINT& uiCurrentIdx, FLOAT* pResultY = nullptr, XMFLOAT3* vColDir = nullptr);
	void Render_NavMesh(void);

private:
	void Release(void);

private:
	void Translation_ViewSpace(void);
	void Translation_Local(void);

private:
	ID3D11Device*			m_pGraphicDev;
	ID3D11DeviceContext*	m_pContext;

private:
	vector<CNavCell*>	m_vecNavCell;
	UINT				m_uiReserveSize;
	UINT				m_uiIdxCnt;
	UINT				m_uiCurIndex;
};