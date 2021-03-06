#include "stdafx.h"
#include "DynamicCamera.h"
#include "Export.h"

CDynamicCamera::CDynamicCamera(ID3D11DeviceContext* pContext)
	: CCamera(pContext)
	, m_fSpeed(50.f)
{
}

CDynamicCamera::~CDynamicCamera(void)
{
}

CDynamicCamera* CDynamicCamera::Create(ID3D11DeviceContext* pContext, FLOAT fNear, FLOAT fFar, XMFLOAT3& vPos, XMFLOAT3& vTarget)
{
	CDynamicCamera* pCamera = new CDynamicCamera(pContext);

	if (FAILED(pCamera->Initialize(fNear, fFar, vPos, vTarget)))
		Safe_Release(pCamera);

	return pCamera;
}

INT CDynamicCamera::Update(const FLOAT& fTimeDelta)
{
	// Key
	WORD wInput = 0;

	if (Get_InputKeyState(DIK_W) & 0x80)		wInput |= 0x0001;
	if (Get_InputKeyState(DIK_S) & 0x80)		wInput |= 0x0002;
	if (Get_InputKeyState(DIK_A) & 0x80)		wInput |= 0x0004;
	if (Get_InputKeyState(DIK_D) & 0x80)		wInput |= 0x0008;

	if (Get_InputKeyState(DIK_UP) & 0x80)		wInput |= 0x0010;
	if (Get_InputKeyState(DIK_DOWN) & 0x80)		wInput |= 0x0020;
	if (Get_InputKeyState(DIK_LEFT) & 0x80)		wInput |= 0x0040;
	if (Get_InputKeyState(DIK_RIGHT) & 0x80)	wInput |= 0x0080;

	if (Get_InputKeyState(DIK_Q) & 0x80)		wInput |= 0x0100;
	if (Get_InputKeyState(DIK_E) & 0x80)		wInput |= 0x0200;

	// Move Camera
	XMVECTOR vPos = XMLoadFloat3(m_pPos);
	XMVECTOR vTarget = XMLoadFloat3(m_pTarget);

	XMVECTOR vLook = vTarget - vPos;
	vLook = XMVector3Normalize(vLook);

	// Pos
	if (wInput & 0x000f)
	{
		XMVECTOR vYzeroRight, vYzeroLook;

		vYzeroRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
		vYzeroRight = XMVectorSetY(vYzeroRight, 0.f);
		vYzeroRight = XMVector3Normalize(vYzeroRight);

		vYzeroLook = XMVectorSetY(vLook, 0.f);
		vYzeroLook = XMVector3Normalize(vYzeroLook);

		if (wInput & 0x0001) vPos += vYzeroLook * m_fSpeed * fTimeDelta;
		if (wInput & 0x0002) vPos -= vYzeroLook * m_fSpeed * fTimeDelta;
		if (wInput & 0x0004) vPos -= vYzeroRight * m_fSpeed * fTimeDelta;
		if (wInput & 0x0008) vPos += vYzeroRight * m_fSpeed * fTimeDelta;

		vTarget = vPos + vLook;

		XMMATRIX matView = XMMatrixLookAtLH(vPos, vTarget, XMVectorSet(0.f, 1.f, 0.f, 0.f));

		XMStoreFloat4x4(m_pView, matView);
		XMStoreFloat3(m_pPos, vPos);
		XMStoreFloat3(m_pTarget, vTarget);
	}

	// Target
	if (wInput & 0x00f0)
	{
		XMVECTOR vRotLook = vLook;

		FLOAT fAngleX = 0.f;
		FLOAT fAngleY = 0.f;

		XMMATRIX matRotAxis = XMMatrixIdentity();

		if (wInput & 0x0010)	fAngleX -= m_fSpeed * fTimeDelta;
		if (wInput & 0x0020)	fAngleX += m_fSpeed * fTimeDelta;
		if (wInput & 0x0040)	fAngleY -= m_fSpeed * fTimeDelta;
		if (wInput & 0x0080)	fAngleY += m_fSpeed * fTimeDelta;

		matRotAxis = XMMatrixRotationX(ToRadian(fAngleX));
		vRotLook = XMVector3TransformNormal(vRotLook, matRotAxis);

		matRotAxis = XMMatrixRotationY(ToRadian(fAngleY));
		vRotLook = XMVector3TransformNormal(vRotLook, matRotAxis);

		vTarget = vPos + vRotLook;

		XMMATRIX matView = XMMatrixLookAtLH(vPos, vTarget, XMVectorSet(0.f, 1.f, 0.f, 0.f));

		XMStoreFloat4x4(m_pView, matView);
		XMStoreFloat3(m_pTarget, vTarget);
	}

	// Zoom
	if (wInput & 0x0300)
	{
		// Zoom
		if ((wInput & 0x0100))
			vPos += vLook * m_fSpeed * fTimeDelta;

		if (wInput & 0x0200)
			vPos -= vLook * m_fSpeed * fTimeDelta;

		vTarget = vPos + vLook;

		XMMATRIX matView = XMMatrixLookAtLH(vPos, vTarget, XMVectorSet(0.f, 1.f, 0.f, 0.f));

		XMStoreFloat4x4(m_pView, matView);
		XMStoreFloat3(m_pPos, vPos);
		XMStoreFloat3(m_pTarget, vTarget);
	}

	Transform_Frustum();

	return 0;
}

void CDynamicCamera::Release(void)
{
	CCamera::Release();

	delete this;
}