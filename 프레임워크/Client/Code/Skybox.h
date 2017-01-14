#ifndef Skybox_h__
#define Skybox_h__

#include "GameObject.h"

class CTransform;
class CCubeTex;
class CTexture;
class CSkybox
	: public CGameObject
{
private:
	explicit CSkybox(ID3D11DeviceContext* pContext);
	virtual ~CSkybox(void);

public:
	static CSkybox* Create(ID3D11DeviceContext* pContext);

public:
	virtual HRESULT	Initialize(void);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Render(void);
	virtual void Release(void);

private:
	virtual HRESULT Ready_Component(void);

private:
	CTransform*		m_pTransform;
	CCubeTex*		m_pBuffer;
	CTexture*		m_pTexture;
};

#endif // Skybox_h__