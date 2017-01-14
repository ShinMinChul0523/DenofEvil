#ifndef Frame_h__
#define Frame_h__

class CFrame
{
private:
	explicit CFrame(void);
	virtual ~CFrame(void);

public:
	BOOL Get_Activate(const FLOAT& fTimeDelta);

public:
	HRESULT Ready_Frame(const FLOAT& fFps);

public:
	static CFrame* Create(const FLOAT& fFps);

public:
	void Release(void);

private:
	FLOAT	m_fFpsRate;
	FLOAT	m_fAccFrame;
};

#endif // Frame_h__
