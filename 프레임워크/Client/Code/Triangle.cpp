#include "stdafx.h"
#include "Triangle.h"
#include "Export.h"
#include "Transform.h"

CTriangle::CTriangle(ID3D11DeviceContext* pContext)
	: CGameObject(pContext)
	, m_pVB(nullptr)
	, m_pTransform(nullptr)
{
}

CTriangle::~CTriangle(void)
{
}

CTriangle* CTriangle::Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext,
	const XMFLOAT3& vPoint1, const XMFLOAT3& vPoint2, const XMFLOAT3& vPoint3)
{
	CTriangle* pTri = new CTriangle(pContext);

	if (FAILED(pTri->Initialize(pGraphicDev, vPoint1, vPoint2, vPoint3)))
		Safe_Release(pTri);

	return pTri;
}

HRESULT CTriangle::Initialize(ID3D11Device* pGraphicDev, const XMFLOAT3& vPoint1, const XMFLOAT3& vPoint2, const XMFLOAT3& vPoint3)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	// Vertex Buffer
	VTXTEX pVtxTex[] =
	{
		// Pos		TexUV				Normal
		{ vPoint1, XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) },
		{ vPoint2, XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) },
		{ vPoint3, XMFLOAT2(0.f, 0.f), XMFLOAT3(0.f, 0.f, 0.f) }
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

	HRESULT hr = pGraphicDev->CreateBuffer(&tBufferDesc, &tSubData, &m_pVB);

	if (FAILED(hr) == true)
	{
		MSG_BOX("NavMesh VB CreateBuffer Failed");
		return E_FAIL;
	}

	return S_OK;
}

INT CTriangle::Update(const FLOAT& fTimeDelta)
{
	CGameObject::Update(fTimeDelta);
	Add_RenderGroup(CRenderer::RENDER_ZSORT, this);

	return 0;
}

void CTriangle::Render(void)
{
	CGraphicDev::GetInstance()->Set_WireFrame(TRUE);

	m_pContext->IASetInputLayout(Get_InputLayout(L"Shader_Normal"));

	ID3D11Buffer* pBaseShaderCB = Get_BaseShaderCB();
	ID3D11SamplerState* pBaseSampler = Get_BaseSampler();

	BASESHADER_CB tBaseShaderCB;

	tBaseShaderCB.matWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_pTransform->m_matWorld));
	tBaseShaderCB.matView = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraView()));
	tBaseShaderCB.matProj = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraProj()));

	m_pContext->UpdateSubresource(pBaseShaderCB, 0, NULL, &tBaseShaderCB, 0, 0);

	m_pContext->VSSetShader(Get_VertexShader(L"Shader_Normal"), NULL, 0);
	m_pContext->VSSetConstantBuffers(0, 1, &pBaseShaderCB);
	m_pContext->PSSetShader(Get_PixelShader(L"Shader_Normal"), NULL, 0);
	m_pContext->PSSetSamplers(0, 1, &pBaseSampler);

	UINT uiStride = sizeof(VTXTEX);
	UINT uiOffset = 0;

	m_pContext->IASetVertexBuffers(0, 1, &m_pVB, &uiStride, &uiOffset);
	m_pContext->Draw(3, 0);

	CGraphicDev::GetInstance()->Set_WireFrame(FALSE);
}

void CTriangle::Release(void)
{
	CGameObject::Release();

	Safe_Release(m_pVB);
	delete this;
}

HRESULT CTriangle::Ready_Component(void)
{
	CComponent* pComponent = NULL;

	// Transform
	pComponent = CTransform::Create();
	m_pTransform = dynamic_cast<CTransform*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

