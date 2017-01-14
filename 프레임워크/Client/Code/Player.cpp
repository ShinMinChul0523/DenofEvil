#include "stdafx.h"
#include "Player.h"
#include "Export.h"
#include "AnimationInfo.h"
#include "Transform.h"
#include "NavMesh.h"

CPlayer::CPlayer(ID3D11DeviceContext* pContext)
	: CGameObject(pContext)
	, m_pAnimInfo(nullptr)
	, m_pMonAnimInfo(nullptr)
	, m_pTransform(nullptr)
	, m_uiObjNum(0)
	, m_uiNavIdx(0)
	, m_fSpeed(0.f)
	, m_fHeight(0.f)
	, m_bJump(FALSE)
	, m_fJumpTime(0.f)
	, m_UseNavMesh(TRUE)
{
	
	m_vLook = XMFLOAT3(0.f, 0.f, -1.f);
}

CPlayer::~CPlayer(void)
{
}

CPlayer* CPlayer::Create(ID3D11DeviceContext* pContext)
{
	CPlayer* pObject = new CPlayer(pContext);

	if (FAILED(pObject->Initialize()))
		Safe_Release(pObject);

	return pObject;
}

HRESULT CPlayer::Initialize(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fSpeed = 6.f;
	m_fHeight = 2.f;
	m_uiObjNum = MESHNUM_PLAYER;

	FLOAT fScale = (1.f / Get_MeshMax(m_uiObjNum)->y) * m_fHeight;
	m_pTransform->m_vScale = XMFLOAT3(fScale, fScale, fScale);
	m_pTransform->m_vPos = XMFLOAT3(126.7f, 0.f, 345.f);
	m_pTransform->m_vDir = XMFLOAT3(0.f, 0.f, -1.f);

	return S_OK;
}

INT CPlayer::Update(const FLOAT& fTimeDelta)
{
	// Temp	-------------------------------------------------------------------------------
	if (Get_InputKeyDown(DIK_1)) m_fSpeed += 1;			// 플레이어 스피드
	if (Get_InputKeyDown(DIK_2)) m_fSpeed -= 1;
	if (m_fSpeed < 0) m_fSpeed = 0;

	if (Get_InputKeyDown(DIK_9))						// 네비게이션 메시 on off
	{
		m_UseNavMesh = !m_UseNavMesh;

		if (m_UseNavMesh)
			m_uiNavIdx = 0;
	}

	// Camera
	if (Get_InputKeyDown(DIK_I)) Set_CurCamera(CCameraMgr::CAMERA_DYNAMIC);
	if (Get_InputKeyDown(DIK_O))
	{
		m_uiObjNum = MESHNUM_PLAYER;

		FLOAT fScale = (1.f / Get_MeshMax(m_uiObjNum)->y) * m_fHeight;
		m_pTransform->m_vScale = XMFLOAT3(fScale, fScale, fScale);

		Set_CurCamera(CCameraMgr::CAMERA_WALK);
		m_fSpeed = 4.f;
	}

	
	Move(fTimeDelta);

	//if (Get_InputMouseDown(CInput::DIM_LBUTTON)) m_pAnimInfo->Set_Key(PLAYER_BLADEATT);
	//if (Get_InputMouseDown(CInput::DIM_RBUTTON)) m_pAnimInfo->Set_Key(PLAYER_BLADEDEF);


	CCollision::GetInstance()->Update(fTimeDelta);

	// Update
	CGameObject::Update(fTimeDelta);
	Add_RenderGroup(CRenderer::RENDER_ZSORT, this);

	m_pTransform->m_vDir = XMFLOAT3(m_pTransform->m_matWorld._31, m_pTransform->m_matWorld._32, m_pTransform->m_matWorld._33);
	XMStoreFloat3(&m_pTransform->m_vDir, XMVector3Normalize(XMLoadFloat3(&m_pTransform->m_vDir)));

	m_pTransform->Update_MatrixNotXRot();
	
	return 0;
}

