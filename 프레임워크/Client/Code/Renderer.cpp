#include "stdafx.h"
#include "Renderer.h"
#include "Export.h"
#include "GameObject.h"
//#include "FontMgr.h"
#include "NavMesh.h"
#include "RcTex.h"

CRenderer::CRenderer(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
	: m_pGraphicDev(pGraphicDev)
	, m_pContext(pContext)
	, m_pSwapChain(NULL)
	, m_pRenderTargetView(NULL)
	, m_pDepthStencilView(NULL)
	, m_bDrawNavMesh(FALSE)
	, m_bDrawRenderTarget(FALSE)
	, m_pRcTex(NULL)
{
}

CRenderer::~CRenderer(void)
{
}

void CRenderer::Add_RenderGroup(RENDERTYPE eType, CGameObject* pGameObject, FLOAT fViewZ /*= 0*/)
{
	if (eType < RENDER_END)
		m_RenderGroup[eType].push_back(pGameObject);

	if (eType == RENDER_ALPHA)
		m_mapAlpha.insert(MAPALPHA::value_type(fViewZ, pGameObject));
}

void CRenderer::Add_RenderInstGroup(RENDERTYPE eType, UINT uiObjNum, XMFLOAT4X4* pMatWorld)
{
	if (eType != RENDER_INST && eType != RENDER_ALPHAINST)
		return;

	MAPINST* pMapInst = nullptr;

	if (eType == RENDER_INST)
		pMapInst = &m_mapInst;

	else if (eType == RENDER_ALPHAINST)
		pMapInst = &m_mapAlphaInst;

	MAPINST::iterator iter = find_if(pMapInst->begin(), pMapInst->end(), CNumFinder(uiObjNum));

	if (iter == pMapInst->end())
	{
		VECWORLD vecObjWorld;
		vecObjWorld.reserve(INSTCNT);
		vecObjWorld.push_back(pMatWorld);
		pMapInst->insert(MAPINST::value_type(uiObjNum, vecObjWorld));
	}

	else
		iter->second.push_back(pMatWorld);
}

HRESULT CRenderer::Ready_Renderer(void)
{
	m_pSwapChain = Get_SwapChain();
	m_pRenderTargetView = Get_RenderTargetView();
	m_pDepthStencilView = Get_DepthStencilView();

	m_pRcTex = dynamic_cast<CRcTex*>(Clone_ResourceMgr(RESOURCE_STAGE, L"Buffer_RcTex"));

	// Shader
	if (FAILED(Ready_ShaderFromFiles(m_pGraphicDev, m_pContext, L"Shader_Default", L"../bin/Resource/Shader/Shader_Default.fx", 0)))
		return E_FAIL;

	if (FAILED(Ready_ShaderFromFiles(m_pGraphicDev, m_pContext, L"Shader_Normal", L"../bin/Resource/Shader/Shader_Normal.fx", 0)))
		return E_FAIL;

	if (FAILED(Ready_ShaderFromFiles(m_pGraphicDev, m_pContext, L"Shader_Skybox", L"../bin/Resource/Shader/Shader_Skybox.fx", 0)))
		return E_FAIL;

	if (FAILED(Ready_ShaderFromFiles(m_pGraphicDev, m_pContext, L"Shader_Terrain", L"../bin/Resource/Shader/Shader_Terrain.fx", 0)))
		return E_FAIL;

	if (FAILED(Ready_ShaderFromFiles(m_pGraphicDev, m_pContext, L"Shader_Instancing", L"../bin/Resource/Shader/Shader_Instancing.fx", 0)))
		return E_FAIL;

	if (FAILED(Ready_ShaderFromFiles(m_pGraphicDev, m_pContext, L"Shader_DynamicMesh", L"../bin/Resource/Shader/Shader_DynamicMesh.fx", CShaderMgr::LAYOUT_FLAG_BONE)))
		return E_FAIL;

	if (FAILED(Ready_ShaderFromFiles(m_pGraphicDev, m_pContext, L"Shader_FogResult", L"../bin/Resource/Shader/Shader_FogResult.fx", 0)))
		return E_FAIL;

	return S_OK;
}

void CRenderer::Render(void)
{
	m_pContext->ClearRenderTargetView((Get_RanderTargetViewInMgr(L"RT_Blend"))[0], D3DXCOLOR(0.f, 0.f, 0.5f, 0.f));
	m_pContext->ClearRenderTargetView((Get_RanderTargetViewInMgr(L"RT_Blend"))[1], D3DXCOLOR(1.f, 1.f, 1.f, 0.f));
	m_pContext->ClearDepthStencilView(Get_DepthStencilViewInMgr(L"RT_Blend"), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	m_pContext->ClearRenderTargetView(m_pRenderTargetView, D3DXCOLOR(0.f, 0.f, 0.5f, 0.f));
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	Render_Priority();

	Render_Inst();
	Render_ZSort();

	// Temp
	if (Get_InputKeyDown(DIK_0)) m_bDrawNavMesh = !m_bDrawNavMesh;		// 네비메시 draw on off
	if (m_bDrawNavMesh)	CNavMesh::GetInstance()->Render_NavMesh();

	//Render_AlphaInst();
	Render_Alpha();

	Render_UI();

	// Render Window
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	if (Get_InputKeyDown(DIK_8)) m_bDrawRenderTarget = !m_bDrawRenderTarget;
	if (m_bDrawRenderTarget) Render_RenderTarget(L"RT_Blend");

	m_pContext->IASetInputLayout(Get_InputLayout(L"Shader_FogResult"));

	ID3D11Buffer* pBaseShaderCB = Get_BaseShaderCB();
	ID3D11SamplerState* pBaseSampler = Get_BaseSampler();

	BASESHADER_CB tBaseShaderCB;

	tBaseShaderCB.matWorld = XMMatrixTranspose(XMMatrixScaling(2.f, 2.f, 2.f));
	tBaseShaderCB.matView = XMMatrixTranspose(XMMatrixIdentity());
	tBaseShaderCB.matProj = XMMatrixTranspose(XMMatrixIdentity());

	m_pContext->UpdateSubresource(pBaseShaderCB, 0, NULL, &tBaseShaderCB, 0, 0);

	m_pContext->VSSetShader(Get_VertexShader(L"Shader_FogResult"), NULL, 0);
	m_pContext->VSSetConstantBuffers(0, 1, &pBaseShaderCB);
	m_pContext->PSSetShader(Get_PixelShader(L"Shader_FogResult"), NULL, 0);
	m_pContext->PSSetSamplers(0, 1, &pBaseSampler);

	for (UINT uiSlot = 0; uiSlot < 2; ++uiSlot)
		Set_Texture(L"RT_Blend", uiSlot, uiSlot);

	m_pRcTex->Render();

	m_pSwapChain->Present(0, 0);

	Clear_RenderGroup();
}

void CRenderer::Clear_RenderGroup(void)
{
	for (INT i = 0; i < RENDER_END; ++i)
	{
		m_RenderGroup[i].clear();
	}

	m_mapAlpha.clear();

	MAPINST::iterator	iter = m_mapAlphaInst.begin();
	MAPINST::iterator	iter_end = m_mapAlphaInst.end();

	for (; iter != iter_end; ++iter)
		iter->second.clear();

	iter = m_mapInst.begin();
	iter_end = m_mapInst.end();

	for (; iter != iter_end; ++iter)
		iter->second.clear();

	m_mapInst.clear();
}

void CRenderer::Render_Priority(void)
{
	Set_RenderTarget(L"RT_Blend", 2, FALSE);

	RENDERLIST::iterator iter = m_RenderGroup[RENDER_PRIORITY].begin();
	RENDERLIST::iterator iter_end = m_RenderGroup[RENDER_PRIORITY].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}

void CRenderer::Render_Inst(void)
{
	Set_RenderTarget(L"RT_Blend", 2, TRUE);

	MAPINST::iterator	iter = m_mapInst.begin();
	MAPINST::iterator	iter_end = m_mapInst.end();

	for (; iter != iter_end; ++iter)
	{
		RenderInst_MeshMgr(iter->first, iter->second);
	}
}

void CRenderer::Render_ZSort(void)
{
	Set_RenderTarget(L"RT_Blend", 2, TRUE);

	RENDERLIST::iterator iter = m_RenderGroup[RENDER_ZSORT].begin();
	RENDERLIST::iterator iter_end = m_RenderGroup[RENDER_ZSORT].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}

void CRenderer::Render_AlphaInst(void)
{
	//m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	MAPINST::iterator	iter = m_mapAlphaInst.begin();
	MAPINST::iterator	iter_end = m_mapAlphaInst.end();

	for (; iter != iter_end; ++iter)
	{
		sort(iter->second.begin(), iter->second.end()
			, [](const XMFLOAT4X4* m1, const XMFLOAT4X4* m2) 
		{
			XMMATRIX matView = XMLoadFloat4x4(Get_CurCameraView());
			XMFLOAT4X4 matViewWorld1, matViewWorld2;

			XMStoreFloat4x4(&matViewWorld1, XMLoadFloat4x4(m1) * matView);
			XMStoreFloat4x4(&matViewWorld2, XMLoadFloat4x4(m2) * matView);

			return matViewWorld1._43 > matViewWorld2._43;
		});

		RenderInst_MeshMgr(iter->first, iter->second);
	}

	Set_AlphaEnable(FALSE);
}

void CRenderer::Render_Alpha(void)
{
	Set_RenderTarget(L"RT_Blend", 2, TRUE);
	Set_AlphaEnable(TRUE);

	MAPALPHA::iterator	iter = m_mapAlpha.begin();
	MAPALPHA::iterator	iter_end = m_mapAlpha.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Render();
	}

	Set_AlphaEnable(FALSE);
}

void CRenderer::Render_UI(void)
{
	RENDERLIST::iterator iter = m_RenderGroup[RENDER_UI].begin();
	RENDERLIST::iterator iter_end = m_RenderGroup[RENDER_UI].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}

CRenderer* CRenderer::Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
{
	CRenderer* pRenderer = new CRenderer(pGraphicDev, pContext);

	if (FAILED(pRenderer->Ready_Renderer()))
	{
		Safe_Release(pRenderer);
	}

	return pRenderer;
}

void CRenderer::Release(void)
{
	Safe_Release(m_pRcTex);
	delete this;
}