#pragma once

#define _MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK);

#define _DECLARE_SINGLETON(CLASSNAME)									\
public:																	\
	static CLASSNAME* GetInstance();									\
	unsigned long DestroyInstance();									\
private:																\
	static CLASSNAME*		m_pInstance;

#define _IMPLEMENT_SINGLETON(CLASSNAME)									\
	CLASSNAME*	CLASSNAME::m_pInstance = nullptr;						\
	CLASSNAME* CLASSNAME::GetInstance() {								\
		if(nullptr == m_pInstance) {									\
			m_pInstance = new CLASSNAME;								\
		}																\
		return m_pInstance;												\
	}																	\
	unsigned long CLASSNAME::DestroyInstance() {						\
		unsigned long dwRefCnt = 0;										\
		if(nullptr != m_pInstance) {									\
			dwRefCnt = m_pInstance->Release();							\
			m_pInstance = nullptr;										\
		}																\
		return dwRefCnt;												\
	}