void CPlayer::Render(void)
{
	m_pContext->IASetInputLayout(Get_InputLayout(L"Shader_DynamicMesh"));

	ID3D11Buffer* pBaseShaderCB = Get_BaseShaderCB();
	ID3D11Buffer* pDynamicShaderCB = Get_DynamicShaderCB();

	ID3D11SamplerState* pBaseSampler = Get_BaseSampler();

	BASESHADER_CB tBaseShaderCB;

	tBaseShaderCB.matWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_pTransform->m_matWorld));
	tBaseShaderCB.matView = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraView()));
	tBaseShaderCB.matProj = XMMatrixTranspose(XMLoadFloat4x4(Get_CurCameraProj()));

	m_pContext->UpdateSubresource(pBaseShaderCB, 0, NULL, &tBaseShaderCB, 0, 0);

	m_pContext->VSSetShader(Get_VertexShader(L"Shader_DynamicMesh"), NULL, 0);
	m_pContext->VSSetConstantBuffers(0, 1, &pBaseShaderCB);
	m_pContext->VSSetConstantBuffers(1, 1, &pDynamicShaderCB);
	m_pContext->PSSetShader(Get_PixelShader(L"Shader_DynamicMesh"), NULL, 0);
	m_pContext->PSSetSamplers(0, 1, &pBaseSampler);

	if (m_uiObjNum == MESHNUM_PLAYER)
		RenderAnim_MeshMgr(m_uiObjNum, m_pAnimInfo, FALSE);

	else
		RenderAnim_MeshMgr(m_uiObjNum, m_pMonAnimInfo, FALSE);
}

void CPlayer::Release(void)
{
	
	CGameObject::Release();
	delete this;
}

HRESULT CPlayer::Ready_Component(void)
{
	CComponent* pComponent = NULL;

	// AnimationInfo
	pComponent = CAnimationInfo::Create(MESHNUM_PLAYER);
	m_pAnimInfo = dynamic_cast<CAnimationInfo*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_AnimInfo", pComponent));

	// Transform
	pComponent = CTransform::Create();
	m_pTransform = dynamic_cast<CTransform*>(pComponent);
	if (pComponent == NULL) return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	
	return S_OK;
}

