#ifndef Shader_h__
#define Shader_h__

#include "Define.h"

class CShader
{
private:
	explicit CShader(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CShader(void);

public:
	ID3D11VertexShader* Get_VertexShader(void);
	ID3D11InputLayout* Get_InputLayout(void);
	ID3D11PixelShader* Get_PixelShader(void);

public:
	static CShader* Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, WCHAR* szFileName, BYTE byLayoutFlag);

private:
	HRESULT Ready_Shader(WCHAR* szFileName, BYTE byLayoutFlag);
	HRESULT Load_ShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

public:
	virtual void Release(void);

private:
	ID3D11Device*			m_pGraphicDev;
	ID3D11DeviceContext*	m_pContext;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11InputLayout*		m_pVertexLayout;
	ID3D11PixelShader*		m_pPixelShader;
};

#endif // Shader_h__
