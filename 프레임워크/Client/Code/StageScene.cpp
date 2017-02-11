#include "stdafx.h"
#include "StageScene.h"
#include "Export_Utility.h"
#include "Transform.h"
#include "Terrain.h"
#include "Skybox.h"
#include "DefaultObj.h"
#include "Player.h"
#include "Golem.h"
#include "NavMesh.h"

CStageScene::CStageScene(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
	: CScene(pGraphicDev, pContext)
{
}

CStageScene::~CStageScene(void)
{
}

CStageScene* CStageScene::Create(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pContext)
{
	SetCursor(NULL);
	ShowCursor(FALSE);

	CStageScene* pScene = new CStageScene(pGraphicDev, pContext);

	if (FAILED(pScene->Ready_Scene()))
	{
		MSG_BOX("StageScene Create Failed");
		Safe_Release(pScene);
	}

	return pScene;
}

HRESULT CStageScene::Ready_Scene(void)
{
	if (FAILED(Ready_GameLogic()))		return E_FAIL;
	if (FAILED(Ready_Environment()))	return E_FAIL;

	Ready_NavCell();

	return S_OK;
}

INT CStageScene::Update(const FLOAT& fTimeDelta)
{
	CScene::Update(fTimeDelta);
	Update_CurCamera(fTimeDelta);

	// Fix Mouse
	POINT ptMouse = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);

	return 0;
}

HRESULT CStageScene::Ready_GameLogic(void)
{
	CLayer* pLayer = CLayer::Create();
	CGameObject* pGameObject = NULL;

	// Terrain
	//pGameObject = CTerrain::Create(m_pContext);
	//if (NULL == pGameObject) return E_FAIL;
	//pLayer->Ready_Object(L"Terrain", pGameObject);

	// Skybox
	pGameObject = CSkybox::Create(m_pContext);
	if (NULL == pGameObject) return E_FAIL;
	pLayer->Ready_Object(L"Skybox", pGameObject);

	// Default Object
	HANDLE hFile = NULL;

	DWORD dwByte;
	OBJDATA tObjData;

	hFile = CreateFile(L"../bin/Data/ObjTest2.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	while (TRUE)
	{
		ReadFile(hFile, &tObjData, sizeof(OBJDATA), &dwByte, NULL);

		if (dwByte == 0)
			break;

		pGameObject = CDefaultObj::Create(m_pContext, tObjData);
		if (NULL == pGameObject) return E_FAIL;
		pLayer->Ready_Object(L"DefaultObj", pGameObject);
	}

	CloseHandle(hFile);


	// Player
	pGameObject = CPlayer::Create(m_pContext);
	if (NULL == pGameObject) return E_FAIL;
	pLayer->Ready_Object(L"Player", pGameObject);

	
	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_GameLogic", pLayer));

	return S_OK;
}

HRESULT CStageScene::Ready_Environment(void)
{
	CLayer* pLayer = CLayer::Create();
	CGameObject* pGameObject = NULL;

	// Camera
	MAPLAYER::iterator iterLayer = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTagFinder(L"Layer_GameLogic"));
	const CComponent* pTransform = iterLayer->second->Get_Component(L"Player", L"Com_Transform");

	Ready_DynamicCamera(m_pContext, CCameraMgr::CAMERA_DYNAMIC, 0.1f, 1000.f, XMFLOAT3(0.f, 5.f, -10.f), XMFLOAT3(0.f, 0.f, 0.f));
	Ready_StaticCamera(m_pContext, CCameraMgr::CAMERA_WALK, (CTransform*)pTransform, 5.f,5.f, 0.1f, 1000.f, XMFLOAT3(0.f, 5.f, -10.f), XMFLOAT3(0.f, 0.f, 0.f));	
	
	Ready_Frustum();

	// RenderTarget
	Ready_RenderTarget(m_pGraphicDev, m_pContext, L"RT_Blend", DXGI_FORMAT_R8G8B8A8_UNORM, WINCX, WINCY, -0.8f, 0.8f);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Environment", pLayer));

	return S_OK;
}

void CStageScene::Release(void)
{
	CNavMesh::GetInstance()->DestroyInstance();
	CScene::Release();

	delete this;
}

void CStageScene::Ready_NavCell(void)
{
	CNavMesh::GetInstance()->Reserve_CellContainerSize(m_pGraphicDev, m_pContext, 10);

	HANDLE hFile = NULL;
	DWORD dwByte;

	CNavCell* pCell = NULL;
	XMFLOAT3 vPoint[3];
	BOOL bOnlyMon = FALSE;

	hFile = CreateFile(L"../bin/Data/1021Navi.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	while (TRUE)
	{
		for (INT iIndex = 0; iIndex < 3; ++iIndex)
		{
			ReadFile(hFile, &vPoint[iIndex], sizeof(XMFLOAT3), &dwByte, NULL);

			if (dwByte == 0)
				break;
		}

		if (dwByte == 0)
			break;

		ReadFile(hFile, &bOnlyMon, sizeof(BOOL), &dwByte, NULL);

		if (dwByte == 0)
			break;

		CNavMesh::GetInstance()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);
	}

	CloseHandle(hFile); 

	CNavMesh::GetInstance()->Link_Cell();
}