#include "stdafx.h"
#include "Frustum.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum(void)
{
	ZeroMemory(m_vOriPoint, sizeof(D3DXVECTOR3) * 8);
	ZeroMemory(m_vConPoint, sizeof(D3DXVECTOR3) * 8);
}

CFrustum::~CFrustum(void)
{
}

HRESULT CFrustum::Ready_Frustum(void)
{
	//¾Õ¸é
	m_vOriPoint[0] = D3DXVECTOR3(-1.f, 1.f, 0.f);
	m_vOriPoint[1] = D3DXVECTOR3(1.f, 1.f, 0.f);
	m_vOriPoint[2] = D3DXVECTOR3(1.f, -1.f, 0.f);
	m_vOriPoint[3] = D3DXVECTOR3(-1.f, -1.f, 0.f);

	//µÞ¸é
	m_vOriPoint[4] = D3DXVECTOR3(-1.f, 1.f, 1.f);
	m_vOriPoint[5] = D3DXVECTOR3(1.f, 1.f, 1.f);
	m_vOriPoint[6] = D3DXVECTOR3(1.f, -1.f, 1.f);
	m_vOriPoint[7] = D3DXVECTOR3(-1.f, -1.f, 1.f);

	XMFLOAT4X4 matProj;
	XMStoreFloat4x4(&matProj, XMMatrixPerspectiveFovLH(ToRadian(45.f), FLOAT(WINCX) / FLOAT(WINCY), 0.1f, 100.f));

	D3DXMATRIX matProjDX;
	memcpy(&matProjDX, &matProj, sizeof(XMFLOAT4X4));

	D3DXMatrixInverse(&matProjDX, NULL, &matProjDX);

	for (INT i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vOriPoint[i], &m_vOriPoint[i], &matProjDX);

	return S_OK;
}

void CFrustum::Transform_Frustum(void)
{
	memcpy(m_vConPoint, m_vOriPoint, sizeof(D3DXVECTOR3) * 8);

	D3DXMATRIX matView;
	memcpy(&matView, Get_CurCameraView(), sizeof(XMFLOAT4X4));

	D3DXMatrixInverse(&matView, NULL, &matView);

	for (INT i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vConPoint[i], &m_vConPoint[i], &matView);

	ZeroMemory(m_Plane, sizeof(D3DXPLANE) * 6);

	// +x
	D3DXPlaneFromPoints(&m_Plane[0], &m_vConPoint[1], &m_vConPoint[5], &m_vConPoint[6]);
	// -x
	D3DXPlaneFromPoints(&m_Plane[1], &m_vConPoint[4], &m_vConPoint[0], &m_vConPoint[3]);
	// +y
	D3DXPlaneFromPoints(&m_Plane[2], &m_vConPoint[4], &m_vConPoint[5], &m_vConPoint[1]);
	// -y
	D3DXPlaneFromPoints(&m_Plane[3], &m_vConPoint[3], &m_vConPoint[2], &m_vConPoint[6]);
	// +z
	D3DXPlaneFromPoints(&m_Plane[4], &m_vConPoint[4], &m_vConPoint[6], &m_vConPoint[5]);
	// -z
	D3DXPlaneFromPoints(&m_Plane[5], &m_vConPoint[0], &m_vConPoint[1], &m_vConPoint[2]);
}

BOOL CFrustum::Sphere_InFrustum(const XMFLOAT3* pCenter, const FLOAT& fRadius) const
{
	D3DXVECTOR3 vCenter;
	memcpy(&vCenter, pCenter, sizeof(XMFLOAT3));

	for (INT i = 0; i < 6; ++i)
	{
		if (D3DXPlaneDotCoord(&m_Plane[i], &vCenter) > fRadius)
			return FALSE;
	}

	return TRUE;
}

BOOL CFrustum::Vertex_InFrustum(const XMFLOAT3* pPoint) const
{
	D3DXVECTOR3 vPoint;
	memcpy(&vPoint, pPoint, sizeof(XMFLOAT3));

	for (INT i = 0; i < 6; ++i)
	{
		if (D3DXPlaneDotCoord(&m_Plane[i], &vPoint) > -1)
			return FALSE;
	}

	return TRUE;
}

void CFrustum::Release(void)
{
	delete this;
}
