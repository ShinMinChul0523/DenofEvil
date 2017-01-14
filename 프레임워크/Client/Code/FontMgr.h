#ifndef FontMgr_h__
#define FontMgr_h__

#include "Define.h"

class CFont;
class CFontMgr
{
	DECLARE_SINGLETON(CFontMgr)

private:
	CFontMgr(void);
	~CFontMgr(void);

public:
	HRESULT Ready_Font(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const TCHAR* pFontName);
	void Render_Font(const TCHAR* pFontName, const TCHAR* pString, FLOAT fSize, FLOAT fPosX, FLOAT fPosY, UINT uiColor);

public:
	void Release(void);

private:
	using MAPFONT = map<const TCHAR*, CFont*>;
	MAPFONT m_mapFont;
};

#endif // FontMgr_h__