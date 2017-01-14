#include "stdafx.h"
#include "GraphicDev.h"

IMPLEMENT_SINGLETON(CGraphicDev)

CGraphicDev::CGraphicDev(void)
: m_pGraphicDev(NULL)
, m_pContext(NULL)
, m_pSwapChain(NULL)
, m_pRenderTargetView(NULL)
, m_pDepthStencilBuffer(NULL)
, m_pDepthStencilView(NULL)
, m_pBaseSampler(NULL)
, m_pBaseShaderCB(NULL)
, m_pInstShaderCB(NULL)
, m_pDynamicShaderCB(NULL)
, m_pAlphaBlendState(NULL)
, m_pWireFrameRS(NULL)
, m_pSolidRS(NULL)
, m_pNoneCullRS(NULL)
{
}

CGraphicDev::~CGraphicDev(void)
{
}

HRESULT CGraphicDev::Ready_GraphicDev(WINMODE eWinMode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY, WORD wFPS, BOOL bEnable4xMsaa, ID3D11Device*& pGraphicDev, ID3D11DeviceContext*& pContext)
{
	// Create Device
	HRESULT hr = E_FAIL;
	UINT uiDeviceFlag = 0;

#if defined(DEBUG) || defined(_DEBUG)
	uiDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, uiDeviceFlag,
		0, 0, D3D11_SDK_VERSION, &m_pGraphicDev, NULL, &m_pContext);

	if (FAILED(hr) == TRUE)
	{
		MSG_BOX("D3D11CreateDevice Failed");
		return E_FAIL;
	}

	// Get DXGIFactory
	IDXGIDevice* pDXGIDevice = NULL;

	hr = m_pGraphicDev->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);

	if (FAILED(hr) == TRUE)
	{
		MSG_BOX("Get IDXGIDevice Failed");
		return E_FAIL;
	}

	IDXGIAdapter* pDXGIAdapter = NULL;

	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);

	if (FAILED(hr) == TRUE)
	{
		MSG_BOX("Get IDXGIAdapter Failed");
		return E_FAIL;
	}

	IDXGIFactory* pDXGIFactory = NULL;

	hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);

	if (FAILED(hr) == TRUE)
	{
		MSG_BOX("Get IDXGIFactory Failed");
		return E_FAIL;
	}

	// Swap Chain
	DXGI_SWAP_CHAIN_DESC sd;

	hr = Set_SwapChain(sd, eWinMode, hWnd, wSizeX, wSizeY, wFPS, bEnable4xMsaa);

	if (FAILED(hr) == TRUE)
	{
		MSG_BOX("Set_SwapChain Failed");
		return E_FAIL;
	}

	hr = pDXGIFactory->CreateSwapChain(m_pGraphicDev, &sd, &m_pSwapChain);

	if (FAILED(hr) == TRUE)
	{
		MSG_BOX("CreateSwapChain Failed");
		return E_FAIL;
	}

	Safe_Release(pDXGIDevice);
	Safe_Release(pDXGIAdapter);
	Safe_Release(pDXGIFactory);

	// Create Render Target
	ID3D11Texture2D* pBackBuffer;

	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	m_pGraphicDev->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView);

	Safe_Release(pBackBuffer);

	// Create DepthStencilBuffer
	D3D11_TEXTURE2D_DESC td;

	Set_DepthStencil(td, wSizeX, wSizeY, bEnable4xMsaa);

	if (FAILED(m_pGraphicDev->CreateTexture2D(&td, 0, &m_pDepthStencilBuffer)))
	{
		MSG_BOX("Create DepthStencilBuffer Failed");
		return E_FAIL;
	}

	if (FAILED(m_pGraphicDev->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView)))
	{
		MSG_BOX("Create DepthStencilView Failed");
		return E_FAIL;
	}

	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Set Viewport
	D3D11_VIEWPORT vp;

	Set_Viewport(vp, wSizeX, wSizeY);

	m_pContext->RSSetViewports(1, &vp);

	// Set PrimitiveTopology
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create ConstantBuffer
	// Base ConstantBuffer
	D3D11_BUFFER_DESC tBufferDesc;
	ZeroMemory(&tBufferDesc, sizeof(D3D11_BUFFER_DESC));

	tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tBufferDesc.ByteWidth = sizeof(BASESHADER_CB);
	tBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (FAILED(m_pGraphicDev->CreateBuffer(&tBufferDesc, NULL, &m_pBaseShaderCB)))
	{
		MSG_BOX("Create BaseShader ConstantBuffer Failed");
		return E_FAIL;
	}

	// Instancing ConstantBuffer
	tBufferDesc.ByteWidth = sizeof(INSTSHADER_CB);

	if (FAILED(m_pGraphicDev->CreateBuffer(&tBufferDesc, NULL, &m_pInstShaderCB)))
	{
		MSG_BOX("Create Instancing ConstantBuffer Failed");
		return E_FAIL;
	}

	// DynamicMesh ConstantBuffer
	tBufferDesc.ByteWidth = sizeof(DYNAMICSHADER_CB);

	if (FAILED(m_pGraphicDev->CreateBuffer(&tBufferDesc, NULL, &m_pDynamicShaderCB)))
	{
		MSG_BOX("Create DynamicMesh ConstantBuffer Failed");
		return E_FAIL;
	}

	// Sampler
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	if (bEnable4xMsaa == TRUE)
		sampDesc.MaxAnisotropy = 4;

	else
		sampDesc.MaxAnisotropy = 1;

	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	m_pGraphicDev->CreateSamplerState(&sampDesc, &m_pBaseSampler);

	// Create RasterizerState
	D3D11_RASTERIZER_DESC tRasterizerDesc;

	ZeroMemory(&tRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	// Soild
	tRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	tRasterizerDesc.CullMode = D3D11_CULL_BACK;
	tRasterizerDesc.DepthClipEnable = TRUE;

	if (FAILED(m_pGraphicDev->CreateRasterizerState(&tRasterizerDesc, &m_pSolidRS)))
	{
		MSG_BOX("CreateRasterizerState Failed");
		return E_FAIL;
	}

	// Wireframe
	tRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

	if (FAILED(m_pGraphicDev->CreateRasterizerState(&tRasterizerDesc, &m_pWireFrameRS)))
	{
		MSG_BOX("CreateRasterizerState Failed");
		return E_FAIL;
	}

	// None Cull
	tRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	tRasterizerDesc.CullMode = D3D11_CULL_NONE;

	if (FAILED(m_pGraphicDev->CreateRasterizerState(&tRasterizerDesc, &m_pNoneCullRS)))
	{
		MSG_BOX("CreateRasterizerState Failed");
		return E_FAIL;
	}

	m_pContext->RSSetState(m_pSolidRS);

	// Create BlendState
	D3D11_BLEND_DESC tBSDesc;
	ZeroMemory(&tBSDesc, sizeof(D3D11_BLEND_DESC));

	tBSDesc.RenderTarget[0].BlendEnable = TRUE;
	tBSDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tBSDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	tBSDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tBSDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tBSDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	tBSDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tBSDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	if (FAILED(m_pGraphicDev->CreateBlendState(&tBSDesc, &m_pAlphaBlendState)))
	{
		MSG_BOX("CreateBlendState Failed");
		return E_FAIL;
	}

	pGraphicDev = m_pGraphicDev;
	pContext = m_pContext;

	return S_OK;
}

