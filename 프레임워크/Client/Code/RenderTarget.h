#pragma once

#include "Define.h"

class CRcTex;
class CTransform;
class CRenderTexture
{
private:
	explicit CRenderTexture(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	~CRenderTexture(void);

public:
	ID3D11RenderTargetView** Get_RanderTargetView(void);
	ID3D11DepthStencilView* Get_DepthStencilView(void);
	void Set_RenderTarget(const UINT& uiNumViews, BOOL bDepthStencilView);
	void Set_Texture(const UINT& uiSlot, const UINT uiIndex);

public:
	static CRenderTexture* Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
									, const DXGI_FORMAT& eFormat, const WORD& wSizeX, const WORD& wSizeY, FLOAT fRenderPosX, FLOAT fRenderPosY);
	void Render(void);
	void Release(void);

private:
	HRESULT Ready_RenderTexture(const DXGI_FORMAT& eFormat, const WORD& wSizeX, const WORD& wSizeY, FLOAT fRenderPosX, FLOAT fRenderPosY);

private:
	ID3D11Device*			m_pGraphicDev;
	ID3D11DeviceContext*	m_pContext;

private:
	ID3D11Texture2D*			m_pRTTexture;
	ID3D11RenderTargetView*		m_pRTView[2];
	ID3D11ShaderResourceView*	m_pShaderResourceView[2];
	D3D11_TEXTURE2D_DESC		m_tTextureDesc;
	ID3D11DepthStencilView*		m_pDepthStencilView;

private:
	CTransform*		m_pTransform;
	CRcTex*			m_pRcTex;
};