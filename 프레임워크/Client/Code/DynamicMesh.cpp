#include "stdafx.h"
#include "DynamicMesh.h"
#include "Export.h"
#include "Texture.h"
#include "Animation.h"
#include "AnimationInfo.h"

CDynaicMesh::CDynaicMesh(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
	: CMesh(pGraphicDev, pContext)
	, m_pOriVertex(nullptr)
	, m_pVertex(nullptr)
	, m_pIndex(nullptr)
	, m_pAnimation(nullptr)
	, m_pBoneWorld(nullptr)
{
	ZeroMemory(&m_vPivotPos, sizeof(XMFLOAT3));
}

CDynaicMesh::~CDynaicMesh(void)
{
}

CDynaicMesh* CDynaicMesh::Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const VTXBONE* pVB, const UINT& uiVtxCnt
	, const UINT* pIB, const UINT& uiIdxCnt, const XMFLOAT3& vMin, const XMFLOAT3& vMax, TCHAR* pTexName, CAnimation* pAnimation)
{
	CDynaicMesh* pMesh = new CDynaicMesh(pGraphicDev, pContext);

	if (FAILED(pMesh->Create_Buffer(pVB, uiVtxCnt, pIB, uiIdxCnt, vMin, vMax, pTexName, pAnimation)))
	{
		MSG_BOX("CDynaicMesh Create_Buffer Failed");
		Safe_Release(pMesh);
	}

	return pMesh;
}

