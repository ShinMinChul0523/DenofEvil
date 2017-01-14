#ifndef Terrain_h__
#define Terrain_h__

#include "GameObject.h"

class CTerrainTex;
class CTexture;
class CTerrain
	: public CGameObject
{
private:
	explicit CTerrain(ID3D11DeviceContext* pContext);
	virtual ~CTerrain(void);

public:
	static CTerrain* Create(ID3D11DeviceContext* pContext);

public:
	virtual HRESULT	Initialize(void);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual void Render(void);
	virtual void Release(void);

private:
	virtual HRESULT Ready_Component(void);

private:
	CTerrainTex*	m_pBuffer;
	CTexture*		m_pTexture;
};

#endif // Terrain_h__