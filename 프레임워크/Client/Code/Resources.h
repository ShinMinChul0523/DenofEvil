#ifndef Resources_h__
#define Resources_h__

#include "Component.h"

class CResource
	: public CComponent
{
protected:
	explicit CResource(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CResource(void);

public:
	virtual CResource* Clone_Resource(void)	PURE;

public:
	virtual void Release(void);

protected:
	WORD*					m_pwCloneCnt;

protected:
	ID3D11Device*			m_pGraphicDev;
	ID3D11DeviceContext*	m_pContext;
};

#endif // Resources_h__