#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"

class CTexture;
class CMesh
	: public CResource
{
protected:
	explicit CMesh(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CMesh(void);

public:
	void Set_RootMinMax(void);
	const XMFLOAT3* Get_Min(void);
	const XMFLOAT3* Get_Max(void);

public:
	void Reserve_ChildSize(const WORD& wSize);
	void Add_Child(CMesh* pChild);
	void Clear_NullChild(void);
	BOOL Check_Remove(void);

public:
	virtual CResource* Clone_Resource(void) PURE;
	virtual void Render(BOOL bColliderDraw = FALSE) PURE;
	virtual void RenderInst(const vector<XMFLOAT4X4*>& vecObjWorld) PURE;
	virtual void Release(void);

protected:
	virtual HRESULT Set_BoundingBox(void)	PURE;

protected:
	vector<CMesh*> m_vecChild;

protected:
	CTexture*		m_pTexture;
	ID3D11Buffer*	m_pVB;
	ID3D11Buffer*	m_pIB;
	UINT			m_uiVtxCnt;
	UINT			m_uiIdxCnt;

protected:
	XMFLOAT3		m_vMin;
	XMFLOAT3		m_vMax;
	ID3D11Buffer*	m_pBBoxVB;
	ID3D11Buffer*	m_pBBoxIB;
};

#endif // Mesh_h__