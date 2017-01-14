#include "stdafx.h"
#include "StaticMesh.h"
#include "Export.h"
#include "Texture.h"

CStaticMesh::CStaticMesh(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
	: CMesh(pGraphicDev, pContext)
{
}

CStaticMesh::~CStaticMesh(void)
{
}

CStaticMesh* CStaticMesh::Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const VTXTEX* pVB, const UINT& uiVtxCnt
	, const UINT* pIB, const UINT& uiIdxCnt, const XMFLOAT3& vMin, const XMFLOAT3& vMax, TCHAR* pTexName)
{
	CStaticMesh* pMesh = new CStaticMesh(pGraphicDev, pContext);

	if (FAILED(pMesh->Create_Buffer(pVB, uiVtxCnt, pIB, uiIdxCnt, vMin, vMax, pTexName)))
	{	
		MSG_BOX("CStaticMesh Create_Buffer Failed");
		Safe_Release(pMesh);
	}

	return pMesh;
}

HRESULT CStaticMesh::Create_Buffer(const VTXTEX* pVB, const UINT& uiVtxCnt, const UINT* pIB, const UINT& uiIdxCnt
	, const XMFLOAT3& vMin, const XMFLOAT3& vMax, TCHAR* pTexName)
{
	m_vMin = vMin;
	m_vMax = vMax;

	m_uiVtxCnt = uiVtxCnt;
	m_uiIdxCnt = uiIdxCnt;

	HRESULT hr = E_FAIL;

	if (m_uiVtxCnt && m_uiIdxCnt)
	{
		D3D11_BUFFER_DESC tBufferDesc;

		ZeroMemory(&tBufferDesc, sizeof(D3D11_BUFFER_DESC));

		tBufferDesc.ByteWidth = sizeof(VTXTEX) * m_uiVtxCnt;
		tBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA tSubData;

		ZeroMemory(&tSubData, sizeof(D3D11_SUBRESOURCE_DATA));

		tSubData.pSysMem = pVB;

		hr = m_pGraphicDev->CreateBuffer(&tBufferDesc, &tSubData, &m_pVB);

		if (FAILED(hr) == TRUE)
		{
			MSG_BOX("CMesh VB CreateBuffer Failed");
			return E_FAIL;
		}

		tBufferDesc.ByteWidth = sizeof(UINT) * m_uiIdxCnt;
		tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		tSubData.pSysMem = pIB;

		hr = m_pGraphicDev->CreateBuffer(&tBufferDesc, &tSubData, &m_pIB);

		if (FAILED(hr) == TRUE)
		{
			MSG_BOX("CMesh IB CreateBuffer Failed");
			return E_FAIL;
		}

		wstring wstrTexName = L"Texture_";
		wstrTexName += pTexName;

		if (wstrTexName != L"Texture_")
			m_pTexture = dynamic_cast<CTexture*>(Clone_ResourceMgr(RESOURCE_STAGE, wstrTexName.c_str()));
	}

	hr = Set_BoundingBox();

	if (FAILED(hr) == TRUE)
	{
		MSG_BOX("CMesh Set_BoundingBox Failed");
		return E_FAIL;
	}

	return S_OK;
}

CResource* CStaticMesh::Clone_Resource(void)
{
	CStaticMesh* pMesh = new CStaticMesh(*this);

	++(*m_pwCloneCnt);

	return pMesh;
}

void CStaticMesh::Render(BOOL bColliderDraw)
{
	{
		UINT uiStride = sizeof(VTXTEX);
		UINT uiOffset = 0;

		if (m_uiVtxCnt != 0)
		{
			// Texture
			if (m_pTexture) m_pTexture->Render(0, 0);

			// Mesh
			m_pContext->IASetVertexBuffers(0, 1, &m_pVB, &uiStride, &uiOffset);
			m_pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

			m_pContext->DrawIndexed(m_uiIdxCnt, 0, 0);
		}

		// Bounding Box
		if (bColliderDraw == TRUE)
		{
			CGraphicDev::GetInstance()->Set_WireFrame(TRUE);

			m_pContext->IASetVertexBuffers(0, 1, &m_pBBoxVB, &uiStride, &uiOffset);
			m_pContext->IASetIndexBuffer(m_pBBoxIB, DXGI_FORMAT_R32_UINT, 0);

			m_pContext->DrawIndexed(36, 0, 0);

			CGraphicDev::GetInstance()->Set_WireFrame(FALSE);
		}
	}

	for (UINT uiSize = 0; uiSize < m_vecChild.size(); ++uiSize)
		m_vecChild[uiSize]->Render(bColliderDraw);
}

