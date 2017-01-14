#ifndef QuadTree_h__
#define QuadTree_h__

#include "Define.h"

class CFrustum;
class CQuadTree
{
	DECLARE_SINGLETON(CQuadTree)

public:
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_LB, CHILD_RB, CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB, CORNER_END };
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END };

private:
	explicit CQuadTree(void);

public:
	~CQuadTree(void);

public:
	HRESULT Ready_Neighbor(const VTXTEX* pTerrainVtxInfo, CQuadTree* pParentQuad, const INT& iVtxCntX, const INT& iVtxCntZ);
	HRESULT Ready_QuadTree(const VTXTEX* pTerrainVtx, const INT& iCntX, const INT& iCntZ);

public:
	void Is_inFrustum(const VTXTEX* pTerrainVtx, const CFrustum* pFrustum);
	void Cull_ToQuadTree(const VTXTEX* pTerrainVtx, const CFrustum* pFrustum, UINT* pIndex, DWORD* pdwTriCnt);

public:
	void Set_Child(const VTXTEX* pTerrainVtx);
	CQuadTree* Make_Child(CHILD eChild);
	BOOL LevelofDetail(const VTXTEX* pTerrainVtx);
	void Release(void);

private:
	BOOL Make_Neighbor(NEIGHBOR eNeighbor, INT* pLT, INT* pRT, INT* pLB, INT* pRB
		, const INT& iVtxCntX, const INT& iVtxCntZ);
	CQuadTree* Find_Neighbor(const VTXTEX* pTerrainVtxInfo
		, const INT& iLT, const INT& iRT, const INT& iLB, const INT& iRB, const INT& iCenter);

private:
	CQuadTree*		m_pChild[CHILD_END];
	CQuadTree*		m_pNeighbor[NEIGHBOR_END];
	INT				m_iCorner[CORNER_END];
	INT				m_iCenter;
	FLOAT			m_fRadius;
	BOOL			m_isIn;
};

#endif // QuadTree_h__