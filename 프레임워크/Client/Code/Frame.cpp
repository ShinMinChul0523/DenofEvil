#include "stdafx.h"
#include "Frame.h"
#include "Define.h"

CFrame::CFrame(void)
: m_fFpsRate(0.0f)
, m_fAccFrame(0.f)
{
}

CFrame::~CFrame(void)
{
}

BOOL CFrame::Get_Activate(const FLOAT& fTimeDelta)
{
	m_fAccFrame += fTimeDelta;

	if (m_fAccFrame > m_fFpsRate)
	{
		m_fAccFrame = 0.f;
		return TRUE;
	}

	return false;
}

HRESULT CFrame::Ready_Frame(const FLOAT& fFps)
{
	m_fFpsRate = 1.f / fFps;

	return S_OK;
}

CFrame* CFrame::Create(const FLOAT& fFps)
{
	CFrame*		pFrame = new CFrame;

	if (FAILED(pFrame->Ready_Frame(fFps)))
	{
		MSG_BOX("CFrame Create Failed");
		Safe_Release(pFrame);
	}

	return pFrame;
}

void CFrame::Release(void)
{
	delete this;
}