void CGraphicDev::Release(void)
{
	if (Safe_Com_Release(m_pNoneCullRS))
		MSG_BOX("m_pNoneCullRS Release Failed");

	if (Safe_Com_Release(m_pSolidRS))
		MSG_BOX("m_pSolidRS Release Failed");

	if (Safe_Com_Release(m_pWireFrameRS))
		MSG_BOX("m_pWireFrameRS Release Failed");

	if (Safe_Com_Release(m_pAlphaBlendState))
		MSG_BOX("m_pAlphaBlendState Release Failed");

	if (Safe_Com_Release(m_pBaseSampler))
		MSG_BOX("m_pBaseSampler Release Failed");

	if (Safe_Com_Release(m_pDynamicShaderCB))
		MSG_BOX("m_pDynamicShaderCB Release Failed");

	if (Safe_Com_Release(m_pInstShaderCB))
		MSG_BOX("m_pInstShaderCB Release Failed");

	if (Safe_Com_Release(m_pBaseShaderCB))
		MSG_BOX("m_pBaseShaderCB Release Failed");

	if (Safe_Com_Release(m_pRenderTargetView))
		MSG_BOX("m_pRenderTargetView Release Failed");

	if (Safe_Com_Release(m_pDepthStencilBuffer))
		MSG_BOX("m_pDepthStencilBuffer Release Failed");

	if (Safe_Com_Release(m_pDepthStencilView))
		MSG_BOX("m_pDepthStencilView Release Failed");

	if (Safe_Com_Release(m_pContext))
		MSG_BOX("m_pContext Release Failed");

	if (Safe_Com_Release(m_pSwapChain))
		MSG_BOX("m_pSwapChain Release Failed");

	if (Safe_Com_Release(m_pGraphicDev))
		//MSG_BOX("m_pGraphicDev Release Failed");

	delete this;
}

