#ifndef GameObject_h__
#define GameObject_h__

#include "Define.h"

class CComponent;
class CGameObject
{
protected:
	explicit CGameObject(ID3D11DeviceContext* pContext);
	virtual ~CGameObject(void);

public:
	const CComponent* Get_Component(const TCHAR* pComponentTag);

public:
	virtual HRESULT	Initialize(void);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT Ready_Component(void);

public:
	virtual void Release(void);

protected:
	ID3D11DeviceContext* m_pContext;

protected:
	using MAPCOMPONENT = map<const TCHAR*, CComponent*>;
	MAPCOMPONENT m_mapComponent;
};

#endif // GameObject_h__