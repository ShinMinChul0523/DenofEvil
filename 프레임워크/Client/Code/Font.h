#ifndef Font_h__
#define Font_h__

#include "Define.h"
#include "FW1FontWrapper.h"

class CFont
{
private:
	explicit CFont(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CFont(void);

public:
	HRESULT Ready_Font(const TCHAR* pFontName);
	void Render_Font(const TCHAR* pString, FLOAT fSize, FLOAT fPosX, FLOAT fPosY, UINT uiColor);

public:
	static CFont* Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const TCHAR* pFontName);

public:
	void Release(void);

private:
	ID3D11Device*			m_pGraphicDev;
	ID3D11DeviceContext*	m_pContext;
	IFW1Factory*			m_pFW1Factory;
	IFW1FontWrapper*		m_pFontWrapper;
};

#endif // Font_h__