HRESULT CDynaicMesh::Create_Buffer(const VTXBONE* pVB, const UINT& uiVtxCnt, const UINT* pIB, const UINT& uiIdxCnt
	, const XMFLOAT3& vMin, const XMFLOAT3& vMax, TCHAR* pTexName, CAnimation* pAnimation)
{
	m_pAnimation = pAnimation;

	m_vMin = vMin;
	m_vMax = vMax;

	m_uiVtxCnt = uiVtxCnt;
	m_uiIdxCnt = uiIdxCnt;

	HRESULT hr = E_FAIL;

	if (m_uiVtxCnt && m_uiIdxCnt)
	{
		m_pOriVertex = new VTXBONE[m_uiVtxCnt];
		memcpy(m_pOriVertex, pVB, sizeof(VTXBONE) * m_uiVtxCnt);

		m_pVertex = new VTXBONE[m_uiVtxCnt];
		memcpy(m_pVertex, pVB, sizeof(VTXBONE) * m_uiVtxCnt);

		m_pIndex = new UINT[m_uiIdxCnt];
		memcpy(m_pIndex, pIB, sizeof(UINT) * m_uiIdxCnt);

		m_pBoneWorld = new XMFLOAT4X4[MAX_BONE_MATRICES];

		m_vPivotPos = pVB[0].vPos;

		D3D11_BUFFER_DESC tBufferDesc;

		ZeroMemory(&tBufferDesc, sizeof(D3D11_BUFFER_DESC));

		tBufferDesc.ByteWidth = sizeof(VTXBONE) * m_uiVtxCnt;
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

CResource* CDynaicMesh::Clone_Resource(void)
{
	CDynaicMesh* pMesh = new CDynaicMesh(*this);

	++(*m_pwCloneCnt);

	return pMesh;
}

void CDynaicMesh::Render(BOOL bColliderDraw)
{
	{
		UINT uiStride = sizeof(VTXBONE);
		UINT uiOffset = 0;

		if (m_uiVtxCnt != 0)
		{
			// Texture
			if (m_pTexture) m_pTexture->Render(0, 0);

			// Animation
			m_pAnimation->UpdateSubresource(nullptr);

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

void CDynaicMesh::RenderAnim(CAnimationInfo* pAnimInfo, BOOL bColliderDraw /*= FALSE*/)
{
	{
		UINT uiStride = sizeof(VTXBONE);
		UINT uiOffset = 0;

		if (m_uiVtxCnt != 0)
		{
			// Texture
			if (m_pTexture) m_pTexture->Render(0, 0);

			// Animation
			m_pAnimation->UpdateSubresource(pAnimInfo, m_pBoneWorld);

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
		dynamic_cast<CDynaicMesh*>(m_vecChild[uiSize])->RenderAnim(pAnimInfo, bColliderDraw);
}

void CDynaicMesh::RenderInst(const vector<XMFLOAT4X4*>& vecObjWorld)
{
	/*
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
		if (m_pTexture) m_pTexture->Render(0, 0);

		// Mesh
		m_pContext->IASetVertexBuffers(0, 1, &m_pVB, &uiStride, &uiOffset);
		m_pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

		// Base
		BASESHADER_CB tBaseShaderCB;

		tBaseShaderCB.matWorld = XMMatrixTranspose(XMMatrixIdentity());
		tBaseShaderCB.matView = XMMatrixTranspose(XMLoadFloat4x4(CStaticCamera::Get_View()));
		tBaseShaderCB.matProj = XMMatrixTranspose(XMLoadFloat4x4(CStaticCamera::Get_Proj()));

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
	*/
}

void CDynaicMesh::Release(void)
{
	CMesh::Release();

	if (m_pwCloneCnt == NULL)
	{
		Safe_Delete(m_pOriVertex);	
		Safe_Delete(m_pVertex);
		Safe_Delete(m_pIndex);
		Safe_Delete(m_pBoneWorld);

		Safe_Release(m_pAnimation);
	}

	delete this;
}

HRESULT CDynaicMesh::Set_BoundingBox(void)
{
	// Vertex
	VTXBONE pVtxTex[] =
	{
		// Pos										TexUV				Normal					Bone			Weight
		{ XMFLOAT3(m_vMin.x, m_vMax.y, m_vMin.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f), { 0, 0, 0, 0 }, { 1.f, 0.f, 0.f, 0.f } },
		{ XMFLOAT3(m_vMax.x, m_vMax.y, m_vMin.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f), { 0, 0, 0, 0 }, { 1.f, 0.f, 0.f, 0.f } },
		{ XMFLOAT3(m_vMax.x, m_vMin.y, m_vMin.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f), { 0, 0, 0, 0 }, { 1.f, 0.f, 0.f, 0.f } },
		{ XMFLOAT3(m_vMin.x, m_vMin.y, m_vMin.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f), { 0, 0, 0, 0 }, { 1.f, 0.f, 0.f, 0.f } },
		{ XMFLOAT3(m_vMin.x, m_vMax.y, m_vMax.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f), { 0, 0, 0, 0 }, { 1.f, 0.f, 0.f, 0.f } },
		{ XMFLOAT3(m_vMax.x, m_vMax.y, m_vMax.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f), { 0, 0, 0, 0 }, { 1.f, 0.f, 0.f, 0.f } },
		{ XMFLOAT3(m_vMax.x, m_vMin.y, m_vMax.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f), { 0, 0, 0, 0 }, { 1.f, 0.f, 0.f, 0.f } },
		{ XMFLOAT3(m_vMin.x, m_vMin.y, m_vMax.z), XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f), { 0, 0, 0, 0 }, { 1.f, 0.f, 0.f, 0.f } }
	};

	UINT uiVtxCnt = ARRAYSIZE(pVtxTex);

	D3D11_BUFFER_DESC tBufferDesc;

	ZeroMemory(&tBufferDesc, sizeof(D3D11_BUFFER_DESC));

	tBufferDesc.ByteWidth = sizeof(VTXBONE) * uiVtxCnt;
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

XMFLOAT4X4 CDynaicMesh::Get_TransBoneMatrix(INT iIndex)
{
	if (iIndex < INT(m_vecChild.size()) && iIndex != -1)
		return dynamic_cast<CDynaicMesh*>(m_vecChild[iIndex])->Get_TransBoneMatrix(-1);

	else
	{
		XMFLOAT4X4 matTransBone;
		XMStoreFloat4x4(&matTransBone, XMMatrixTranslationFromVector(XMLoadFloat3(&m_vPivotPos)) * XMLoadFloat4x4(&m_pBoneWorld[0]));
		return matTransBone;
	}
}

void CDynaicMesh::Add_ObbCheckList(const XMFLOAT4X4* pWorld, const CGameObject* pObj)
{
	for (size_t iIndex = 0; iIndex < m_vecChild.size(); ++iIndex)
	{
		CDynaicMesh* pChild = dynamic_cast<CDynaicMesh*>(m_vecChild[iIndex]);

		XMFLOAT4X4 matWorld;
		XMStoreFloat4x4(&matWorld, XMLoadFloat4x4(&pChild->m_pBoneWorld[0]) * XMLoadFloat4x4(pWorld));

		VTXBONE* pOriVertex = pChild->m_pOriVertex;
		XMFLOAT4X4* pBoneWorld = pChild->m_pBoneWorld;

		for (UINT uiIndex = 0; uiIndex < pChild->m_uiVtxCnt; ++uiIndex)
		{
			XMFLOAT4X4 matTrans;
			ZeroMemory(&matTrans, sizeof(XMFLOAT4X4));

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					matTrans.m[i][j] += pBoneWorld[pOriVertex[uiIndex].uiBones[0]].m[i][j] * pOriVertex[uiIndex].fWeights[0];
					matTrans.m[i][j] += pBoneWorld[pOriVertex[uiIndex].uiBones[1]].m[i][j] * pOriVertex[uiIndex].fWeights[1];
					matTrans.m[i][j] += pBoneWorld[pOriVertex[uiIndex].uiBones[2]].m[i][j] * pOriVertex[uiIndex].fWeights[2];
					matTrans.m[i][j] += pBoneWorld[pOriVertex[uiIndex].uiBones[3]].m[i][j] * pOriVertex[uiIndex].fWeights[3];
				}
			}

			XMStoreFloat3(&pChild->m_pVertex[uiIndex].vPos, XMVector3TransformCoord(XMLoadFloat3(&pOriVertex[uiIndex].vPos)
				, XMLoadFloat4x4(&matTrans) * XMLoadFloat4x4(pWorld)));
		}

		CCollision::GetInstance()->Add_ObbCheckList(*m_vecChild[iIndex]->Get_Min(), *m_vecChild[iIndex]->Get_Max(), matWorld
			, pChild->m_pVertex, pChild->m_pIndex, pChild->m_uiIdxCnt, pObj);

		pChild->Add_ObbCheckList(pWorld, pObj);
	}
}