#include "stdafx.h"
#include "NavCell.h"
#include "Export.h"

CNavCell::CNavCell(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
	: m_pGraphicDev(pGraphicDev)
	, m_pContext(pContext)
	, m_pTexture(NULL)
	, m_pVB(NULL)
{
	ZeroMemory(m_pNeighbor, sizeof(XMFLOAT3) * NEIGHBOR_END);
	ZeroMemory(m_vNormal, sizeof(XMFLOAT3) * LINE_END);
}

CNavCell::~CNavCell(void)
{
}

CNavCell* CNavCell::Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
							, const XMFLOAT3* pPointA, const XMFLOAT3* pPointB, const XMFLOAT3* pPointC, const UINT& uiIdx)
{
	CNavCell* pNavCell = new CNavCell(pGraphicDev, pContext);

	if (FAILED(pNavCell->Init_Cell(uiIdx, pPointA, pPointB, pPointC)))
	{
		MSG_BOX("CNavCell Create Failed");
		Safe_Release(pNavCell);
	}

	return pNavCell;
}

HRESULT CNavCell::Init_Cell(const UINT& uiIdx, const XMFLOAT3* pPointA, const XMFLOAT3* pPointB, const XMFLOAT3* pPointC)
{
	m_uiIndex = uiIdx;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	XMVECTOR vDirAB = XMLoadFloat3(&m_vPoint[POINT_B]) - XMLoadFloat3(&m_vPoint[POINT_A]);
	XMVECTOR vDirBC = XMLoadFloat3(&m_vPoint[POINT_C]) - XMLoadFloat3(&m_vPoint[POINT_B]);
	XMVECTOR vDirCA = XMLoadFloat3(&m_vPoint[POINT_A]) - XMLoadFloat3(&m_vPoint[POINT_C]);

	XMStoreFloat3(&m_vNormal[LINE_AB], vDirAB);
	XMStoreFloat3(&m_vNormal[LINE_BC], vDirBC);
	XMStoreFloat3(&m_vNormal[LINE_CA], vDirCA);

	XMVECTOR vNormal[LINE_END];
	vNormal[LINE_AB] = XMVectorSet(m_vNormal[LINE_AB].z * -1.f, 0.f, m_vNormal[LINE_AB].x, 0.f);
	vNormal[LINE_BC] = XMVectorSet(m_vNormal[LINE_BC].z * -1.f, 0.f, m_vNormal[LINE_BC].x, 0.f);
	vNormal[LINE_CA] = XMVectorSet(m_vNormal[LINE_CA].z * -1.f, 0.f, m_vNormal[LINE_CA].x, 0.f);

	XMVector3Normalize(vNormal[LINE_AB]);
	XMVector3Normalize(vNormal[LINE_BC]);
	XMVector3Normalize(vNormal[LINE_CA]);

	XMStoreFloat3(&m_vNormal[LINE_AB], vNormal[LINE_AB]);
	XMStoreFloat3(&m_vNormal[LINE_BC], vNormal[LINE_BC]);
	XMStoreFloat3(&m_vNormal[LINE_CA], vNormal[LINE_CA]);

	// Vertex Buffer
	VTXTEX pVtxTex[] =
	{
		// Pos																				TexUV				Normal
		{ XMFLOAT3(m_vPoint[POINT_A].x, m_vPoint[POINT_A].y, m_vPoint[POINT_A].z),	XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) },
		{ XMFLOAT3(m_vPoint[POINT_B].x, m_vPoint[POINT_B].y, m_vPoint[POINT_B].z), XMFLOAT2(1.f, 0.f),	XMFLOAT3(0.f, 0.f, 0.f) },
		{ XMFLOAT3(m_vPoint[POINT_C].x, m_vPoint[POINT_C].y, m_vPoint[POINT_C].z), XMFLOAT2(1.f, 1.f),	XMFLOAT3(0.f, 0.f, 0.f) }
	};

	UINT uiVtxCnt = ARRAYSIZE(pVtxTex);

	D3D11_BUFFER_DESC tBufferDesc;

	ZeroMemory(&tBufferDesc, sizeof(D3D11_BUFFER_DESC));

	tBufferDesc.ByteWidth = sizeof(VTXTEX) * uiVtxCnt;
	tBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSubData;

	ZeroMemory(&tSubData, sizeof(D3D11_SUBRESOURCE_DATA));

	tSubData.pSysMem = pVtxTex;

	HRESULT hr = m_pGraphicDev->CreateBuffer(&tBufferDesc, &tSubData, &m_pVB);

	if (FAILED(hr) == true)
	{
		MSG_BOX("NavMesh VB CreateBuffer Failed");
		return E_FAIL;
	}

	// Texture
	m_pTexture = dynamic_cast<CTexture*>(Clone_ResourceMgr(RESOURCE_STAGE, L"Texture_Color"));

	if (m_pTexture == NULL)
	{
		MSG_BOX("NavCell Clone_Texture Failed");
		return E_FAIL;
	}

	return S_OK;
}

