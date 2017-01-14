#include "stdafx.h"
#include "TimeMgr.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr(void)
{
}

CTimeMgr::~CTimeMgr(void)
{
}

HRESULT CTimeMgr::Ready_Timer(const TCHAR* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (NULL != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();

	if (NULL == pTimer)
		return E_FAIL;

	m_mapTimer.insert(MAPTIMER::value_type(pTimerTag, pTimer));

	return S_OK;
}

void CTimeMgr::Set_TimeDelta(const TCHAR* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (NULL == pTimer)
		return;

	pTimer->Set_TimeDelta();
}

CTimer* CTimeMgr::Find_Timer(const TCHAR* pTimerTag)
{
	MAPTIMER::iterator iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), CTagFinder(pTimerTag));

	if (iter == m_mapTimer.end())
		return NULL;

	return iter->second;
}

void CTimeMgr::Release(void)
{
	for_each(m_mapTimer.begin(), m_mapTimer.end(), CRelease_Pair());
	m_mapTimer.clear();

	delete this;
}

FLOAT CTimeMgr::Get_TimeDelta(const TCHAR* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (NULL == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}
