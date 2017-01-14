#pragma once

#include "Define.h"

class CGraphicDev
{
	DECLARE_SINGLETON(CGraphicDev);

public:
	enum WINMODE { MODE_FULL, MODE_WIN };

private:
	explicit CGraphicDev(void);
	~CGraphicDev(void);

public:
	IDXGISwapChain*				Get_SwapChain();
	ID3D11RenderTargetView*		Get_RenderTargetView();
	ID3D11DepthStencilView*		Get_DepthStencilView();
	ID3D11Buffer*				Get_BaseShaderCB();
	ID3D11Buffer*				Get_InstShaderCB();
	ID3D11Buffer*				Get_DynamicShaderCB();
	ID3D11SamplerState*			Get_BaseSampler();

public:
	void Set_AlphaEnable(BOOL bEnable);
	void Set_CullEnable(BOOL bEnable);
	void Set_WireFrame(BOOL bWireFrame);

public:
	HRESULT Ready_GraphicDev(WINMODE eWinMode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY, WORD wFPS, BOOL bEnable4xMsaa
		, ID3D11Device*& pGraphicDev, ID3D11DeviceContext*& pContext);

private:
	HRESULT Set_SwapChain(DXGI_SWAP_CHAIN_DESC& sd, WINMODE eWinMode, HWND hWnd
		, const WORD& wSizeX, const WORD& wSizeY, WORD wFPS, BOOL bEnable4xMsaa);

	void Set_DepthStencil(D3D11_TEXTURE2D_DESC& td, const WORD& wSizeX, const WORD& wSizeY, BOOL bEnable4xMsaa);
	void Set_Viewport(D3D11_VIEWPORT& vp, const WORD& wSizeX, const WORD& wSizeY);

public:
	void Release(void);

private:
	// Device
	ID3D11Device*					m_pGraphicDev;
	ID3D11DeviceContext*			m_pContext;
	IDXGISwapChain*					m_pSwapChain;

	// RenderTarget
	ID3D11RenderTargetView*			m_pRenderTargetView;
	ID3D11Texture2D*				m_pDepthStencilBuffer;
	ID3D11DepthStencilView*			m_pDepthStencilView;

	// Shader
	ID3D11Buffer*					m_pBaseShaderCB;
	ID3D11Buffer*					m_pInstShaderCB;
	ID3D11Buffer*					m_pDynamicShaderCB;
	ID3D11SamplerState*				m_pBaseSampler;

	// RS
	ID3D11RasterizerState*			m_pWireFrameRS;
	ID3D11RasterizerState*			m_pSolidRS;
	ID3D11RasterizerState*			m_pNoneCullRS;
	ID3D11BlendState*				m_pAlphaBlendState;
};