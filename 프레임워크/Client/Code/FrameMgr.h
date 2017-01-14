#ifndef FrameMgr_h__
#define FrameMgr_h__

#include "Define.h"

class CFrame;
class CFrameMgr
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	explicit CFrameMgr(void);
	~CFrameMgr(void);

public:
	BOOL Get_Activate(const TCHAR* pFrameTag, const FLOAT& fTimeDelta);

public:
	HRESULT Ready_Frame(const TCHAR* pFrameTag, const FLOAT& fFps);

private:
	CFrame* Find_Frame(const TCHAR* pFrameTag);

public:
	void Release(void);

private:
	using MAPFRAME = map<const TCHAR*, CFrame*>;
	MAPFRAME m_mapFrame;
};

#endif // FrameMgr_h__
