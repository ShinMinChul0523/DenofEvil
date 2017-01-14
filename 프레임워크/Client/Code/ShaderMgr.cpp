#include "stdafx.h"
#include "ShaderMgr.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(CShaderMgr)

CShaderMgr::CShaderMgr(void)
{
}

CShaderMgr::~CShaderMgr(void)
{
}

ID3D11VertexShader* CShaderMgr::Get_VertexShader(const TCHAR* pShaderTag)
{
	CShader*	pShader = Find_Shader(pShaderTag);

	if (NULL == pShader)
		return NULL;

	return pShader->Get_VertexShader();
}

ID3D11InputLayout* CShaderMgr::Get_InputLayout(const TCHAR* pShaderTag)
{
	CShader*	pShader = Find_Shader(pShaderTag);

	if (NULL == pShader)
		return NULL;

	return pShader->Get_InputLayout();
}

ID3D11PixelShader* CShaderMgr::Get_PixelShader(const TCHAR* pShaderTag)
{
	CShader*	pShader = Find_Shader(pShaderTag);

	if (NULL == pShader)
		return NULL;

	return pShader->Get_PixelShader();
}

HRESULT CShaderMgr::Ready_ShaderFromFiles(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
	, const TCHAR* pShaderTag, WCHAR* szFileName, BYTE byLayoutFlag)
{
	CShader* pShader = Find_Shader(pShaderTag);

	if (NULL != pShader)
	{
		MSG_BOX("Already ShaderFile Failed");
		return E_FAIL;
	}

	pShader = CShader::Create(pGraphicDev, pContext, szFileName, byLayoutFlag);

	if (NULL == pShader)
		return E_FAIL;

	m_mapShaders.insert(MAPSHADERS::value_type(pShaderTag, pShader));

	return S_OK;
}

CShader* CShaderMgr::Find_Shader(const TCHAR* pShaderTag)
{
	MAPSHADERS::iterator iter = find_if(m_mapShaders.begin(), m_mapShaders.end(), CTagFinder(pShaderTag));

	if (iter == m_mapShaders.end())
		return NULL;

	return iter->second;
}

void CShaderMgr::Release(void)
{
	for_each(m_mapShaders.begin(), m_mapShaders.end(), CRelease_Pair());
	m_mapShaders.clear();

	delete this;
}
