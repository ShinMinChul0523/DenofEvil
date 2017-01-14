#pragma once

#include "Resources.h"

class CTexture
	: public CResource
{
public:
	enum TEXTURETYPE { TYPE_NORMAL, TYPE_TGA, TYPE_DDSCUBE, TYPE_END };

private:
	explicit CTexture(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CTexture(void);

public:
	static CTexture* Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
		, TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);

private:
	CTexture* Clone_Resource(void);
	HRESULT Create_Texture(TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);

public:
	void Render(const UINT uiSlot, const DWORD& dwIndex);
	void Release(void);

private:
	using VECTEXTURE = vector<ID3D11ShaderResourceView*>;
	VECTEXTURE		m_vecTexture;
	size_t			m_sizetContainerSize;
};