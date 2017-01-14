#ifndef Gun_h__
#define Gun_h__

#include "Equipment.h"

class CGun
	: public CEquipment
{
private:
	explicit CGun(ID3D11DeviceContext* pContext);
	virtual ~CGun(void);

public:
	static CGun* Create(ID3D11DeviceContext* pContext, BOOL bReverse);

public:
	virtual HRESULT	Initialize(BOOL bReverse);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Render(void);
	virtual void Release(void);

private:
	virtual HRESULT Ready_Component(void);

private:
	UINT m_uiObjNum;
};

#endif // Gun_h__
