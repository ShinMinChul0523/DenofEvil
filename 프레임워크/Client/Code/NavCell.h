#pragma once

#include "Define.h"

class CTexture;
class CNavCell
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };

private:
	explicit CNavCell(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CNavCell(void);

public:
	const XMFLOAT3* Get_Point(POINT PointFlag);
	const XMFLOAT3* Get_Normal(LINE LineFlag);
	CNavCell* Get_Neighbor(NEIGHBOR NeighborFlag);
	const UINT Get_Index(void);

public:
	void Set_Neighbor(NEIGHBOR eNeighbor, CNavCell* pNeighbor);

private:
	HRESULT Init_Cell(const UINT& uiIdx, const XMFLOAT3* pPointA, const XMFLOAT3* pPointB, const XMFLOAT3* pPointC);

public:
	BOOL Compare_Point(const XMFLOAT3* pPointA, const XMFLOAT3* pPointB, CNavCell* pNeighbor);
	void Render(BOOL bColCell = FALSE);

public:
	static CNavCell* Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext,
		const XMFLOAT3* pPointA, const XMFLOAT3* pPointB, const XMFLOAT3* pPointC, const UINT& uiIdx);

public:
	virtual void Release(void);

private:
	ID3D11Device*			m_pGraphicDev;
	ID3D11DeviceContext*	m_pContext;
	CNavCell*				m_pNeighbor[NEIGHBOR_END];

private:
	XMFLOAT3				m_vPoint[POINT_END];
	XMFLOAT3				m_vNormal[LINE_END];
	UINT					m_uiIndex;

private:
	CTexture*				m_pTexture;
	ID3D11Buffer*			m_pVB;
};