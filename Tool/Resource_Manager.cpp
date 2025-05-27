#include "stdafx.h"
#include "Resource_Manager.h"
#include "Resources.h"

_IMPLEMENT_SINGLETON(CResource_Manager)

CResource_Manager::CResource_Manager()
{

}

CResource * CResource_Manager::Get_Resource(RESOURCETYPE eType, const _tchar * pResourceTag) const
{
	return Find_Resource(eType, pResourceTag);
}

HRESULT CResource_Manager::Add_Resource(RESOURCETYPE eType, const _tchar * pResourceTag, CResource * pResource)
{
	if (nullptr == pResource)
		return E_FAIL;

	CResource* pFindResource = Find_Resource(eType, pResourceTag);

	if (nullptr != pFindResource)
		return E_FAIL;

	m_mapResource[eType].insert(MAPRESOURCE::value_type(pResourceTag, pResource));

	return NOERROR;
}



_ulong CResource_Manager::Clear_Resource(RESOURCETYPE eType)
{
	_ulong		dwRefCnt = 0;

	for (auto& Pair : m_mapResource[eType])
	{		
		if (dwRefCnt = Safe_Release(Pair.second))
			return dwRefCnt;	
	}
	m_mapResource[eType].clear();

	return dwRefCnt;
}

CResource * CResource_Manager::Find_Resource(RESOURCETYPE eType, const _tchar * pResourceTag) const
{
	auto iter = m_mapResource[eType].find(pResourceTag);

	if(iter == m_mapResource[eType].end())
		return nullptr;

	return iter->second;
}

void CResource_Manager::Free()
{
	for (size_t i = 0; i < TYPE_END; i++)
	{
		for (auto& Pair : m_mapResource[i])
		{
			Safe_Release(Pair.second);
		}
		m_mapResource[i].clear();
	}
}

