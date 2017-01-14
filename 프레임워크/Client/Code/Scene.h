#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

class CScene
{
protected:
	explicit CScene(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CScene(void);

public:
	virtual HRESULT Ready_Scene(void)	PURE;
	
public:
	virtual INT Update(const FLOAT &fTimeDelta);
	virtual void Render(void);

public:
	virtual void Release(void);

protected:
	ID3D11Device*			m_pGraphicDev;
	ID3D11DeviceContext*	m_pContext;

protected:
	using MAPLAYER = map<const TCHAR*, CLayer*>;
	MAPLAYER m_mapLayer;
};

#endif // Scene_h__