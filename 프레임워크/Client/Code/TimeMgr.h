#ifndef TimeMgr_h__
#define TimeMgr_h__

#include "Define.h"

class CTimer;
class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	explicit CTimeMgr(void);
	~CTimeMgr(void);

public:
	FLOAT Get_TimeDelta(const TCHAR* pTimerTag);

public:
	HRESULT Ready_Timer(const TCHAR* pTimerTag);
	void Set_TimeDelta(const TCHAR* pTimerTag);

private:
	CTimer* Find_Timer(const TCHAR* pTimerTag);

public:
	void Release(void);

private:
	using MAPTIMER = map<const TCHAR*, CTimer*>;
	MAPTIMER m_mapTimer;
};

#endif // TimeMgr_h__
