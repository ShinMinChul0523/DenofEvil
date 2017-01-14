#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

class CStaticMesh
	: public CMesh
{
private:
	explicit CStaticMesh(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CStaticMesh(void);

public:
	static CStaticMesh* Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const VTXTEX* pVB, const UINT& uiVtxCnt
		, const UINT* pIB, const UINT& uiIdxCnt, const XMFLOAT3& vMin, const XMFLOAT3& vMax, TCHAR* pTexName);

public:
	virtual CResource* Clone_Resource(void);
	virtual void Render(BOOL bColliderDraw = FALSE);
	virtual void RenderInst(const vector<XMFLOAT4X4*>& vecObjWorld);
	virtual void Release(void);

private:
	HRESULT Create_Buffer(const VTXTEX* pVB, const UINT& uiVtxCnt, const UINT* pIB, const UINT& uiIdxCnt
		, const XMFLOAT3& vMin, const XMFLOAT3& vMax, TCHAR* pTexName);

private:
	virtual HRESULT Set_BoundingBox(void);
};

#endif // StaticMesh_h__