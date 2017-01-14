#include "stdafx.h"
#include "Equipment.h"

CEquipment::CEquipment(ID3D11DeviceContext* pContext)
	: CGameObject(pContext)
	, m_pTransform(NULL)
{
	XMStoreFloat4x4(&m_matParent, XMMatrixIdentity());
}

CEquipment::~CEquipment(void)
{
}

HRESULT CEquipment::Initialize(void)
{
	return S_OK;
}

void CEquipment::Release(void)
{
	CGameObject::Release();
}

void CEquipment::SetParent(const XMFLOAT4X4& matParent)
{
	m_matParent = matParent;
}