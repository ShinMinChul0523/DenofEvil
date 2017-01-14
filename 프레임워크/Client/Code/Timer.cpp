#include "stdafx.h"
#include "Timer.h"
#include "Define.h"

CTimer::CTimer(void)
	: m_fTimeDelta(0.f)
{
}

CTimer::~CTimer(void)
{
}

HRESULT CTimer::Ready_Timer(void)
{
	//  cpu�� ������� ī������ ����
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceCounter(&m_FixTime);

	// cpu�� 1�ʴ� ������ų �� �ִ� ����
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void CTimer::Set_TimeDelta(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart > m_CpuTick.QuadPart)
	{
		// cpu�� 1�ʴ� ������ų �� �ִ� ����
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = FLOAT(m_FrameTime.QuadPart - m_LastTime.QuadPart) / m_CpuTick.QuadPart;

	m_LastTime = m_FrameTime;
}

CTimer* CTimer::Create(void)
{
	CTimer* pTimer = new CTimer;

	if (FAILED(pTimer->Ready_Timer()))
	{
		MSG_BOX("CTimer Create Failed");
		Safe_Release(pTimer);
	}

	return pTimer;
}

void CTimer::Release(void)
{
	delete this;
}

FLOAT CTimer::Get_TimeDelta(void)
{
	return m_fTimeDelta;
}
