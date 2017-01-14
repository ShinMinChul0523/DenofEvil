#include "stdafx.h"
#include "BoundingBox.h"
#include "Transform.h"
#include "CubeTex.h"
#include "Export.h"

CBoundingBox::CBoundingBox(ID3D11DeviceContext* pContext)
	: CGameObject(pContext)
	, m_pBuffer(NULL)
	, m_pTransform(NULL)
{
}

CBoundingBox::~CBoundingBox(void)
{
}

CBoundingBox* CBoundingBox::Create(ID3D11DeviceContext* pContext, const XMFLOAT3& vMin, const XMFLOAT3& vMax, const XMFLOAT4X4& matParent)
{
	CBoundingBox* pBox = new CBoundingBox(pContext);

	if (FAILED(pBox->Initialize(vMin, vMax, matParent)))
		Safe_Release(pBox);

	return pBox;
}

HRESULT CBoundingBox::Initialize(const XMFLOAT3& vMin, const XMFLOAT3& vMax, const XMFLOAT4X4& matParent)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_matParent = matParent;
	m_pTransform->m_vPos = XMFLOAT3((vMin.x + vMax.x) * 0.5f, (vMin.y + vMax.y) * 0.5f, (vMin.z + vMax.z) * 0.5f);
	m_pTransform->m_vScale = XMFLOAT3((vMax.x - vMin.x) * 0.5f, (vMax.y - vMin.y) * 0.5f, (vMax.z - vMin.z) * 0.5f);

	return S_OK;
}

INT CBoundingBox::Update(const FLOAT& fTimeDelta)
{
	CGameObject::Update(fTimeDelta);
	XMStoreFloat4x4(&m_pTransform->m_matWorld, XMLoadFloat4x4(&m_pTransform->m_matWorld) * XMLoadFloat4x4(&m_matParent));

	Add_RenderGroup(CRenderer::RENDER_ZSORT, this);

	return 0;
}

void CBoundingBox::Render(void)
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

	m_pBuffer->Render();

	CGraphicDev::GetInstance()->Set_WireFrame(FALSE);
}

void CBoundingBox::Release(void)
{
	CGameObject::Release();
	delete this;
}

HRESULT CBoundingBox::Ready_Component(void)
{
	CComponent* pComponent = NULL;

	// Buffer
	pComponent = Clone_ResourceMgr(RESOURCE_STAGE, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<CCubeTex*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// Transform
	pComponent = CTransform::Create();
	m_pTransform = dynamic_cast<CTransform*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

