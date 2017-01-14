#ifndef ResourceMgr_h__
#define ResourceMgr_h__

#include "Define.h"

#include <fbxsdk.h>
#include "RcTex.h"
#include "CubeTex.h"
#include "TerrainTex.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Texture.h"

class CResourceMgr
{
	DECLARE_SINGLETON(CResourceMgr)

public:
	enum BUFFERTYPE { BUFFER_RCTEX, BUFFER_CUBE, BUFFER_TERRAIN, BUFFER_END };
	enum MESHTYPE { MESH_STATIC, MESH_DYNAMIC, MESH_END };

private:
	CResourceMgr(void);
	~CResourceMgr(void);

public:
	CResource* Clone_ResourceMgr(const WORD& wContainerIdx, const TCHAR* pResourceKey);
	HRESULT Reserve_ContainerSize(const WORD& wSize);

public:
	HRESULT Ready_Buffer(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
		, const WORD& wContainerIdx, BUFFERTYPE eBufferType, const TCHAR* pResourceKey
		, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv);

	HRESULT Ready_Texture(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
		, const WORD& wContainerIdx, const TCHAR* pResourceKey, CTexture::TEXTURETYPE eTextureType
		, const TCHAR* pFilePath, const WORD& wCnt);

	HRESULT Ready_Mesh(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext
		, const WORD& wContainerIdx, MESHTYPE eMeshType, const TCHAR* pResourceKey, const TCHAR* pFilePath);

private:
	void Ready_FbxSdkMgr(void);
	void Load_StaticMesh(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, FbxNode* pNode, CStaticMesh* pParent);
	void Load_DynamicMesh(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext, FbxNode* pNode, CDynaicMesh* pParent
		, FbxScene* pFbxScene, FbxArray<FbxString*>& arrAniName, vector<pair<WORD, WORD>>& vecFrameCnt);
	CResource* Find_Resource(const WORD& wContainerIdx, const TCHAR* pResourceKey);

private:
	void Load_FrameData(const TCHAR* pResourceKey, vector<pair<WORD, WORD>>& vecFrameCnt);
	FbxAMatrix GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime, FbxPose* pPose = NULL, FbxAMatrix* pParentGlobalPosition = NULL);
	FbxAMatrix GetPoseMatrix(FbxPose* pPose, int pNodeIndex);
	FbxAMatrix GetGeometry(FbxNode* pNode);

public:
	void Reset_Resource(const WORD& wContainerIdx);
	void Release(void);

private:
	using MAPRESOURCE = map<const TCHAR*, CResource*>;
	MAPRESOURCE*	m_pmapResource;
	WORD			m_wReservedSize;

private:
	CResource*		m_pRootMeshStore;
	FbxManager*		m_pFbxSdkMgr;

private:
	list<wstring*> m_ResourceNameList;
};

#endif // ResourceMgr_h__