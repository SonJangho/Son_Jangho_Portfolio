#pragma once

#include "Defines.h"

class CBase abstract 
{
protected:
	explicit CBase();
	virtual ~CBase() = default;
public:
	_ulong AddRef();
	_ulong Release();
public:
	virtual _int	Get_Scene();
protected:
	_ulong		m_dwRefCnt = 0;
protected:
	virtual void Free() = 0;
};

