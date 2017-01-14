#ifndef Equipment_h__
#define Equipment_h__

#include "GameObject.h"

class CTransform;
class CEquipment
	: public CGameObject
{
protected:
	explicit CEquipment(ID3D11DeviceContext* pContext);
	virtual ~CEquipment(void);

public:
	virtual HRESULT	Initialize(void);
	virtual INT Update(const FLOAT& fTimeDelta) PURE;
	virtual void Render(void) PURE;
	virtual void Release(void);

public:
	void SetParent(const XMFLOAT4X4& matParent);

protected:
	virtual HRESULT Ready_Component(void) PURE;

protected:
	CTransform*		m_pTransform;
	XMFLOAT4X4		m_matParent;
};

#endif // Equipment_h__