HRESULT CGraphicDev::Set_SwapChain(DXGI_SWAP_CHAIN_DESC& sd, WINMODE eWinMode, HWND hWnd
	, const WORD& wSizeX, const WORD& wSizeY, WORD wFPS, BOOL bEnable4xMsaa)
{
	HRESULT hr = E_FAIL;

	sd.BufferDesc.Width = wSizeX;
	sd.BufferDesc.Height = wSizeY;
	sd.BufferDesc.RefreshRate.Numerator = wFPS;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4X MSAA
	if (bEnable4xMsaa == TRUE)
	{
		UINT ui4xMsaaQuality;

		hr = m_pGraphicDev->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
			4, &ui4xMsaaQuality);

		if (FAILED(hr) == TRUE)
		{
			MSG_BOX("CheckMultisampleQualityLevels Failed");
			return E_FAIL;
		}

		assert(ui4xMsaaQuality > 0);

		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = ui4xMsaaQuality - 1;
	}

	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = eWinMode;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	return S_OK;
}

void CGraphicDev::Set_DepthStencil(D3D11_TEXTURE2D_DESC& td, const WORD& wSizeX, const WORD& wSizeY, BOOL bEnable4xMsaa)
{
	td.Width = wSizeX;
	td.Height = wSizeY;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (bEnable4xMsaa == TRUE)
	{
		UINT ui4xMsaaQuality;

		m_pGraphicDev->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &ui4xMsaaQuality);
		assert(ui4xMsaaQuality > 0);

		td.SampleDesc.Count = 4;
		td.SampleDesc.Quality = ui4xMsaaQuality - 1;
	}

	else
	{
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
	}

	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
}

void CGraphicDev::Set_Viewport(D3D11_VIEWPORT& vp, const WORD& wSizeX, const WORD& wSizeY)
{
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
	vp.Width = static_cast<float>(wSizeX);
	vp.Height = static_cast<float>(wSizeY);
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
}

IDXGISwapChain* CGraphicDev::Get_SwapChain()
{
	return m_pSwapChain;
}

ID3D11RenderTargetView* CGraphicDev::Get_RenderTargetView()
{
	return m_pRenderTargetView;
}

ID3D11DepthStencilView* CGraphicDev::Get_DepthStencilView()
{
	return m_pDepthStencilView;
}

ID3D11Buffer* CGraphicDev::Get_BaseShaderCB()
{
	return m_pBaseShaderCB;
}

ID3D11Buffer* CGraphicDev::Get_InstShaderCB()
{
	return m_pInstShaderCB;
}

ID3D11Buffer* CGraphicDev::Get_DynamicShaderCB()
{
	return m_pDynamicShaderCB;
}

ID3D11SamplerState* CGraphicDev::Get_BaseSampler()
{
	return m_pBaseSampler;
}

void CGraphicDev::Set_AlphaEnable(BOOL bEnable)
{
	if (bEnable)
		m_pContext->OMSetBlendState(m_pAlphaBlendState, NULL, 0xFFFFFF);

	else
		m_pContext->OMSetBlendState(NULL, NULL, 0xFFFFFF);
}

void CGraphicDev::Set_CullEnable(BOOL bEnable)
{
	if (bEnable)
		m_pContext->RSSetState(m_pSolidRS);

	else
		m_pContext->RSSetState(m_pNoneCullRS);
}

void CGraphicDev::Set_WireFrame(BOOL bWireFrame)
{
	if (bWireFrame)
		m_pContext->RSSetState(m_pWireFrameRS);

	else
		m_pContext->RSSetState(m_pSolidRS);
}