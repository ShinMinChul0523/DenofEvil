#include "stdafx.h"
#include "CameraMgr.h"
#include "Camera.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr(void)
	: m_eCurCamera(CAMERA_WALK)
{
	m_vecCamera.reserve(CAMERA_END);

	for (size_t i = 0; i < CAMERA_END; ++i)
		m_vecCamera.push_back(nullptr);
}


CCameraMgr::~CCameraMgr(void)
{
}

void CCameraMgr::Ready_DynamicCamera(ID3D11DeviceContext* pContext, CAMERALIST eCameraName, FLOAT fNear, FLOAT fFar, XMFLOAT3& vPos, XMFLOAT3& vTarget)
{
	if (m_vecCamera[eCameraName] == nullptr)
		m_vecCamera[eCameraName] = CDynamicCamera::Create(pContext, fNear, fFar, vPos, vTarget);
}

void CCameraMgr::Ready_StaticCamera(ID3D11DeviceContext* pContext, CAMERALIST eCameraName, const CTransform* pObjTrans, FLOAT fDist, FLOAT fHeightPivot
	, FLOAT fNear, FLOAT fFar, XMFLOAT3& vPos, XMFLOAT3& vTarget)
{
	if (m_vecCamera[eCameraName] == nullptr)
		m_vecCamera[eCameraName] = CStaticCamera::Create(pContext, pObjTrans, fDist, fHeightPivot, fNear, fFar, vPos, vTarget);
}

void CCameraMgr::Update_CurCamera(const FLOAT& fTimeDelta)
{
	m_vecCamera[m_eCurCamera]->Update(fTimeDelta);
}

void CCameraMgr::Release(void)
{
	for (size_t i = 0; i < CAMERA_END; ++i)
	{
		Safe_Release(m_vecCamera[i]);
	}

	m_vecCamera.clear();

	delete this;
}

const XMFLOAT4X4* CCameraMgr::Get_CurCameraProj(void)
{
	return m_vecCamera[m_eCurCamera]->Get_Proj();
}

const XMFLOAT4X4* CCameraMgr::Get_CurCameraView(void)
{
	return m_vecCamera[m_eCurCamera]->Get_View();
}

void CCameraMgr::Set_CurCamera(CAMERALIST eCameraName)
{
	m_eCurCamera = eCameraName;

	Ready_Frustum();
	m_vecCamera[m_eCurCamera]->Update(0);
}
