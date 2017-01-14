#ifndef Triangle_h__
#define Triangle_h__

#include "GameObject.h"

class CTransform;
class CTriangle
	: public CGameObject
{
private:
	explicit CTriangle(ID3D11DeviceContext* pContext);
	virtual ~CTriangle(void);

public:
	static CTriangle* Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, 
		const XMFLOAT3& vPoint1, const XMFLOAT3& vPoint2, const XMFLOAT3& vPoint3);

public:
	virtual HRESULT	Initialize(ID3D11Device* pGraphicDev, const XMFLOAT3& vPoint1, const XMFLOAT3& vPoint2, const XMFLOAT3& vPoint3);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Render(void);
	virtual void Release(void);

private:
	virtual HRESULT Ready_Component(void);

private:
	ID3D11Buffer*	m_pVB;
	CTransform*		m_pTransform;
};

#endif // Triangle_h__