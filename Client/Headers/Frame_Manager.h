#pragma once

#include "Base.h"

class CFrame;
class CFrame_Manager final : public CBase
{
	_DECLARE_SINGLETON(CFrame_Manager)
private:
	explicit CFrame_Manager();
	virtual ~CFrame_Manager() = default;	
public:
	HRESULT Add_Frame(const _tchar* pFrameTag, const _uint& iCallCntPerSec);
	_bool Permit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
private:
	map<const _tchar*, CFrame*>			m_mapFrames;
	typedef map<const _tchar*, CFrame*>	MAPFRAMES;
private:
	CFrame* Find_Frame(const _tchar* pFrameTag);
public:
	virtual void Free();
};

