#include "stdafx.h"
#include "MainApp.h"
#include "Export.h"
#include "SceneSelector.h"

CMainApp::CMainApp(void)
{
}

CMainApp::~CMainApp(void)
{
}

HRESULT CMainApp::Initialize(void)
{
	// Graphic Device
	ID3D11Device* pGraphicDev = NULL;
	ID3D11DeviceContext* pContext = NULL;

	if (FAILED(Ready_GraphicDev(CGraphicDev::MODE_WIN, g_hWnd, WINCX, WINCY, 0, TRUE, pGraphicDev, pContext)))
	{
		MSG_BOX("Ready_GraphicDev Failed");
		return E_FAIL;
	}

	// Collision
	CCollision::GetInstance()->Set_Device(pGraphicDev, pContext);

	// Resource
	Reserve_ContainerSize(RESOURCE_END);

	Ready_TextureFromFile(pGraphicDev, pContext);
	Ready_DynamicMeshFromFile(pGraphicDev, pContext);
	Ready_StaticMeshFromFile(pGraphicDev, pContext);

	// Buffer
	Ready_Buffer(pGraphicDev, pContext, RESOURCE_STAGE, CResourceMgr::BUFFER_TERRAIN
		, L"Buffer_TerrainTex", MAPCX, MAPCZ, MAPITV);

	Ready_Buffer(pGraphicDev, pContext, RESOURCE_STAGE, CResourceMgr::BUFFER_RCTEX, L"Buffer_RcTex");
	Ready_Buffer(pGraphicDev, pContext, RESOURCE_STAGE, CResourceMgr::BUFFER_CUBE, L"Buffer_CubeTex");

	// Management
	if (FAILED(Ready_Management(pGraphicDev, pContext)))
	{
		MSG_BOX("Ready_Management Failed");
		return E_FAIL;
	}

	// Scene
	if (FAILED(Change_Scene(CSceneSelector(CSceneSelector::SCENE_STAGE))))
	{
		MSG_BOX("Change_Scene(SCENE_STAGE) Failed");
		return E_FAIL;
	}

	return S_OK;
}

INT CMainApp::Update(const FLOAT& fTimeDelta)
{
	return Update_Management(fTimeDelta);
}

void CMainApp::Render(void)
{
	Render_Management();
}

CMainApp* CMainApp::Create(void)
{
	CMainApp* pMainApp = new CMainApp;

	if (FAILED(pMainApp->Initialize()))
	{
		MSG_BOX("CMainApp Create Failed");
		Safe_Release(pMainApp);
	}

	return pMainApp;
}

void CMainApp::Release(void)
{
	Release_Resource();
	Release_Utility();

	delete this;
}

