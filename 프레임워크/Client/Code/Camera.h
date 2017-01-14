#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

class CCamera
	: public CGameObject
{
protected:
	explicit CCamera(ID3D11DeviceContext* pContext);
	virtual ~CCamera(void);

public:
	const XMFLOAT4X4* Get_Proj(void);
	const XMFLOAT4X4* Get_View(void);

protected:
	void Set_Proj(FLOAT fNear, FLOAT fFar);
	void Set_View(XMFLOAT3 vPos, XMFLOAT3 vTarget);

public:
	virtual HRESULT	Initialize(FLOAT fNear, FLOAT fFar, XMFLOAT3 vPos, XMFLOAT3 vTarget);
	virtual INT Update(const FLOAT& fTimeDelta)	PURE;
	virtual void Release(void);

protected:
	XMFLOAT4X4*				m_pProj;
	XMFLOAT4X4*				m_pView;

protected:
	XMFLOAT3*				m_pPos;
	XMFLOAT3*				m_pTarget;
};

#endif Camera_h__