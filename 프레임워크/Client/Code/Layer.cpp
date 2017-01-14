#include "stdafx.h"
#include "Layer.h"
#include "GameObject.h"
#include "Component.h"

CLayer::CLayer(void)
{
}

CLayer::~CLayer(void)
{
}

const CComponent* CLayer::Get_Component(const TCHAR* pObjectTag, const TCHAR* pComponentTag, DWORD dwObjectIdx /* = 0*/)
{
	MAPOBJLIST::iterator	iter = find_if(m_mapObjlist.begin(), m_mapObjlist.end(), CTagFinder(pObjectTag));

	if (iter == m_mapObjlist.end())
		return NULL;

	OBJECTLIST::iterator	iterlist = iter->second.begin();
	OBJECTLIST::iterator	iterlist_end = iter->second.end();

	for (DWORD dwIndex = 0; dwIndex < dwObjectIdx; ++dwIndex)
	{
		++iterlist;

		if (iterlist == iterlist_end)
			return NULL;
	}

	return (*iterlist)->Get_Component(pComponentTag);
}

HRESULT CLayer::Ready_Object(const TCHAR* pObjectTag, CGameObject* pGameObject)
{
	list<CGameObject*>*	pObjectList = Find_ObjectList(pObjectTag);

	if (pObjectList == NULL)
	{
		list<CGameObject*> newObjectList;
		newObjectList.push_back(pGameObject);
		m_mapObjlist.insert(map<const TCHAR*, list<CGameObject*>>::value_type(pObjectTag, newObjectList));
	}

	else
	{
		pObjectList->push_back(pGameObject);
	}

	return S_OK;
}

INT CLayer::Update(const FLOAT& fTimeDelta)
{
	MAPOBJLIST::iterator iter		= m_mapObjlist.begin();
	MAPOBJLIST::iterator iter_end	= m_mapObjlist.end();

	INT iResult = 0;

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator iterList		= iter->second.begin();
		OBJECTLIST::iterator iterList_end	= iter->second.end();

		for (; iterList != iterList_end; ++iterList)
		{
			iResult = (*iterList)->Update(fTimeDelta);

			if (iResult & 0x80000000)
				return iResult;
		}
	}

	return iResult;
}

void CLayer::Render(void)
{
	MAPOBJLIST::iterator iter		= m_mapObjlist.begin();
	MAPOBJLIST::iterator iter_end	= m_mapObjlist.end();

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator iterList		= iter->second.begin();
		OBJECTLIST::iterator iterList_end	= iter->second.end();

		for (; iterList != iterList_end; ++iterList)
		{
			(*iterList)->Render();
		}
	}
}

CLayer* CLayer::Create(void)
{
	CLayer* pLayer = new CLayer();

	return pLayer;
}

void CLayer::Release(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)
			Safe_Release((*iterList));

		iter->second.clear();
	}

	m_mapObjlist.clear();

	delete this;
}

list<CGameObject*>* CLayer::Find_ObjectList(const TCHAR* pObjectTag)
{
	map<const TCHAR*, list<CGameObject*>>::iterator	iter = find_if(m_mapObjlist.begin(), m_mapObjlist.end(), CTagFinder(pObjectTag));

	if (iter == m_mapObjlist.end())
		return NULL;

	return &iter->second;
}