#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

class CDynamicCamera
	: public CCamera
{
private:
	explicit CDynamicCamera(ID3D11DeviceContext* pContext);
	virtual ~CDynamicCamera(void);

public:
	static CDynamicCamera* Create(ID3D11DeviceContext* pContext, FLOAT fNear, FLOAT fFar, XMFLOAT3& vPos, XMFLOAT3& vTarget);

public:
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Release(void);

private:
	FLOAT	m_fSpeed;
	FLOAT	m_fCurAngleX;
};

#endif // DynamicCamera_h__