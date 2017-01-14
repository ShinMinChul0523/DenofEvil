#include "stdafx.h"
#include "Monster.h"
#include "AnimationInfo.h"
#include "Transform.h"

CMonster::CMonster(ID3D11DeviceContext* pContext)
	: CGameObject(pContext)
	, m_pAnimInfo(nullptr)
	, m_pTransform(nullptr)
	, m_uiObjNum(0)
	, m_fSpeed(0.f)
	, m_fHeight(0.f)
{
}

CMonster::~CMonster(void)
{
}

HRESULT CMonster::Ready_Component(void)
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

INT CMonster::Update(const FLOAT& fTimeDelta)
{
	CGameObject::Update(fTimeDelta);
	return 0;
}

void CMonster::Release(void)
{
	CGameObject::Release();
}