#include "stdafx.h"
#include "Terrain.h"
#include "Export.h"

CTerrain::CTerrain(ID3D11DeviceContext* pContext)
	: CGameObject(pContext)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
{
}

CTerrain::~CTerrain(void)
{
}

CTerrain* CTerrain::Create(ID3D11DeviceContext* pContext)
{
	CTerrain* pTerrain = new CTerrain(pContext);

	if (FAILED(pTerrain->Initialize()))
		Safe_Release(pTerrain);

	return pTerrain;
}

HRESULT CTerrain::Initialize(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

INT CTerrain::Update(const FLOAT& fTimeDelta)
{
	CGameObject::Update(fTimeDelta);
	Add_RenderGroup(CRenderer::RENDER_ZSORT, this);

	return 0;
}

void CTerrain::Render(void)
{
	m_pContext->IASetInputLayout(Get_InputLayout(L"Shader_Terrain"));

	ID3D11Buffer* pBaseShaderCB = Get_BaseShaderCB();
	ID3D11SamplerState* pBaseSampler = Get_BaseSampler();

	BASESHADER_CB tBaseShaderCB;

	tBaseShaderCB.matWorld = XMMatrixTranspose(XMMatrixIdentity());
	tBaseShaderCB.matView = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraView()));
	tBaseShaderCB.matProj = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraProj()));

	m_pContext->UpdateSubresource(pBaseShaderCB, 0, NULL, &tBaseShaderCB, 0, 0);

	m_pContext->VSSetShader(Get_VertexShader(L"Shader_Terrain"), NULL, 0);
	m_pContext->VSSetConstantBuffers(0, 1, &pBaseShaderCB);
	m_pContext->PSSetShader(Get_PixelShader(L"Shader_Terrain"), NULL, 0);
	m_pContext->PSSetSamplers(0, 1, &pBaseSampler);

	for (UINT uiSlot = 0; uiSlot < 5; ++uiSlot)
		m_pTexture->Render(uiSlot, uiSlot);

	m_pBuffer->Render();
}

void CTerrain::Release(void)
{
	CGameObject::Release();
	delete this;
}

HRESULT CTerrain::Ready_Component(void)
{
	CComponent* pComponent = NULL;

	//Buffer
	pComponent = Clone_ResourceMgr(RESOURCE_STAGE, L"Buffer_TerrainTex");
	m_pBuffer = dynamic_cast<CTerrainTex*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	//Texture
	pComponent = Clone_ResourceMgr(RESOURCE_STAGE, L"Texture_Terrain");
	m_pTexture = dynamic_cast<CTexture*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	return S_OK;
}