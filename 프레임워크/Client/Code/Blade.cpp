#include "stdafx.h"
#include "Blade.h"
#include "Export.h"
#include "Transform.h"

CBlade::CBlade(ID3D11DeviceContext* pContext)
	: CEquipment(pContext)
	, m_uiObjNum(0)
{
}

CBlade::~CBlade(void)
{
}

CBlade* CBlade::Create(ID3D11DeviceContext* pContext)
{
	CBlade* pEquipment = new CBlade(pContext);

	if (FAILED(pEquipment->Initialize()))
		Safe_Release(pEquipment);

	return pEquipment;
}

HRESULT CBlade::Initialize(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_uiObjNum = MESHNUM_BLADE;

	m_pTransform->m_vPos = XMFLOAT3(1.2f, -2.f, -0.5f);
	m_pTransform->m_vAngle = XMFLOAT3(ToRadian(35.f), 0.f, ToRadian(45.f));

	return S_OK;
}

INT CBlade::Update(const FLOAT& fTimeDelta)
{
	CGameObject::Update(fTimeDelta);
	XMStoreFloat4x4(&m_pTransform->m_matWorld, XMLoadFloat4x4(&m_pTransform->m_matWorld) * XMLoadFloat4x4(&m_matParent));

	Add_RenderGroup(CRenderer::RENDER_ZSORT, this);

	return 0;
}

void CBlade::Render(void)
{
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

	Render_MeshMgr(m_uiObjNum, FALSE);
}

void CBlade::Release(void)
{
	CEquipment::Release();
	delete this;
}

HRESULT CBlade::Ready_Component(void)
{
	CComponent* pComponent = NULL;

	// Transform
	pComponent = CTransform::Create();
	m_pTransform = dynamic_cast<CTransform*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

const XMFLOAT4X4* CBlade::Get_WorldPointer(void)
{
	return &m_pTransform->m_matWorld;
}
