#ifndef ShaderMgr_h__
#define ShaderMgr_h__

#include "Define.h"

class CShader;
class CShaderMgr
{
	DECLARE_SINGLETON(CShaderMgr)

public:
	enum LAYOUT_FLAG { LAYOUT_FLAG_BONE = 0x01 };

private:
	explicit CShaderMgr(void);
	~CShaderMgr(void);

public:
	ID3D11VertexShader* Get_VertexShader(const TCHAR* pShaderTag);
	ID3D11InputLayout* Get_InputLayout(const TCHAR* pShaderTag);
	ID3D11PixelShader* Get_PixelShader(const TCHAR* pShaderTag);

public:
	HRESULT Ready_ShaderFromFiles(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
		, const TCHAR* pShaderTag, WCHAR* szFileNamebyLayoutFlag, BYTE byLayoutFlag);

private:
	CShader* Find_Shader(const TCHAR* pShaderTag);

public:
	void Release(void);

private:
	using MAPSHADERS = map<const TCHAR*, CShader*>;
	MAPSHADERS	m_mapShaders;
};

#endif // ShaderMgr_h__
