#pragma once

template <typename T>
void Safe_Delete(T& pPointer)
{
	if (nullptr != pPointer)
	{
		delete pPointer;
		pPointer = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& pPointer)
{
	if (nullptr != pPointer)
	{
		delete [] pPointer;
		pPointer = nullptr;
	}
}



template <typename T>
unsigned long Safe_Release(T& pInstance)
{
	unsigned long dwRefCnt = 0;

	if (nullptr != pInstance)
	{
		dwRefCnt = pInstance->Release();
		pInstance = nullptr;
	}
	return dwRefCnt;
}