#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"

class CGameObject;
class CAnimationInfo;
class CAnimation;
class CDynaicMesh
	: public CMesh
{
private:
	explicit CDynaicMesh(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CDynaicMesh(void);

public:
	static CDynaicMesh* Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const VTXBONE* pVB, const UINT& uiVtxCnt
		, const UINT* pIB, const UINT& uiIdxCnt, const XMFLOAT3& vMin, const XMFLOAT3& vMax, TCHAR* pTexName, CAnimation* pAnimation);

public:
	virtual CResource* Clone_Resource(void);
	virtual void Render(BOOL bColliderDraw = FALSE);
	void RenderAnim(CAnimationInfo* pAnimInfo, BOOL bColliderDraw = FALSE);
	virtual void RenderInst(const vector<XMFLOAT4X4*>& vecObjWorld);
	virtual void Release(void);

public:
	XMFLOAT4X4 Get_TransBoneMatrix(INT iIndex);
	void Add_ObbCheckList(const XMFLOAT4X4* pWorld, const CGameObject* pObj);

private:
	HRESULT Create_Buffer(const VTXBONE* pVB, const UINT& uiVtxCnt, const UINT* pIB, const UINT& uiIdxCnt
		, const XMFLOAT3& vMin, const XMFLOAT3& vMax, TCHAR* pTexName, CAnimation* pAnimation);

private:
	virtual HRESULT Set_BoundingBox(void);

private:
	VTXBONE*		m_pOriVertex;
	VTXBONE*		m_pVertex;
	UINT*			m_pIndex;
	XMFLOAT4X4*		m_pBoneWorld;

private:
	CAnimation*		m_pAnimation;
	XMFLOAT3		m_vPivotPos;
};

#endif // DynamicMesh_h__