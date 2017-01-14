#ifndef AnimationInfo_h__
#define AnimationInfo_h__

#include "Component.h"

class CAnimationInfo
	: public CComponent
{
public:
	typedef struct tagFrameCnt {
		WORD wFrameCnt;
		WORD wFrameSpeed;
	}FRAMEINFO;

	using VECFRAMEINFO = vector<FRAMEINFO>;

private:
	explicit CAnimationInfo(MESHNUM eMeshNum);
	virtual ~CAnimationInfo(void);

public:
	static void Push_FrameInfo(VECFRAMEINFO& vecFrameInfo);

public:
	const WORD& Get_CurKey(void);
	const WORD& Get_NextKey(void);
	const WORD& Get_CurFrame(void);
	const WORD& Get_NextFrame(void);
	const FLOAT& Get_Ratio(void);
	BOOL Get_Playing(void);

public:
	void Set_Key(WORD wKey);

public:
	static CAnimationInfo* Create(MESHNUM eMeshNum);
	virtual void Update(FLOAT fTime);
	virtual void Release(void);

private:
	static vector<VECFRAMEINFO> m_vecMeshFrameInfo;

private:
	MESHNUM				m_eMeshNum;
	WORD				m_wCurKey;
	WORD				m_wNextKey;
	WORD				m_wStoreNextKey;
	WORD				m_wCurFrame;
	WORD				m_wNextFrame;
	FLOAT				m_fRatio;
};

#endif // AnimationInfo_h__