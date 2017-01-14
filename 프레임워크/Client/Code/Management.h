#ifndef Management_h__
#define Management_h__

#include "Renderer.h"

class CScene;
class CManagement
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement(void);
	~CManagement(void);

public:
	void Add_RenderGroup(CRenderer::RENDERTYPE eType, CGameObject* pGameObject, FLOAT fViewZ);
	void Add_RenderInstGroup(CRenderer::RENDERTYPE eType, UINT uiObjNum, XMFLOAT4X4* pMatWorld);

public:
	HRESULT Ready_Management(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	INT Update(const FLOAT& fTimeDelta);
	void Render(void);
	void Release(void);

public:
	template <typename T>
	HRESULT Change_Scene(T& Functor);

private:
	CScene*		m_pScene;
	CRenderer*	m_pRenderer;

private:
	ID3D11Device*			m_pGraphicDev;
	ID3D11DeviceContext*	m_pContext;
};

// SceneChange
template <typename T>
HRESULT CManagement::Change_Scene(T& Functor)
{
	if (m_pScene != NULL)
		Safe_Release(m_pScene);

	FAILED_CHECK_RETURN(Functor(&m_pScene, m_pGraphicDev, m_pContext), E_FAIL);

	m_pRenderer->Clear_RenderGroup();

	return S_OK;
}

#endif // Management_h__