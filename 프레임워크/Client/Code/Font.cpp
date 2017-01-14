#include "stdafx.h"
#include "Font.h"

CFont::CFont(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
	: m_pGraphicDev(pGraphicDev)
	, m_pContext(pContext)
	, m_pFW1Factory(NULL)
	, m_pFontWrapper(NULL)
{
}

CFont::~CFont(void)
{
}

HRESULT CFont::Ready_Font(const TCHAR* pFontName)
{
	FAILED_CHECK(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory));
	FAILED_CHECK(m_pFW1Factory->CreateFontWrapper(m_pGraphicDev, pFontName, &m_pFontWrapper));

	return S_OK;
}

void CFont::Render_Font(const TCHAR* pString, FLOAT fSize, FLOAT fPosX, FLOAT fPosY, UINT uiColor)
{
	m_pFontWrapper->DrawString(m_pContext, pString,	fSize, fPosX, fPosY, uiColor, 0);
}

CFont* CFont::Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const TCHAR* pFontName)
{
	CFont* pFont = new CFont(pGraphicDev, pContext);

	if (FAILED(pFont->Ready_Font(pFontName)))
	{
		MSG_BOX("CFont Create Failed");
		Safe_Release(pFont);
	}

	return pFont;
}

void CFont::Release(void)
{
	m_pFontWrapper->Release();
	m_pFW1Factory->Release();

	delete this;
}
