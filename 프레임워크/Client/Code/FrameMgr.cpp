#include "stdafx.h"
#include "FrameMgr.h"
#include "Frame.h"

IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr(void)
{
}

CFrameMgr::~CFrameMgr(void)
{
}

BOOL CFrameMgr::Get_Activate(const TCHAR* pFrameTag, const FLOAT& fTimeDelta)
{
	CFrame* pFrame = Find_Frame(pFrameTag);

	if (NULL == pFrame)
		return false;

	return pFrame->Get_Activate(fTimeDelta);
}

HRESULT CFrameMgr::Ready_Frame(const TCHAR* pFrameTag, const FLOAT& fFps)
{
	CFrame*		pFrame = Find_Frame(pFrameTag);

	if (NULL != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fFps);

	if (NULL == pFrame)
		return E_FAIL;

	m_mapFrame.insert(MAPFRAME::value_type(pFrameTag, pFrame));

	return S_OK;
}

CFrame* CFrameMgr::Find_Frame(const TCHAR* pFrameTag)
{
	MAPFRAME::iterator	iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), CTagFinder(pFrameTag));

	if (iter == m_mapFrame.end())
		return NULL;

	return iter->second;
}

void CFrameMgr::Release(void)
{
	for_each(m_mapFrame.begin(), m_mapFrame.end(), CRelease_Pair());
	m_mapFrame.clear();

	delete this;
}