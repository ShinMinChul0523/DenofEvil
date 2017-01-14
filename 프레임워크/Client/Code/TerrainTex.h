#pragma once

#include "Resources.h"

class CTerrainTex
	: public CResource
{
private:
	explicit CTerrainTex(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CTerrainTex(void);

public:
	static CTerrainTex* Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
		, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv);

private:
	CTerrainTex* Clone_Resource(void);
	HRESULT Create_Buffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv);

public:
	void Render(void);
	void Release(void);

private:
	ID3D11Buffer*			m_pVB;
	ID3D11Buffer*			m_pIB;
	UINT					m_uiVtxCnt;
	UINT					m_uiIdxCnt;

private:
	VTXTEX*					m_pVertex;
};