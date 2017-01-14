#pragma once

#include "Define.h"

class CRenderTexture;
class CRenderTargetMgr
{
	DECLARE_SINGLETON(CRenderTargetMgr)

private:
	explicit CRenderTargetMgr(void);
	~CRenderTargetMgr(void);

public:
	ID3D11RenderTargetView** Get_RanderTargetView(const TCHAR* pRenderTargetTag);
	ID3D11DepthStencilView* Get_DepthStencilView(const TCHAR* pRenderTargetTag);
	void Set_RenderTarget(const TCHAR* pRenderTargetTag, const UINT& uiNumViews, BOOL bDepthStencilView);
	void Set_Texture(const TCHAR* pRenderTargetTag, const UINT& uiSlot, const UINT uiIndex);

public:
	HRESULT Ready_RenderTarget(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const TCHAR* pRenderTargetTag
								, const DXGI_FORMAT& eFormat, const WORD& wSizeX, const WORD& wSizeY, FLOAT fRenderPosX, FLOAT fRenderPosY);
	void Render(const TCHAR* pRenderTargetTag);
	void Release(void);

private:
	CRenderTexture* Find_RenderTarget(const TCHAR* pRenderTargetTag);

private:
	typedef map<const TCHAR*, CRenderTexture*>	MAPRENDERTARGETS;
	MAPRENDERTARGETS	m_mapRenderTarget;
};