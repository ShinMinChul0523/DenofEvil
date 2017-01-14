//For.Getter------------------------------------------------------------------------------------------------------------
ID3D11VertexShader* Get_VertexShader(const TCHAR* pShaderTag)
{
	return CShaderMgr::GetInstance()->Get_VertexShader(pShaderTag);
}

ID3D11InputLayout* Get_InputLayout(const TCHAR* pShaderTag)
{
	return CShaderMgr::GetInstance()->Get_InputLayout(pShaderTag);
}

ID3D11PixelShader* Get_PixelShader(const TCHAR* pShaderTag)
{
	return CShaderMgr::GetInstance()->Get_PixelShader(pShaderTag);
}
BOOL Sphere_InFrustum(const XMFLOAT3* pCenter, const FLOAT& fRadius)
{
	return CFrustum::GetInstance()->Sphere_InFrustum(pCenter, fRadius);
}

BOOL Vertex_InFrustum(const XMFLOAT3* pPoint)
{
	return CFrustum::GetInstance()->Vertex_InFrustum(pPoint);
}

const XMFLOAT4X4* Get_CurCameraProj(void)
{
	return CCameraMgr::GetInstance()->Get_CurCameraProj();
}

const XMFLOAT4X4* Get_CurCameraView(void)
{
	return CCameraMgr::GetInstance()->Get_CurCameraView();
}

FLOAT Get_TerrainColY(const XMFLOAT3& vPos)
{
	return CCollision::GetInstance()->Get_TerrainColY(vPos);
}

XMFLOAT3 Pick_Terrain(const XMFLOAT3& vPos, const XMFLOAT3& vDir)
{
	return CCollision::GetInstance()->Pick_Terrain(vPos, vDir);
}

ID3D11RenderTargetView** Get_RanderTargetViewInMgr(const TCHAR* pRenderTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Get_RanderTargetView(pRenderTargetTag);
}

ID3D11DepthStencilView* Get_DepthStencilViewInMgr(const TCHAR* pRenderTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Get_DepthStencilView(pRenderTargetTag);
}

//For.Setter-------------------------------------------------------------------------------------------------------------
void Add_RenderGroup(CRenderer::RENDERTYPE eType, CGameObject* pGameObject, FLOAT fViewZ)
{
	CManagement::GetInstance()->Add_RenderGroup(eType, pGameObject, fViewZ);
}

void Add_RenderInstGroup(CRenderer::RENDERTYPE eType, UINT uiObjNum, XMFLOAT4X4* pMatWorld)
{
	CManagement::GetInstance()->Add_RenderInstGroup(eType, uiObjNum, pMatWorld);
}

void Set_CurCamera(CCameraMgr::CAMERALIST eCameraName)
{
	CCameraMgr::GetInstance()->Set_CurCamera(eCameraName);
}

void Set_TerrainVtxInCollision(const VTXTEX* pTerrainVtx)
{
	CCollision::GetInstance()->Set_TerrainVtxInCollision(pTerrainVtx);
}

void Set_RenderTarget(const TCHAR* pRenderTargetTag, const UINT& uiNumViews, BOOL bDepthStencilView)
{
	CRenderTargetMgr::GetInstance()->Set_RenderTarget(pRenderTargetTag, uiNumViews, bDepthStencilView);
}

void Set_Texture(const TCHAR* pRenderTargetTag, const UINT& uiSlot, const UINT uiIndex)
{
	CRenderTargetMgr::GetInstance()->Set_Texture(pRenderTargetTag, uiSlot, uiIndex);
}

//For.Generic------------------------------------------------------------------------------------------------------------

// Init
HRESULT Ready_Management(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
{
	return CManagement::GetInstance()->Ready_Management(pGraphicDev, pContext);
}

HRESULT Ready_ShaderFromFiles(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
	, const TCHAR* pShaderTag, WCHAR* szFileName, BYTE byLayoutFlag)
{
	return CShaderMgr::GetInstance()->Ready_ShaderFromFiles(pGraphicDev, pContext, pShaderTag, szFileName, byLayoutFlag);
}

HRESULT Ready_Frustum(void)
{
	return CFrustum::GetInstance()->Ready_Frustum();
}

void Ready_DynamicCamera(ID3D11DeviceContext* pContext, CCameraMgr::CAMERALIST eCameraName, FLOAT fNear, FLOAT fFar, XMFLOAT3& vPos, XMFLOAT3& vTarget)
{
	CCameraMgr::GetInstance()->Ready_DynamicCamera(pContext, eCameraName, fNear, fFar, vPos, vTarget);
}

void Ready_StaticCamera(ID3D11DeviceContext* pContext, CCameraMgr::CAMERALIST eCameraName, const CTransform* pObjTrans, FLOAT fDist, FLOAT fHeightPivot
	, FLOAT fNear, FLOAT fFar, XMFLOAT3& vPos, XMFLOAT3& vTarget)
{
	CCameraMgr::GetInstance()->Ready_StaticCamera(pContext, eCameraName, pObjTrans, fDist, fHeightPivot, fNear, fFar, vPos, vTarget);
}

HRESULT Ready_RenderTarget(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const TCHAR* pRenderTargetTag
	, const DXGI_FORMAT& eFormat, const WORD& wSizeX, const WORD& wSizeY, FLOAT fRenderPosX, FLOAT fRenderPosY)
{
	return CRenderTargetMgr::GetInstance()->Ready_RenderTarget(pGraphicDev, pContext, pRenderTargetTag, eFormat, wSizeX, wSizeY, fRenderPosX, fRenderPosY);
}

// Update & Render
INT Update_Management(const FLOAT& fTimeDelta)
{
	return CManagement::GetInstance()->Update(fTimeDelta);
}
void Render_Management(void)
{
	CManagement::GetInstance()->Render();
}

template <typename T> HRESULT Change_Scene(T& Functor)
{
	return CManagement::GetInstance()->Change_Scene(Functor);
}

void Transform_Frustum(void)
{
	CFrustum::GetInstance()->Transform_Frustum();
}

void Update_CurCamera(const FLOAT& fTimeDelta)
{
	CCameraMgr::GetInstance()->Update_CurCamera(fTimeDelta);
}

void Render_RenderTarget(const TCHAR* pRenderTargetTag)
{
	CRenderTargetMgr::GetInstance()->Render(pRenderTargetTag);
}

// Destroy
void Release_Utility(void)
{
	CCollision::GetInstance()->DestroyInstance();
	CFrustum::GetInstance()->DestroyInstance();
	CCameraMgr::GetInstance()->DestroyInstance();
	CShaderMgr::GetInstance()->DestroyInstance();
	CRenderTargetMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}