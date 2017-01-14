#ifndef Frustum_h__
#define Frustum_h__

#include "Define.h"

class CFrustum
{
	DECLARE_SINGLETON(CFrustum)

private:
	explicit CFrustum(void);
	~CFrustum(void);

public:
	HRESULT Ready_Frustum(void);
	void Transform_Frustum(void);
	void Release(void);

public:
	BOOL Sphere_InFrustum(const XMFLOAT3* pCenter, const FLOAT& fRadius) const;
	BOOL Vertex_InFrustum(const XMFLOAT3* pPoint) const;

private:
	D3DXVECTOR3		m_vOriPoint[8];
	D3DXVECTOR3		m_vConPoint[8];

	D3DXPLANE		m_Plane[6];
};

#endif // Frustum_h__