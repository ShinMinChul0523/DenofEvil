#include "stdafx.h"
#include "RenderTargetMgr.h"
#include "RenderTarget.h"

IMPLEMENT_SINGLETON(CRenderTargetMgr)

CRenderTargetMgr::CRenderTargetMgr(void)
{
}

CRenderTargetMgr::~CRenderTargetMgr(void)
{
}

ID3D11RenderTargetView** CRenderTargetMgr::Get_RanderTargetView(const TCHAR* pRenderTargetTag)
{
	CRenderTexture* pRenderTarget = Find_RenderTarget(pRenderTargetTag);

	if (NULL == pRenderTarget)
		return NULL;

	return pRenderTarget->Get_RanderTargetView();
}

ID3D11DepthStencilView* CRenderTargetMgr::Get_DepthStencilView(const TCHAR* pRenderTargetTag)
{
	CRenderTexture* pRenderTarget = Find_RenderTarget(pRenderTargetTag);

	if (NULL == pRenderTarget)
		return NULL;

	return pRenderTarget->Get_DepthStencilView();
}

void CRenderTargetMgr::Set_RenderTarget(const TCHAR* pRenderTargetTag, const UINT& uiNumViews, BOOL bDepthStencilView)
{
	CRenderTexture* pRenderTarget = Find_RenderTarget(pRenderTargetTag);

	if (NULL == pRenderTarget)
		return;

	pRenderTarget->Set_RenderTarget(uiNumViews, bDepthStencilView);
}

void CRenderTargetMgr::Set_Texture(const TCHAR* pRenderTargetTag, const UINT& uiSlot, const UINT uiIndex)
{
	CRenderTexture* pRenderTarget = Find_RenderTarget(pRenderTargetTag);

	if (NULL == pRenderTarget)
		return;

	pRenderTarget->Set_Texture(uiSlot, uiIndex);
}

HRESULT CRenderTargetMgr::Ready_RenderTarget(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const TCHAR* pRenderTargetTag
												, const DXGI_FORMAT& eFormat, const WORD& wSizeX, const WORD& wSizeY, FLOAT fRenderPosX, FLOAT fRenderPosY)
{
	CRenderTexture* pRenderTarget = Find_RenderTarget(pRenderTargetTag);

	if (NULL != pRenderTarget)
	{
		MSG_BOX("Already RenderTarget Failed");
		return E_FAIL;
	}

	pRenderTarget = CRenderTexture::Create(pGraphicDev, pContext, eFormat, wSizeX, wSizeY, fRenderPosX, fRenderPosY);

	if (NULL == pRenderTarget)
		return E_FAIL;

	m_mapRenderTarget.insert(MAPRENDERTARGETS::value_type(pRenderTargetTag, pRenderTarget));

	return S_OK;
}

void CRenderTargetMgr::Render(const TCHAR* pRenderTargetTag)
{
	CRenderTexture* pRenderTarget = Find_RenderTarget(pRenderTargetTag);

	if (NULL == pRenderTarget)
		return;

	pRenderTarget->Render();
}

void CRenderTargetMgr::Release(void)
{
	for_each(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CRelease_Pair());
	m_mapRenderTarget.clear();

	delete this;
}

CRenderTexture* CRenderTargetMgr::Find_RenderTarget(const TCHAR* pRenderTargetTag)
{
	MAPRENDERTARGETS::iterator iter = find_if(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CTagFinder(pRenderTargetTag));

	if (iter == m_mapRenderTarget.end())
		return NULL;

	return iter->second;
}