void CMainApp::Ready_DynamicMeshFromFile(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
{
	wstring wstrPath = L"../bin/Data/List_DynamicMesh.txt";

	wifstream inFile;
	inFile.open(wstrPath.c_str(), ios::in);

	TCHAR pFileName[MAX_NAME];
	TCHAR pFileAlpha[2];
	TCHAR pFileBillboard[2];

	WORD wAlpha, wBillboard;
	MESHDATA* pMeshData = NULL;

	while (!inFile.eof())
	{
		inFile.getline(pFileName, MAX_NAME, '-');
		inFile.getline(pFileAlpha, 2, '-');
		inFile.getline(pFileBillboard, 2);

		wAlpha = _wtoi(pFileAlpha);
		wBillboard = _wtoi(pFileBillboard);

		wstring wstrMeshKey = L"Mesh_";
		wstrMeshKey += pFileName;

		wstring wstrMeshPath = L"../bin/Resource/Mesh/";
		wstrMeshPath += pFileName;
		wstrMeshPath += L".FBX";

		Ready_Mesh(pGraphicDev, pContext, RESOURCE_STAGE, CResourceMgr::MESH_DYNAMIC, wstrMeshKey.c_str(), wstrMeshPath.c_str());

		pMeshData = new MESHDATA;

		pMeshData->pMesh = (CMesh*)Clone_ResourceMgr(RESOURCE_STAGE, wstrMeshKey.c_str());
		pMeshData->bAlpha = (BOOL)wAlpha;
		pMeshData->bBillboard = (BOOL)wBillboard;

		Ready_MeshMgr(pMeshData);
	}

	inFile.close();
}

void CMainApp::Ready_StaticMeshFromFile(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
{
	// Static Mesh
	wstring wstrPath = L"../bin/Data/List_StaticMesh.txt";

	wifstream inFile;
	inFile.open(wstrPath.c_str(), ios::in);

	TCHAR pFileName[MAX_NAME];
	TCHAR pFileAlpha[2];
	TCHAR pFileBillboard[2];

	WORD wAlpha, wBillboard;
	MESHDATA* pMeshData = NULL;

	while (!inFile.eof())
	{
		inFile.getline(pFileName, MAX_NAME, '-');
		inFile.getline(pFileAlpha, 2, '-');
		inFile.getline(pFileBillboard, 2);

		wAlpha = _wtoi(pFileAlpha);
		wBillboard = _wtoi(pFileBillboard);

		wstring wstrMeshKey = L"Mesh_";
		wstrMeshKey += pFileName;

		wstring wstrMeshPath = L"../bin/Resource/Mesh/";
		wstrMeshPath += pFileName;
		wstrMeshPath += L".FBX";

		Ready_Mesh(pGraphicDev, pContext, RESOURCE_STAGE, CResourceMgr::MESH_STATIC, wstrMeshKey.c_str(), wstrMeshPath.c_str());

		pMeshData = new MESHDATA;

		pMeshData->pMesh = (CMesh*)Clone_ResourceMgr(RESOURCE_STAGE, wstrMeshKey.c_str());
		pMeshData->bAlpha = (BOOL)wAlpha;
		pMeshData->bBillboard = (BOOL)wBillboard;

		Ready_MeshMgr(pMeshData);
	}

	inFile.close();

	 wstrPath = L"../bin/Data/List_MapToolMesh.txt";

	inFile.open(wstrPath.c_str(), ios::in);

	while (!inFile.eof())
	{
		inFile.getline(pFileName, MAX_NAME, '-');
		inFile.getline(pFileAlpha, 2, '-');
		inFile.getline(pFileBillboard, 2);

		wAlpha = _wtoi(pFileAlpha);
		wBillboard = _wtoi(pFileBillboard);

		wstring wstrMeshKey = L"Mesh_";
		wstrMeshKey += pFileName;

		wstring wstrMeshPath = L"../bin/Resource/Mesh/";
		wstrMeshPath += pFileName;
		wstrMeshPath += L".FBX";

		Ready_Mesh(pGraphicDev, pContext, RESOURCE_STAGE, CResourceMgr::MESH_STATIC, wstrMeshKey.c_str(), wstrMeshPath.c_str());

		pMeshData = new MESHDATA;

		pMeshData->pMesh = (CMesh*)Clone_ResourceMgr(RESOURCE_STAGE, wstrMeshKey.c_str());
		pMeshData->bAlpha = (BOOL)wAlpha;
		pMeshData->bBillboard = (BOOL)wBillboard;

		Ready_MeshMgr(pMeshData);
	}

	inFile.close();
}

void CMainApp::Ready_TextureFromFile(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
{
	wstring wstrPath = L"../bin/Data/List_Texture.txt";

	wifstream inFile;
	inFile.open(wstrPath.c_str(), ios::in);

	TCHAR pFileName[MAX_NAME];
	TCHAR pFileType[4];
	TCHAR pFileNum[4];

	while (!inFile.eof())
	{
		inFile.getline(pFileName, MAX_NAME, '-');
		inFile.getline(pFileType, 4, '-');
		inFile.getline(pFileNum, 4);

		WORD wTextureCnt = _wtoi(pFileNum);

		wstring wstrTextureKey = L"Texture_";
		wstrTextureKey += pFileName;

		wstring wstrTexturePath = L"../bin/Resource/Texture/";
		wstrTexturePath += pFileName;
		if (wTextureCnt) wstrTexturePath += L"%d.";
		else wstrTexturePath += L".";
		wstrTexturePath += pFileType;

		CTexture::TEXTURETYPE eTextureType = CTexture::TYPE_NORMAL;

		if (!lstrcmp(pFileType, L"tga"))
			eTextureType = CTexture::TYPE_TGA;

		else if (!lstrcmp(pFileType, L"dds"))
			eTextureType = CTexture::TYPE_DDSCUBE;

		Ready_Texture(pGraphicDev, pContext, RESOURCE_STAGE, wstrTextureKey.c_str(), eTextureType, wstrTexturePath.c_str(), wTextureCnt);
	}

	inFile.close();
}