BOOL CNavCell::Compare_Point(const XMFLOAT3* pPointA, const XMFLOAT3* pPointB, CNavCell* pNeighbor)
{
	// **************************************************************************************
	if ((*pPointA).x == m_vPoint[POINT_A].x && (*pPointA).y == m_vPoint[POINT_A].y && (*pPointA).z == m_vPoint[POINT_A].z)
	{
		if ((*pPointB).x == m_vPoint[POINT_B].x && (*pPointB).y == m_vPoint[POINT_B].y && (*pPointB).z == m_vPoint[POINT_B].z)
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return TRUE;
		}
		else if ((*pPointB).x == (m_vPoint[POINT_C]).x && (*pPointB).y == (m_vPoint[POINT_C]).y && (*pPointB).z == (m_vPoint[POINT_C]).z)
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return TRUE;
		}
	}

	if ((*pPointA).x == m_vPoint[POINT_B].x && (*pPointA).y == m_vPoint[POINT_B].y && (*pPointA).z == m_vPoint[POINT_B].z)
	{
		if ((*pPointB).x == m_vPoint[POINT_A].x && (*pPointB).y == m_vPoint[POINT_A].y && (*pPointB).z == m_vPoint[POINT_A].z)
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return TRUE;
		}
		else if ((*pPointB).x == (m_vPoint[POINT_C]).x && (*pPointB).y == (m_vPoint[POINT_C]).y && (*pPointB).z == (m_vPoint[POINT_C]).z)
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return TRUE;
		}
	}

	if ((*pPointA).x == m_vPoint[POINT_C].x && (*pPointA).y == m_vPoint[POINT_C].y && (*pPointA).z == m_vPoint[POINT_C].z)
	{
		if ((*pPointB).x == m_vPoint[POINT_A].x && (*pPointB).y == m_vPoint[POINT_A].y && (*pPointB).z == m_vPoint[POINT_A].z)
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return TRUE;
		}
		else if ((*pPointB).x == (m_vPoint[POINT_B]).x && (*pPointB).y == (m_vPoint[POINT_B]).y && (*pPointB).z == (m_vPoint[POINT_B]).z)
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return TRUE;
		}
	}

	return FALSE;
}

void CNavCell::Render(BOOL bColCell)
{
	m_pContext->IASetInputLayout(CShaderMgr::GetInstance()->Get_InputLayout(L"Shader_Default"));

	ID3D11Buffer* pConstantBuffer = Get_BaseShaderCB();
	ID3D11SamplerState* pBaseSampler = Get_BaseSampler();

	BASESHADER_CB tConstantBuffer;

	tConstantBuffer.matWorld = XMMatrixTranspose(XMMatrixIdentity());
	tConstantBuffer.matView = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraView()));
	tConstantBuffer.matProj = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraProj()));

	m_pContext->UpdateSubresource(pConstantBuffer, 0, NULL, &tConstantBuffer, 0, 0);

	m_pContext->VSSetShader(CShaderMgr::GetInstance()->Get_VertexShader(L"Shader_Default"), NULL, 0);
	m_pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	m_pContext->PSSetShader(CShaderMgr::GetInstance()->Get_PixelShader(L"Shader_Default"), NULL, 0);
	m_pContext->PSSetSamplers(0, 1, &pBaseSampler);

	m_pTexture->Render(0, bColCell);

	UINT uiStride = sizeof(VTXTEX);
	UINT uiOffset = 0;

	m_pContext->IASetVertexBuffers(0, 1, &m_pVB, &uiStride, &uiOffset);
	m_pContext->Draw(3, 0);
}

void CNavCell::Release(void)
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pVB);

	delete this;
}

const XMFLOAT3* CNavCell::Get_Point(POINT PointFlag)
{
	return &m_vPoint[PointFlag];
}

const XMFLOAT3* CNavCell::Get_Normal(LINE LineFlag)
{
	return &m_vNormal[LineFlag];
}

CNavCell* CNavCell::Get_Neighbor(NEIGHBOR NeighborFlag)
{
	return m_pNeighbor[NeighborFlag];
}

const UINT CNavCell::Get_Index(void)
{
	return m_uiIndex;
}

void CNavCell::Set_Neighbor(NEIGHBOR eNeighbor, CNavCell* pNeighbor)
{
	m_pNeighbor[eNeighbor] = pNeighbor;

}
