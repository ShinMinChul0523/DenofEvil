#include "stdafx.h"
#include "StaticCamera.h"
#include "Export_Utility.h"
#include "Transform.h"

CStaticCamera::CStaticCamera(ID3D11DeviceContext* pContext, const CTransform* pObjTrans, FLOAT fDist, FLOAT fHeightPivot)
	: CCamera(pContext)
	, m_pObjTrans(pObjTrans)
	, m_fDist(fDist)
	, m_fHeightPivot(fHeightPivot)
{
}

CStaticCamera::~CStaticCamera(void)
{
}

CStaticCamera* CStaticCamera::Create(ID3D11DeviceContext* pContext, const CTransform* pObjTrans, FLOAT fDist, FLOAT fHeightPivot,
	FLOAT fNear, FLOAT fFar, XMFLOAT3& vPos, XMFLOAT3& vTarget)
{
	CStaticCamera* pCamera = new CStaticCamera(pContext, pObjTrans, fDist, fHeightPivot);

	if (FAILED(pCamera->Initialize(fNear, fFar, vPos, vTarget)))
		Safe_Release(pCamera);

	return pCamera;
}

INT CStaticCamera::Update(const FLOAT& fTimeDelta)
{
	// Target
	XMVECTOR vTarget = XMVectorSet(m_pObjTrans->m_vPos.x, m_pObjTrans->m_vPos.y + m_fHeightPivot, m_pObjTrans->m_vPos.z, 1.f);

	XMVECTOR vObjDir = XMVectorSet(m_pObjTrans->m_vDir.x, m_pObjTrans->m_vDir.y - 0.1f, m_pObjTrans->m_vDir.z, 0.f);
	vObjDir = XMVector3Normalize(vObjDir);

	// Pos
	XMVECTOR vPos = vTarget - (vObjDir * 5.f * m_fDist);

	XMFLOAT3 vPosF3;
	XMStoreFloat3(&vPosF3, vPos);

	FLOAT fTerrainY = Get_TerrainColY(vPosF3);

	if (fTerrainY > vPosF3.y)
	{
		vPosF3.y = fTerrainY;

		FLOAT fLength = fTerrainY - XMVectorGetY(vTarget);

		// Y값 차이가 (m_fDist * 5.f)m 일 때
		if (fLength > m_fDist * 5.f)
		{
			XMStoreFloat3(&vPosF3, vTarget);
			vPosF3.y += m_fDist * 5.f;

			XMFLOAT3 vDirF3;
			XMVECTOR vDirTemp = vObjDir;
			XMVectorSetY(vDirTemp, 0.f);
			vDirTemp = XMVector3Normalize(vDirTemp);

			XMStoreFloat3(&vDirF3, -vDirTemp);
			vDirF3.y = 0.f;

			vPosF3 = Pick_Terrain(vPosF3, vDirF3);

			vDirTemp = vTarget - vPos;
			XMVectorSetY(vDirTemp, 0.f);
			XMStoreFloat3(&vDirF3, XMVector3Normalize(vDirTemp));
			vPosF3 = XMFLOAT3(vPosF3.x + vDirF3.x, vPosF3.y + vDirF3.y, vPosF3.z + vDirF3.z);
		}

		vPosF3.y += 0.5f;
	}

	vPos = XMLoadFloat3(&vPosF3);

	// Set View
	XMMATRIX matView = XMMatrixLookAtLH(vPos, vTarget, XMVectorSet(0.f, 1.f, 0.f, 0.f));

	XMStoreFloat4x4(m_pView, matView);
	XMStoreFloat3(m_pPos, vPos);
	XMStoreFloat3(m_pTarget, vTarget);

	Transform_Frustum();

	return 0;
}

void CStaticCamera::Release(void)
{
	CCamera::Release();

	delete this;
}