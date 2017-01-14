#ifndef Renderer_h__
#define Renderer_h__

#include "Define.h"

class CRcTex;
class CScene;
class CGameObject;
class CRenderer
{
public:
	enum RENDERTYPE { RENDER_PRIORITY, RENDER_ZSORT, RENDER_UI, RENDER_END, RENDER_INST, RENDER_ALPHA, RENDER_ALPHAINST };

private:
	explicit CRenderer(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);
	virtual ~CRenderer(void);

public:
	void Add_RenderGroup(RENDERTYPE eType, CGameObject* pGameObject, FLOAT fViewZ = 0);
	void Add_RenderInstGroup(RENDERTYPE eType, UINT uiObjNum, XMFLOAT4X4* pMatWorld);

public:
	HRESULT Ready_Renderer(void);
	void Render(void);
	void Clear_RenderGroup(void);

private:
	void Render_Priority(void);
	void Render_Inst(void);
	void Render_ZSort(void);
	void Render_AlphaInst(void);
	void Render_Alpha(void);
	void Render_UI(void);

public:
	static CRenderer* Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext);

public:
	void Release(void);

private:
	ID3D11Device*				m_pGraphicDev;
	ID3D11DeviceContext*		m_pContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pRenderTargetView;
	ID3D11DepthStencilView*		m_pDepthStencilView;

private:
	using RENDERLIST = list<CGameObject*>;
	RENDERLIST	m_RenderGroup[RENDER_END];

	using VECWORLD = vector<XMFLOAT4X4*>;
	using MAPINST = map<UINT, VECWORLD>;
	MAPINST m_mapInst;
	MAPINST m_mapAlphaInst;

	using MAPALPHA = multimap<FLOAT, CGameObject*, greater<FLOAT>>;
	MAPALPHA m_mapAlpha;

private:
	CRcTex* m_pRcTex;

	//Temp;
	BOOL m_bDrawNavMesh;
	BOOL m_bDrawRenderTarget;
};

#endif // Renderer_h__
