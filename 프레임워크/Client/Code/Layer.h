#ifndef Layer_h__
#define Layer_h__

#include "Define.h"

class CGameObject;
class CComponent;
class CLayer
{
private:
	explicit CLayer(void);
	virtual ~CLayer(void);

public:
	const CComponent* Get_Component(const TCHAR* pObjectTag, const TCHAR* pComponentTag, DWORD dwObjectIdx = 0);

public:
	HRESULT Ready_Object(const TCHAR* pObjectTag, CGameObject* pGameObject);

public:
	INT Update(const FLOAT& fTimeDelta);
	void Render(void);

public:
	static CLayer* Create(void);
	void Release(void);

private:
	list<CGameObject*>* Find_ObjectList(const TCHAR* pObjectTag);

private:
	using OBJECTLIST = list<CGameObject*>;
	using MAPOBJLIST = map<const TCHAR*, OBJECTLIST>;
	MAPOBJLIST m_mapObjlist;
};

#endif // Layer_h__