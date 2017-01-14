#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"

CGameObject::CGameObject(ID3D11DeviceContext* pContext)
	: m_pContext(pContext)
{
}

CGameObject::~CGameObject(void)
{
}

HRESULT CGameObject::Initialize(void)
{
	return S_OK;
}

INT CGameObject::Update(const FLOAT& fTimeDelta)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Update(fTimeDelta);
	}

	return 0;
}

void CGameObject::Render(void)
{
}

HRESULT CGameObject::Ready_Component(void)
{
	return S_OK;
}

void CGameObject::Release(void)
{
	for_each(m_mapComponent.begin(), m_mapComponent.end(), CRelease_Pair());
	m_mapComponent.clear();
}

const CComponent* CGameObject::Get_Component(const TCHAR* pComponentTag)
{
	MAPCOMPONENT::iterator iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CTagFinder(pComponentTag));

	if (iter == m_mapComponent.end())
		return NULL;

	return iter->second;
}