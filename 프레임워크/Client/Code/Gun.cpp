#include "stdafx.h"
#include "Gun.h"
#include "Export.h"
#include "Transform.h"

CGun::CGun(ID3D11DeviceContext* pContext)
	: CEquipment(pContext)
	, m_uiObjNum(0)
{
}

CGun::~CGun(void)
{
}

CGun* CGun::Create(ID3D11DeviceContext* pContext, BOOL bReverse)
{
	CGun* pEquipment = new CGun(pContext);

	if (FAILED(pEquipment->Initialize(bReverse)))
		Safe_Release(pEquipment);

	return pEquipment;
}

HRESULT CGun::Initialize(BOOL bReverse)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_uiObjNum = MESHNUM_GUN;

	if (bReverse == FALSE)
	{
		m_pTransform->m_vPos = XMFLOAT3(2.f, 6.f, 1.f);
		m_pTransform->m_vAngle = XMFLOAT3(ToRadian(60.f), 0.f, ToRadian(55.f));
	}

	else
	{
		m_pTransform->m_vPos = XMFLOAT3(6.f, -5.5f, 1.f);
		m_pTransform->m_vAngle = XMFLOAT3(ToRadian(60.f), 0.f, -ToRadian(55.f));
	}

	return S_OK;
}

INT CGun::Update(const FLOAT& fTimeDelta)
{
	CGameObject::Update(fTimeDelta);
	XMStoreFloat4x4(&m_pTransform->m_matWorld, XMLoadFloat4x4(&m_pTransform->m_matWorld) * XMLoadFloat4x4(&m_matParent));

	Add_RenderGroup(CRenderer::RENDER_ZSORT, this);

	return 0;
}

void CGun::Render(void)
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

void CGun::Release(void)
{
	CEquipment::Release();
	delete this;
}

HRESULT CGun::Ready_Component(void)
{
	CComponent* pComponent = NULL;

	// Transform
	pComponent = CTransform::Create();
	m_pTransform = dynamic_cast<CTransform*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}