#include "stdafx.h"
#include "Transform.h"

CTransform::CTransform(void)
	: m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_vPos(0.f, 0.f, 0.f)
	, m_vDir(0.f, 0.f, 1.f)
{
	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());
}

CTransform::~CTransform(void)
{
}

void CTransform::Update(FLOAT fTime)
{
	XMMATRIX matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	XMMATRIX matRotX = XMMatrixRotationX(m_vAngle.x);
	XMMATRIX matRotY = XMMatrixRotationY(m_vAngle.y);
	XMMATRIX matRotZ = XMMatrixRotationZ(m_vAngle.z);
	XMMATRIX matTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);

	XMStoreFloat4x4(&m_matWorld, matScale * matRotX * matRotY * matRotZ * matTrans);
}

void CTransform::Update_MatrixNotXRot(void)
{
	XMMATRIX matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	XMMATRIX matRotX = XMMatrixRotationX(m_vAngle.x);
	XMMATRIX matRotY = XMMatrixRotationY(m_vAngle.y);
	XMMATRIX matRotZ = XMMatrixRotationZ(m_vAngle.z);
	XMMATRIX matTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);

	XMStoreFloat4x4(&m_matWorld, matScale * matRotX * matRotY * matRotZ * matTrans);
}

CTransform* CTransform::Create(void)
{
	CTransform* pTransform = new CTransform();

	return pTransform;
}

void CTransform::Release(void)
{
	CComponent::Release();

	delete this;
}
