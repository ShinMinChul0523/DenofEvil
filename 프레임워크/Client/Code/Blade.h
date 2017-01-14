#ifndef Blade_h__
#define Blade_h__

#include "Equipment.h"

class CBlade
	: public CEquipment
{
private:
	explicit CBlade(ID3D11DeviceContext* pContext);
	virtual ~CBlade(void);

public:
	static CBlade* Create(ID3D11DeviceContext* pContext);

public:
	const XMFLOAT4X4* Get_WorldPointer(void);

public:
	virtual HRESULT	Initialize(void);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Render(void);
	virtual void Release(void);

private:
	virtual HRESULT Ready_Component(void);

private:
	UINT m_uiObjNum;
};

#endif // Blade_h__
