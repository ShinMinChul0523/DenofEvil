#include "stdafx.h"
#include "FontMgr.h"
#include "Font.h"

IMPLEMENT_SINGLETON(CFontMgr)

CFontMgr::CFontMgr(void)
{
}

CFontMgr::~CFontMgr(void)
{

}

HRESULT CFontMgr::Ready_Font(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const TCHAR* pFontName)
{
	MAPFONT::iterator	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTagFinder(pFontName));

	if (iter != m_mapFont.end())
		return E_FAIL;

	CFont* pFont = CFont::Create(pGraphicDev, pContext, pFontName);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_mapFont.insert(MAPFONT::value_type(pFontName, pFont));

	return S_OK;
}

void CFontMgr::Render_Font(const TCHAR* pFontName, const TCHAR* pString, FLOAT fSize, FLOAT fPosX, FLOAT fPosY, UINT uiColor)
{
	MAPFONT::iterator	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTagFinder(pFontName));

	if (iter == m_mapFont.end())
		return;

	iter->second->Render_Font(pString, fSize, fPosX, fPosY, uiColor);
}

void CFontMgr::Release(void)
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CRelease_Pair());
	m_mapFont.clear();

	delete this;
}

