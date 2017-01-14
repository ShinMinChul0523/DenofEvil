#ifndef Component_h__
#define Component_h__

#include "Define.h"

class CComponent
{
protected:
	explicit CComponent(void);
	virtual ~CComponent(void);

public:
	virtual void Update(FLOAT fTime);
	virtual void Release(void) PURE;
};

#endif // Component_h__