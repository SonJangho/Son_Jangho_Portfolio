#pragma once

#include "Base.h"

// CTimer들을 여러개 모아서 보관한다.

class CTimer;
class CTimer_Manager : public CBase
{
	_DECLARE_SINGLETON(CTimer_Manager)
private:
	explicit CTimer_Manager();
	virtual ~CTimer_Manager() = default;
public:
	_float Get_TimeDelta(const _tchar* pTimerTag);
public:
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Compute_Timer(const _tchar* pTimerTag);
private:
	map<const _tchar*, CTimer*>			m_mapTimers;
	typedef map<const _tchar*, CTimer*>	MAPTIMERS;
private:
	CTimer*	Find_Timer(const _tchar * pTimerTag);
protected:
	virtual void Free();
};

