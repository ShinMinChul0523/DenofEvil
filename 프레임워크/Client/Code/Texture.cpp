#include "stdafx.h"
#include "Texture.h"
#include <DXGIFormat.h>
#include "DirectXTex.h"

using namespace DirectX;

CTexture::CTexture(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
	: CResource(pGraphicDev, pContext)
	, m_sizetContainerSize(0)
{
}

CTexture::~CTexture(void)
{
}

CTexture* CTexture::Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt)
{
	CTexture* pTexture = new CTexture(pGraphicDev, pContext);

	if (FAILED(pTexture->Create_Texture(eTextureType, pFilePath, wCnt)))
	{
		MSG_BOX("Texture Ready_Texture Failed");
		Safe_Release(pTexture);
	}

	return pTexture;
}

CTexture* CTexture::Clone_Resource(void)
{
	CTexture* pTexture = new CTexture(*this);

	++(*m_pwCloneCnt);

	return pTexture;
}

HRESULT CTexture::Create_Texture(TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt)
{
	ID3D11ShaderResourceView* pShaderResourceView = NULL;
	TCHAR szFullPath[MAX_PATH] = L"";
	HRESULT hr = E_FAIL;

	ScratchImage image;
	TexMetadata info;

	size_t iTextureCnt = 1;
	if (wCnt > 0) iTextureCnt = wCnt;

	m_vecTexture.reserve(iTextureCnt);

	for (size_t i = 0; i < iTextureCnt; ++i)
	{
		if(wCnt == 0)	wsprintf(szFullPath, pFilePath);
		else			wsprintf(szFullPath, pFilePath, i);

		switch (eTextureType)
		{
		case TYPE_NORMAL:
			hr = D3DX11CreateShaderResourceViewFromFile(m_pGraphicDev, szFullPath
				, NULL, NULL, &pShaderResourceView, NULL);
			break;

		case TYPE_TGA:
			hr = LoadFromTGAFile(szFullPath, &info, image);
			if (FAILED(hr))	break;

			hr = CreateShaderResourceView(m_pGraphicDev, image.GetImages(), image.GetImageCount(), info, &pShaderResourceView);
			break;

		case TYPE_DDSCUBE:
			hr = LoadFromDDSFile(szFullPath, DDS_FLAGS_NONE, &info, image);
			if (FAILED(hr))	break;

			info.miscFlags &= ~TEX_MISC_TEXTURECUBE;

			hr = CreateShaderResourceView(m_pGraphicDev, image.GetImages(), image.GetImageCount(), info, &pShaderResourceView);
			break;
		}

		if (FAILED(hr) == TRUE)
			return E_FAIL;

		m_vecTexture.push_back(pShaderResourceView);
	}

	m_sizetContainerSize = m_vecTexture.size();

	return S_OK;
}

void CTexture::Render(const UINT uiSlot, const DWORD& dwIndex)
{
	m_pContext->PSSetShaderResources(uiSlot, 1, &m_vecTexture[dwIndex]);
}

void CTexture::Release(void)
{
	CResource::Release();

	if (m_pwCloneCnt == NULL)
	{
		size_t dwSize = m_vecTexture.size();

		for(size_t i = 0; i < dwSize; ++i)
			Safe_Release(m_vecTexture[i]);

		m_vecTexture.clear();
	}

	delete this;
}
