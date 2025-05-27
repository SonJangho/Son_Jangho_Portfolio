#pragma once

#include "Base.h"
#include "Buffer_RcTex.h"
#include "Texture.h"
#include "Texture_Bmp.h"

class CResource_Manager final : public CBase
{
	_DECLARE_SINGLETON(CResource_Manager)
public:
	enum RESOURCETYPE { TYPE_STATIC, TYPE_LOGO, TYPE_STAGE, TYPE_END };	
private:
	explicit CResource_Manager();
	virtual ~CResource_Manager() = default;	
public:
	CResource* Get_Resource(RESOURCETYPE eType, const _tchar* pResourceTag) const;
public:
	HRESULT Add_Resource(RESOURCETYPE eType, const _tchar* pResourceTag, CResource* pResource);
	
public:	
	_ulong Clear_Resource(RESOURCETYPE eType);
private:
	// �ʼ������� : ���ҽ����� �̸�(Ű)�� ���صּ� Ű�� Ž���� �Ѵ�.
	// �迭�� ������ ���� : ���ҽ����� ������ ������ ���������ϱ�����.
	map<const _tchar*, CResource*>			m_mapResource[TYPE_END];
	typedef map<const _tchar*, CResource*>	MAPRESOURCE;
private:
	CResource* Find_Resource(RESOURCETYPE eType, const _tchar* pResourceTag) const;
protected:
	virtual void Free();
};

