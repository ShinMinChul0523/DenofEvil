//For.Getter------------------------------------------------------------------------------------------------------------
XMFLOAT4X4 Get_TransMeshBone(UINT uiObjNum, INT iSubsetIdx)
{
	return CMeshMgr::GetInstance()->Get_TransMeshBone(uiObjNum, iSubsetIdx);
}
const XMFLOAT3* Get_MeshMin(UINT uiObjNum)
{
	return CMeshMgr::GetInstance()->Get_MeshMin(uiObjNum);
}

const XMFLOAT3* Get_MeshMax(UINT uiObjNum)
{
	return CMeshMgr::GetInstance()->Get_MeshMax(uiObjNum);
}

BOOL Get_MeshUseAlpha(UINT uiObjNum)
{
	return CMeshMgr::GetInstance()->Get_MeshUseAlpha(uiObjNum);
}

//For.Setter-------------------------------------------------------------------------------------------------------------
void Add_ObbCheckList(UINT uiObjNum, const XMFLOAT4X4* pWorld, const CGameObject* pObj)
{
	CMeshMgr::GetInstance()->Add_ObbCheckList(uiObjNum, pWorld, pObj);
}

HRESULT Reserve_ContainerSize(const WORD& wSize)
{
	return CResourceMgr::GetInstance()->Reserve_ContainerSize(wSize);
}

//For.Generic------------------------------------------------------------------------------------------------------------

// Init
HRESULT Ready_Buffer(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
	, const WORD& wContainerIdx, CResourceMgr::BUFFERTYPE eBufferType, const TCHAR* pResourceKey
	, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv)
{
	return CResourceMgr::GetInstance()->Ready_Buffer(pGraphicDev, pContext, wContainerIdx, eBufferType, pResourceKey, wCntX, wCntZ, wItv);
}

HRESULT Ready_Texture(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, const WORD& wContainerIdx, const TCHAR* pResourceKey
	, CTexture::TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt)
{
	return CResourceMgr::GetInstance()->Ready_Texture(pGraphicDev, pContext, wContainerIdx, pResourceKey, eTextureType, pFilePath, wCnt);
}

HRESULT Ready_Mesh(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
	, const WORD& wContainerIdx, CResourceMgr::MESHTYPE eMeshType, const TCHAR* pResourceKey, const TCHAR* pFilePath)
{
	return CResourceMgr::GetInstance()->Ready_Mesh(pGraphicDev, pContext, wContainerIdx, eMeshType, pResourceKey, pFilePath);
}

void Ready_MeshMgr(MESHDATA* pMeshData)
{
	CMeshMgr::GetInstance()->Ready_MeshMgr(pMeshData);
}

// Update & Render
CResource* Clone_ResourceMgr(const WORD& wContainerIdx, const TCHAR* pResourceKey)
{
	return CResourceMgr::GetInstance()->Clone_ResourceMgr(wContainerIdx, pResourceKey);
}

void Reset_Resource(const WORD& wContainerIdx)
{
	CResourceMgr::GetInstance()->Reset_Resource(wContainerIdx);
}

void Render_MeshMgr(UINT uiObjNum, BOOL bColliderDraw)
{
	CMeshMgr::GetInstance()->Render_MeshMgr(uiObjNum, bColliderDraw);
}

void RenderAnim_MeshMgr(UINT uiObjNum, CAnimationInfo* pAnimInfo, BOOL bColliderDraw)
{
	CMeshMgr::GetInstance()->RenderAnim_MeshMgr(uiObjNum, pAnimInfo, bColliderDraw);
}

void RenderInst_MeshMgr(UINT uiObjNum, const vector<XMFLOAT4X4*>& vecObjWorld)
{
	CMeshMgr::GetInstance()->RenderInst_MeshMgr(uiObjNum, vecObjWorld);
}

// Destroy
void Release_Resource(void)
{
	CMeshMgr::GetInstance()->DestroyInstance();
	CResourceMgr::GetInstance()->DestroyInstance();
}