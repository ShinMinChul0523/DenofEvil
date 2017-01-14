#include "stdafx.h"
#include "DefaultObj.h"
#include "Export.h"
#include "Transform.h"
#include "Export_System.h"

CDefaultObj::CDefaultObj(ID3D11DeviceContext* pContext)
	: CGameObject(pContext)
	, m_uiObjNum(0)
	, m_fRadius(0.f)
	, m_pTransform(NULL)
{
}

CDefaultObj::~CDefaultObj(void)
{
}

CDefaultObj* CDefaultObj::Create(ID3D11DeviceContext* pContext, const OBJDATA& tObjData)
{
	CDefaultObj* pObject = new CDefaultObj(pContext);

	if (FAILED(pObject->Initialize(tObjData)))
		Safe_Release(pObject);

	return pObject;
}

HRESULT CDefaultObj::Initialize(const OBJDATA& tObjData)
{
	if (FAILED(Ready_Component(tObjData)))
		return E_FAIL;

	m_uiObjNum = tObjData.uiImgNum + MESHNUM_END;

	m_pTransform->m_vScale = XMFLOAT3(tObjData.vScale);
	m_pTransform->m_vAngle = XMFLOAT3(tObjData.vAngle);
	m_pTransform->m_vPos = XMFLOAT3(tObjData.vPos);

	XMFLOAT3 vMax = *Get_MeshMax(m_uiObjNum);

	vMax = XMFLOAT3(vMax.x * m_pTransform->m_vScale.x, 
					vMax.y * m_pTransform->m_vScale.y,
					vMax.z * m_pTransform->m_vScale.z);

	m_fRadius = (vMax.x > vMax.y) ? vMax.x : vMax.y;
	m_fRadius = (vMax.z > m_fRadius) ? vMax.z : m_fRadius;

	return S_OK;
}

INT CDefaultObj::Update(const FLOAT& fTimeDelta)
{
	CGameObject::Update(fTimeDelta);

	if (Sphere_InFrustum(&m_pTransform->m_vPos, m_fRadius) == TRUE)
	{
		if (Get_MeshUseAlpha(m_uiObjNum))
		{
			XMFLOAT4X4 matViewWorld;
			XMStoreFloat4x4(&matViewWorld, XMLoadFloat4x4(&m_pTransform->m_matWorld) * XMLoadFloat4x4(Get_CurCameraView()));

			Add_RenderGroup(CRenderer::RENDER_ALPHA, this, matViewWorld._43);
		}

		else
			Add_RenderInstGroup(CRenderer::RENDER_INST, m_uiObjNum, &m_pTransform->m_matWorld);
	}

	return 0;
}

void CDefaultObj::Render(void)
{
	m_pContext->IASetInputLayout(Get_InputLayout(L"Shader_Default"));

	ID3D11Buffer* pBaseShaderCB = Get_BaseShaderCB();
	ID3D11SamplerState* pBaseSampler = Get_BaseSampler();

	BASESHADER_CB tBaseShaderCB;

	tBaseShaderCB.matWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_pTransform->m_matWorld));
	tBaseShaderCB.matView = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraView()));
	tBaseShaderCB.matProj = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraProj()));

	m_pContext->UpdateSubresource(pBaseShaderCB, 0, NULL, &tBaseShaderCB, 0, 0);

	m_pContext->VSSetShader(Get_VertexShader(L"Shader_Default"), NULL, 0);
	m_pContext->VSSetConstantBuffers(0, 1, &pBaseShaderCB);
	m_pContext->PSSetShader(Get_PixelShader(L"Shader_Default"), NULL, 0);
	m_pContext->PSSetSamplers(0, 1, &pBaseSampler);

	Render_MeshMgr(m_uiObjNum, FALSE);
}

void CDefaultObj::Release(void)
{
	CGameObject::Release();
	delete this;
}

HRESULT CDefaultObj::Ready_Component(const OBJDATA& tObjData)
{
	CComponent* pComponent = NULL;

	// Transform
	pComponent = CTransform::Create();
	m_pTransform = dynamic_cast<CTransform*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}
