#ifndef StageCamera_h__
#define StageCamera_h__

#include "Camera.h"

class CTransform;
class CStaticCamera
	: public CCamera
{
private:
	explicit CStaticCamera(ID3D11DeviceContext* pContext, const CTransform* pObjTrans, FLOAT fDist, FLOAT fHeightPivot);
	virtual ~CStaticCamera(void);

public:
	static CStaticCamera* Create(ID3D11DeviceContext* pContext, const CTransform* pObjTrans, FLOAT fDist, FLOAT fHeightPivot
		, FLOAT fNear, FLOAT fFar, XMFLOAT3& vPos, XMFLOAT3& vTarget);

public:
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Release(void);

private:
	const CTransform*	m_pObjTrans;
	FLOAT				m_fDist;
	FLOAT				m_fHeightPivot;
	XMVECTOR			m_vCamaraDir;
	bool				m_bCreate;

};

#endif StageCamera_h__