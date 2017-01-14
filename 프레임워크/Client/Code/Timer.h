#ifndef Timer_h__
#define Timer_h__

class CTimer
{
private:
	explicit CTimer(void);
	virtual ~CTimer(void);

public:
	FLOAT Get_TimeDelta(void);

public:
	HRESULT Ready_Timer(void);
	void Set_TimeDelta(void);

public:
	static CTimer* Create(void);
	void Release(void);

private:
	LARGE_INTEGER	m_FrameTime;
	LARGE_INTEGER	m_FixTime;
	LARGE_INTEGER	m_LastTime;
	LARGE_INTEGER	m_CpuTick;

private:
	FLOAT			m_fTimeDelta;
};

#endif // Timer_h__
