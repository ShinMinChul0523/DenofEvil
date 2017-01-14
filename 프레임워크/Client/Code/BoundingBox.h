#ifndef BoundingBox_h__
#define BoundingBox_h__

#include "GameObject.h"

class CTransform;
class CCubeTex;
class CBoundingBox
	: public CGameObject
{
private:
	explicit CBoundingBox(ID3D11DeviceContext* pContext);
	virtual ~CBoundingBox(void);

public:
	static CBoundingBox* Create(ID3D11DeviceContext* pContext, const XMFLOAT3& vMin, const XMFLOAT3& vMax
		, const XMFLOAT4X4& matParent);

public:
	virtual HRESULT	Initialize(const XMFLOAT3& vMin, const XMFLOAT3& vMax, const XMFLOAT4X4& matParent);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Render(void);
	virtual void Release(void);

private:
	virtual HRESULT Ready_Component(void);

private:
	CTransform*	m_pTransform;
	CCubeTex*	m_pBuffer;
	XMFLOAT4X4	m_matParent;
};

#endif // BoundingBox_h__