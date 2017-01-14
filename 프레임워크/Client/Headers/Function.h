#ifndef Function_h__
#define Function_h__

template <typename T> void Safe_Delete(T& pointer)
{
	if (NULL != pointer)
	{
		delete pointer;
		pointer = NULL;
	}
}

template <typename T> void Safe_Delete_Array(T& pointer)
{
	if (NULL != pointer)
	{
		delete[] pointer;
		pointer = NULL;
	}
}

template <typename T> DWORD Safe_Com_Release(T& pointer)
{
	DWORD	iRefCnt = 0;

	if (NULL != pointer)
	{
		iRefCnt = pointer->Release();

		if (0 == iRefCnt)
			pointer = NULL;
	}

	return iRefCnt;
}

template <typename T> void Safe_Release(T& pointer)
{
	if (NULL != pointer)
	{
		pointer->Release();
		pointer = NULL;
	}
}

#endif // Function_h__