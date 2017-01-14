#ifndef Monster_h__
#define Monster_h__

#include "GameObject.h"

class CAnimationInfo;
class CTransform;
class CMonster
	: public CGameObject
{
protected:
	explicit CMonster(ID3D11DeviceContext* pContext);
	virtual ~CMonster(void);

public:
	virtual HRESULT	Initialize(void) PURE;
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Render(void) PURE;
	virtual void Release(void);

public:
	virtual void Attacked(void) PURE;

protected:
	virtual HRESULT Ready_Component(void);

protected:
	CAnimationInfo* m_pAnimInfo;
	CTransform*		m_pTransform;
	UINT			m_uiObjNum;
	FLOAT			m_fSpeed;
	FLOAT			m_fHeight;
};

#endif // Golem_h__
