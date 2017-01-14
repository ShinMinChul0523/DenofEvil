#ifndef Functor_h__
#define Functor_h__

class CTagFinder
{
public:
	explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
	~CTagFinder(void) {}
public:
	template <typename T> bool operator () (T& Pair)
	{
		int iResult = lstrcmp(m_pTag, Pair.first);

		if (0 == iResult)
			return TRUE;
		return false;
	}
private:
	const TCHAR*				m_pTag;
};

class CNumFinder
{
public:
	explicit CNumFinder(UINT uiNum) : m_uiNum(uiNum) {}
	~CNumFinder(void) {}
public:
	template <typename T> bool operator () (T& Pair)
	{
		if (m_uiNum == Pair.first)
			return TRUE;

		return false;
	}
private:
	UINT				m_uiNum;
};

class CRelease_Pair
{
public:
	explicit CRelease_Pair() {}
	~CRelease_Pair(void) {}
public:
	template <typename T>
	void operator () (T& Pair)
	{
		if (NULL != Pair.second)
		{
			Pair.second->Release();
			Pair.second = NULL;
		}
	}
};

class CRelease_Obj
{
public:
	explicit CRelease_Obj(void) {}
	~CRelease_Obj(void) {}
public: // operator
	template <typename T> void operator () (T& pInstance)
	{
		if (NULL != pInstance)
		{
			pInstance->Release();
			pInstance = NULL;
		}
	}
};

#endif // Functor_h__