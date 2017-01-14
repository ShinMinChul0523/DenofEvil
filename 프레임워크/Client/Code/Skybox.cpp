#include "stdafx.h"
#include "Skybox.h"
#include "Export.h"
#include "Transform.h"

CSkybox::CSkybox(ID3D11DeviceContext* pContext)
	: CGameObject(pContext)
	, m_pTransform(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
{
}

CSkybox::~CSkybox(void)
{
}

CSkybox* CSkybox::Create(ID3D11DeviceContext* pContext)
{
	CSkybox* pSkybox = new CSkybox(pContext);

	if (FAILED(pSkybox->Initialize()))
		Safe_Release(pSkybox);

	return pSkybox;
}

HRESULT CSkybox::Initialize(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransform->m_vScale = XMFLOAT3(150.f, 150.f, 150.f);

	return S_OK;
}

INT CSkybox::Update(const FLOAT& fTimeDelta)
{
	CGameObject::Update(fTimeDelta);
	Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	return 0;
}

void CSkybox::Render(void)
{
	m_pContext->IASetInputLayout(Get_InputLayout(L"Shader_Skybox"));

	ID3D11Buffer* pBaseShaderCB = Get_BaseShaderCB();
	ID3D11SamplerState* pBaseSampler = Get_BaseSampler();

	XMFLOAT4X4 pCameraView = (*Get_CurCameraView());
	ZeroMemory(pCameraView.m[3], sizeof(FLOAT) * 3);

	BASESHADER_CB tBaseShaderCB;

	tBaseShaderCB.matWorld = XMMatrixTranspose(XMLoadFloat4x4(&(m_pTransform->m_matWorld)));
	tBaseShaderCB.matView = XMMatrixTranspose(XMLoadFloat4x4(&pCameraView));
	tBaseShaderCB.matProj = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraProj()));

	m_pContext->UpdateSubresource(pBaseShaderCB, 0, NULL, &tBaseShaderCB, 0, 0);

	m_pContext->VSSetShader(Get_VertexShader(L"Shader_Skybox"), NULL, 0);
	m_pContext->VSSetConstantBuffers(0, 1, &pBaseShaderCB);
	m_pContext->PSSetShader(Get_PixelShader(L"Shader_Skybox"), NULL, 0);
	m_pContext->PSSetSamplers(0, 1, &pBaseSampler);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render();
}

void CSkybox::Release(void)
{
	CGameObject::Release();
	delete this;
}

HRESULT CSkybox::Ready_Component(void)
{
	CComponent* pComponent = NULL;

	// Transform
	pComponent = CTransform::Create();
	m_pTransform = dynamic_cast<CTransform*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// Buffer
	pComponent = Clone_ResourceMgr(RESOURCE_STAGE, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<CCubeTex*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// Texture
	pComponent = Clone_ResourceMgr(RESOURCE_STAGE, L"Texture_Skybox");
	m_pTexture = dynamic_cast<CTexture*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	return S_OK;
}