void CStaticMesh::RenderInst(const vector<XMFLOAT4X4*>& vecObjWorld)
{
	UINT uiStride = sizeof(VTXTEX);
	UINT uiOffset = 0;

	if (m_uiVtxCnt != 0)
	{
		m_pContext->IASetInputLayout(Get_InputLayout(L"Shader_Instancing"));

		ID3D11Buffer* pBaseShaderCB = Get_BaseShaderCB();
		ID3D11Buffer* pInstShaderCB = Get_InstShaderCB();

		ID3D11SamplerState* pBaseSampler = Get_BaseSampler();

		// Shader Set
		m_pContext->VSSetShader(Get_VertexShader(L"Shader_Instancing"), NULL, 0);
		m_pContext->VSSetConstantBuffers(0, 1, &pBaseShaderCB);
		m_pContext->VSSetConstantBuffers(1, 1, &pInstShaderCB);
		m_pContext->PSSetShader(Get_PixelShader(L"Shader_Instancing"), NULL, 0);
		m_pContext->PSSetSamplers(0, 1, &pBaseSampler);

		//Texture
		if(m_pTexture) m_pTexture->Render(0, 0);

		// Mesh
		m_pContext->IASetVertexBuffers(0, 1, &m_pVB, &uiStride, &uiOffset);
		m_pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

		// Base
		BASESHADER_CB tBaseShaderCB;

		tBaseShaderCB.matWorld = XMMatrixTranspose(XMMatrixIdentity());
		tBaseShaderCB.matView = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraView()));
		tBaseShaderCB.matProj = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraProj()));

		m_pContext->UpdateSubresource(pBaseShaderCB, 0, NULL, &tBaseShaderCB, 0, 0);

		UINT uiCnt = UINT(vecObjWorld.size() / INSTCNT) + 1;

		// Instancing
		for (UINT uiDrawCnt = 0; uiDrawCnt < UINT(vecObjWorld.size() / INSTCNT) + 1; ++uiDrawCnt)
		{
			INSTSHADER_CB tInstShaderCB;
			UINT uiInstSize = vecObjWorld.size() - (uiDrawCnt * INSTCNT);

			if (uiInstSize > INSTCNT)
				uiInstSize = INSTCNT;

			for (UINT uiIndex = 0; uiIndex < uiInstSize; ++uiIndex)
				tInstShaderCB.matWorld[uiIndex] = XMMatrixTranspose(XMLoadFloat4x4(vecObjWorld[uiIndex + (uiDrawCnt * INSTCNT)]));

			m_pContext->UpdateSubresource(pInstShaderCB, 0, NULL, &tInstShaderCB, 0, 0);

			// Draw
			m_pContext->DrawIndexedInstanced(m_uiIdxCnt, uiInstSize, 0, 0, 0);

		}
	}

	for (UINT uiSize = 0; uiSize < m_vecChild.size(); ++uiSize)
		m_vecChild[uiSize]->RenderInst(vecObjWorld);
}

void CStaticMesh::Release(void)
{
	CMesh::Release();

	if (m_pwCloneCnt == NULL)
	{
	}

	delete this;
}

HRESULT CStaticMesh::Set_BoundingBox(void)
{
	// Vertex
	VTXTEX pVtxTex[] =
	{
		// Pos										TexUV				Normal
		{ XMFLOAT3(m_vMin.x, m_vMax.y, m_vMin.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) },
		{ XMFLOAT3(m_vMax.x, m_vMax.y, m_vMin.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) },
		{ XMFLOAT3(m_vMax.x, m_vMin.y, m_vMin.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) },
		{ XMFLOAT3(m_vMin.x, m_vMin.y, m_vMin.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) },
		{ XMFLOAT3(m_vMin.x, m_vMax.y, m_vMax.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) },
		{ XMFLOAT3(m_vMax.x, m_vMax.y, m_vMax.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) },
		{ XMFLOAT3(m_vMax.x, m_vMin.y, m_vMax.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) },
		{ XMFLOAT3(m_vMin.x, m_vMin.y, m_vMax.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) }
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

	HRESULT hr = m_pGraphicDev->CreateBuffer(&tBufferDesc, &tSubData, &m_pBBoxVB);

	if (FAILED(hr) == TRUE)
	{
		MSG_BOX("Bouding Box VB CreateBuffer Failed");
		return E_FAIL;
	}

	// Index
	UINT pIndex[] =
	{
		1, 5, 6,
		1, 6, 2,
		4, 0, 3,
		4, 3, 7,
		4, 5, 1,
		4, 1, 0,
		3, 2, 6,
		3, 6, 7,
		7, 6, 5,
		7, 5, 4,
		0, 1, 2,
		0, 2, 3
	};

	UINT uiIdxCnt = ARRAYSIZE(pIndex);

	tBufferDesc.ByteWidth = sizeof(UINT) * uiIdxCnt;
	tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	tSubData.pSysMem = pIndex;

	hr = m_pGraphicDev->CreateBuffer(&tBufferDesc, &tSubData, &m_pBBoxIB);

	if (FAILED(hr) == TRUE)
	{
		MSG_BOX("Bounding Box IB CreateBuffer Failed");
		return E_FAIL;
	}

	return S_OK;
}