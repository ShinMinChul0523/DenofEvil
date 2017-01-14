#ifndef Golem_h__
#define Golem_h__

#include "Monster.h"

class CAnimationInfo;
class CTransform;
class CGolem
	: public CMonster
{
private:
	explicit CGolem(ID3D11DeviceContext* pContext);
	virtual ~CGolem(void);

public:
	static CGolem* Create(ID3D11DeviceContext* pContext);

public:
	virtual HRESULT	Initialize(void);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Render(void);
	virtual void Release(void);

public:
	virtual void Attacked(void);

private:
	virtual HRESULT Ready_Component(void);
};

#endif // Golem_h__
