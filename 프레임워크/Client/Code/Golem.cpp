#include "stdafx.h"
#include "Golem.h"
#include "Export.h"
#include "Transform.h"
#include "AnimationInfo.h"

CGolem::CGolem(ID3D11DeviceContext* pContext)
	: CMonster(pContext)
{
}

CGolem::~CGolem(void)
{
}

CGolem* CGolem::Create(ID3D11DeviceContext* pContext)
{
	CGolem* pObject = new CGolem(pContext);

	if (FAILED(pObject->Initialize()))
		Safe_Release(pObject);

	return pObject;
}

HRESULT CGolem::Initialize(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pAnimInfo->Set_Key(MONSTER_RUN);

	m_fSpeed = 4.f;
	m_fHeight = 12.f;
	m_uiObjNum = MESHNUM_GOLEM;

	FLOAT fScale = (1.f / Get_MeshMax(m_uiObjNum)->z) * m_fHeight; 
	m_pTransform->m_vScale = XMFLOAT3(fScale, fScale, fScale);

	m_pTransform->m_vAngle.x = ToRadian(270.f);

	m_pTransform->m_vPos = XMFLOAT3(783.f, 0.f, 603.f);
	m_pTransform->m_vDir = XMFLOAT3(0.f, 0.f, 1.f);
	
	return S_OK;
}

INT CGolem::Update(const FLOAT& fTimeDelta)
{
	m_pTransform->m_vPos.y = Get_TerrainColY(m_pTransform->m_vPos) + 0.5f;

	CMonster::Update(fTimeDelta);
	Add_RenderGroup(CRenderer::RENDER_ZSORT, this);
	Add_ObbCheckList(m_uiObjNum, &m_pTransform->m_matWorld, this);

	return 0;
}

void CGolem::Render(void)
{
	m_pContext->IASetInputLayout(Get_InputLayout(L"Shader_DynamicMesh"));

	ID3D11Buffer* pBaseShaderCB = Get_BaseShaderCB();
	ID3D11Buffer* pDynamicShaderCB = Get_DynamicShaderCB();

	ID3D11SamplerState* pBaseSampler = Get_BaseSampler();

	BASESHADER_CB tBaseShaderCB;

	tBaseShaderCB.matWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_pTransform->m_matWorld));
	tBaseShaderCB.matView = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraView()));
	tBaseShaderCB.matProj = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraProj()));

	m_pContext->UpdateSubresource(pBaseShaderCB, 0, NULL, &tBaseShaderCB, 0, 0);

	m_pContext->VSSetShader(Get_VertexShader(L"Shader_DynamicMesh"), NULL, 0);
	m_pContext->VSSetConstantBuffers(0, 1, &pBaseShaderCB);
	m_pContext->VSSetConstantBuffers(1, 1, &pDynamicShaderCB);
	m_pContext->PSSetShader(Get_PixelShader(L"Shader_DynamicMesh"), NULL, 0);
	m_pContext->PSSetSamplers(0, 1, &pBaseSampler);

	RenderAnim_MeshMgr(m_uiObjNum, m_pAnimInfo, FALSE);
}

void CGolem::Attacked(void)
{
	m_pAnimInfo->Set_Key(MONSTER_DIE);
}

void CGolem::Release(void)
{
	CMonster::Release();
	delete this;
}

HRESULT CGolem::Ready_Component(void)
{
	if (FAILED(CMonster::Ready_Component()))
		return E_FAIL;

	return S_OK;
}