void CPlayer::Move(const FLOAT& fTimeDelta)
{
	// Move
	XMVECTOR vMove = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	if (Get_InputKeyState(DIK_W) & 0x80)	vMove += XMVectorSet(m_pTransform->m_vDir.x, 0.f, m_pTransform->m_vDir.z, 0.f);
	if (Get_InputKeyState(DIK_S) & 0x80)	vMove -= XMVectorSet(m_pTransform->m_vDir.x, 0.f, m_pTransform->m_vDir.z, 0.f);
	if (Get_InputKeyState(DIK_A) & 0x80)	vMove -= XMVectorSet(m_pTransform->m_vDir.z, 0.f, -m_pTransform->m_vDir.x, 0.f);
	if (Get_InputKeyState(DIK_D) & 0x80)	vMove += XMVectorSet(m_pTransform->m_vDir.z, 0.f, -m_pTransform->m_vDir.x, 0.f);

	// Set Animation
	if (!m_pAnimInfo->Get_Playing() && m_bJump == FALSE)
	{
		if (XMVectorGetX(vMove) == 0.f && XMVectorGetZ(vMove) == 0.f)
			m_pAnimInfo->Set_Key(PLAYER_IDLE);

		else
		{

			//if (Get_InputKeyState(DIK_S) & 0x80)
				//m_pAnimInfo->Set_Key(PLAYER_BACKSTAB);

			//else
				//m_pAnimInfo->Set_Key(PLAYER_RUN);
		}
	}

	if (XMVectorGetX(vMove) != 0.f || XMVectorGetZ(vMove) != 0.f)
		XMStoreFloat3(&m_vLook, XMVector3Normalize(vMove));

	vMove = (XMVector3Normalize(vMove) * m_fSpeed * fTimeDelta) + XMLoadFloat3(&m_pTransform->m_vPos);

	XMFLOAT3 vStoreMove;
	XMStoreFloat3(&vStoreMove, vMove);

	FLOAT fTerrainHeight = Get_TerrainColY(m_pTransform->m_vPos);

	// Jump
	if ((Get_InputKeyState(DIK_SPACE) & 0x80) && m_bJump == FALSE)
	{
		
		m_bJump = TRUE;
		m_fJumpTime = 0.f;
	}

	if (m_bJump == TRUE)
	{
		m_fJumpTime += fTimeDelta;

		if (m_fJumpTime > 0.4f)
		{
			float fJumpY = (sinf(D3DXToRadian(90.f)) - ((m_fJumpTime - 0.4f) * 2.5f));
			vStoreMove.y += fJumpY * fTimeDelta * 30.f;
		}

		else
			vStoreMove.y = fTerrainHeight + 0.01f;

		if (!m_pAnimInfo->Get_Playing())
			m_pAnimInfo->Set_Key(PLAYER_IDLE);
	}

	// NavMesh
	XMFLOAT3 vColDir = XMFLOAT3(0.f, 0.f, 0.f);

	if ((!m_UseNavMesh) || CNavMesh::GetInstance()->MoveOnNavMesh(&vStoreMove, m_uiNavIdx, nullptr, &vColDir))
	{
		XMFLOAT4X4 matPlayerWorld;

		XMMATRIX matScale = XMMatrixScalingFromVector(XMLoadFloat3(&m_pTransform->m_vScale));
		XMMATRIX matRotY = XMMatrixRotationY(m_pTransform->m_vAngle.y);
		XMMATRIX matRotZ = XMMatrixRotationZ(m_pTransform->m_vAngle.z);
		XMMATRIX matTrans = XMMatrixTranslationFromVector(XMLoadFloat3(&vStoreMove));

		XMStoreFloat4x4(&matPlayerWorld, matScale * matRotY * matRotZ * matTrans);

		// Monster Collision
		XMFLOAT3 vMin = *Get_MeshMin(m_uiObjNum);
		XMFLOAT3 vMax = *Get_MeshMax(m_uiObjNum);

		vMin.z -= 5.f;
		vMax.z += 5.f;

		XMFLOAT3 vColResult = CCollision::GetInstance()->Get_OBBList(vMin, vMax, vStoreMove, m_vLook, matPlayerWorld);

		if (vColResult.x != vStoreMove.x || vColResult.z != vStoreMove.z)
			CNavMesh::GetInstance()->MoveOnNavMesh(&vColResult, m_uiNavIdx);

		m_pTransform->m_vPos = vColResult;

		if (m_bJump == FALSE)
			m_pTransform->m_vPos.y = fTerrainHeight;

		else
		{
			if (fTerrainHeight > m_pTransform->m_vPos.y)
			{
				m_pTransform->m_vPos.y = fTerrainHeight;
				m_bJump = FALSE;
			}
		}
	}

	// Sliding
	else
	{
		XMVECTOR vSlide = (XMVector3Dot(XMLoadFloat3(&vColDir), -XMLoadFloat3(&m_vLook)) * XMLoadFloat3(&vColDir)) + XMLoadFloat3(&m_vLook);
		vSlide = XMVector3Normalize(vSlide);

		FLOAT fAngleSpeed = min((1.f - XMVectorGetX(XMVector3Dot(XMLoadFloat3(&vColDir), -XMLoadFloat3(&m_vLook)))) * 2.f, 1.f);

		XMFLOAT3 vMovePosSlide;
		XMStoreFloat3(&vMovePosSlide, XMLoadFloat3(&m_pTransform->m_vPos) + (vSlide * m_fSpeed * fTimeDelta * fAngleSpeed));

		if (CNavMesh::GetInstance()->MoveOnNavMesh(&vMovePosSlide, m_uiNavIdx))
			m_pTransform->m_vPos = vMovePosSlide;
	}

	// Angle
	int iDistance = 0;

	iDistance = Get_InputMouseMove(CInput::DIM_X);

	if (iDistance)
		m_pTransform->m_vAngle.y += iDistance * 0.003f;

	iDistance = Get_InputMouseMove(CInput::DIM_Y);

	if (iDistance)
	{
		if ((iDistance > 0.f && m_pTransform->m_vAngle.x < ToRadian(80.f))
			|| (iDistance < 0.f && m_pTransform->m_vAngle.x > ToRadian(-80.f)))
		{
			m_pTransform->m_vAngle.x += iDistance * 0.003f;
		}
	}
}
