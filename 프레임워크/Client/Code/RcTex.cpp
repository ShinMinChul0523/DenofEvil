#include "stdafx.h"
#include "RcTex.h"

CRcTex::CRcTex(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
	: CResource(pGraphicDev, pContext)
	, m_pVB(NULL)
	, m_pIB(NULL)
{
}

CRcTex::~CRcTex(void)
{
}

CRcTex* CRcTex::Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
{
	CRcTex* pRcTex = new CRcTex(pGraphicDev, pContext);

	if (FAILED(pRcTex->Create_Buffer()))
	{
		MSG_BOX("RcTex Create_Buffer Failed");
		Safe_Release(pRcTex);
	}

	return pRcTex;
}

CRcTex* CRcTex::Clone_Resource(void)
{
	CRcTex* pRcTex = new CRcTex(*this);

	++(*m_pwCloneCnt);

	return pRcTex;
}

HRESULT CRcTex::Create_Buffer(void)
{
	HRESULT hr = E_FAIL;

	// Vertex
	VTXTEX pVtxTex[] = 
	{
		// Pos							TexUV					Normal
		{ XMFLOAT3(-0.5f, 0.5f, 0.f),	XMFLOAT2(0.f, 0.f),		XMFLOAT3(0.f, 0.f, -1.f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.f),	XMFLOAT2(1.f, 0.f),		XMFLOAT3(0.f, 0.f, -1.f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.f),	XMFLOAT2(1.f, 1.f),		XMFLOAT3(0.f, 0.f, -1.f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.f),	XMFLOAT2(0.f, 1.f),		XMFLOAT3(0.f, 0.f, -1.f) }
	};

	UINT uiVtxCnt = ARRAYSIZE(pVtxTex);

	D3D11_BUFFER_DESC tBufferDesc;

	ZeroMemory(&tBufferDesc, sizeof(D3D11_BUFFER_DESC));

	tBufferDesc.ByteWidth					= sizeof(VTXTEX) * uiVtxCnt;
	tBufferDesc.Usage						= D3D11_USAGE_IMMUTABLE;
	tBufferDesc.BindFlags					= D3D11_BIND_VERTEX_BUFFER;
	tBufferDesc.CPUAccessFlags				= 0;	// CPU가 갱신하지 않음

	D3D11_SUBRESOURCE_DATA tSubData;

	ZeroMemory(&tSubData, sizeof(D3D11_SUBRESOURCE_DATA));

	tSubData.pSysMem = pVtxTex;

	hr = m_pGraphicDev->CreateBuffer(&tBufferDesc, &tSubData, &m_pVB);

	if (FAILED(hr) == TRUE)
	{
		MSG_BOX("RcTex VB CreateBuffer Failed");
		return E_FAIL;
	}

	// Index
	UINT pIndex[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	UINT uiIdxCnt = ARRAYSIZE(pIndex);

	tBufferDesc.ByteWidth	= sizeof(UINT) * uiIdxCnt;
	tBufferDesc.BindFlags	= D3D11_BIND_INDEX_BUFFER;

	tSubData.pSysMem = pIndex;

	hr = m_pGraphicDev->CreateBuffer(&tBufferDesc, &tSubData, &m_pIB);

	if (FAILED(hr) == TRUE)
	{
		MSG_BOX("RcTex IB CreateBuffer Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CRcTex::Render(void)
{
	UINT uiStride = sizeof(VTXTEX);
	UINT uiOffset = 0;

	m_pContext->IASetVertexBuffers(0, 1, &m_pVB, &uiStride, &uiOffset);
	m_pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	m_pContext->DrawIndexed(6, 0, 0);
}

void CRcTex::Release(void)
{
	CResource::Release();

	if (m_pwCloneCnt == NULL)
	{
		Safe_Release(m_pVB);
		Safe_Release(m_pIB);
	}

	delete this;
}