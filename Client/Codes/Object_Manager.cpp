#include "stdafx.h"
#include "..\Headers\Object_Manager.h"
#include "Layer.h"

_IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

CGameObject * CObject_Manager::Get_Object(OBJECTTYPE eType, const _tchar * pLayerTag, const _uint & iCnt)
{
	CLayer*	pLayer = Find_Layer(eType, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Object(iCnt);	
}

HRESULT CObject_Manager::Add_Object(OBJECTTYPE eType, const _tchar * pLayerTag, CGameObject * pGameObject)
{
	if (TYPE_END <= eType)
		return E_FAIL;

	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*	pLayer = Find_Layer(eType, pLayerTag);

	if (nullptr == pLayer) // 레이어가 없으면 레이어를 만들자. 만든 레이어에 객체를 추가하자.
	{
		pLayer = CLayer::Create();

		if (nullptr == pLayer)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;

		m_mapLayer[eType].insert(MAPLAYER::value_type(pLayerTag, pLayer));
	}

	else // 레이어가 있으면 그 레이어에 객체를 추가하자.
	{
		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;
	}

	return NOERROR;
}

_int CObject_Manager::Update_Layer(const _float& fTimeDelta)
{
	_int		iExitCode = 0;

	for (size_t i = 0; i < TYPE_END; i++)
	{
		for (auto& Pair : m_mapLayer[i])
		{
			if (nullptr != Pair.second)
			{
				iExitCode = Pair.second->Update_Layer(fTimeDelta);
				if (iExitCode & 0x80000000)
					return iExitCode;
			}
		}
	}	

	return iExitCode;
}

_int CObject_Manager::LastUpdate_Layer(const _float& fTimeDelta)
{
	_int		iExitCode = 0;

	for (size_t i = 0; i < TYPE_END; i++)
	{
		for (auto& Pair : m_mapLayer[i])
		{
			if (nullptr != Pair.second)
			{
				iExitCode = Pair.second->LastUpdate_Layer(fTimeDelta);
				if (iExitCode & 0x80000000)
					return iExitCode;
			}
		}
	}
	return iExitCode;
}

void CObject_Manager::Render_Layer()
{
	for (size_t i = 0; i < TYPE_END; i++)
	{
		for (auto& Pair : m_mapLayer[i])
		{
			if (nullptr != Pair.second)			
				Pair.second->Render_Layer();							
		}
	}
}

_ulong CObject_Manager::Clear_Dynamic_Object()
{
	for (auto& Pair : m_mapLayer[TYPE_DYNAMIC])
	{
		Safe_Release(Pair.second);
	}
	m_mapLayer[TYPE_DYNAMIC].clear();

	return _ulong();
}

void CObject_Manager::Clear_Layer(OBJECTTYPE eType, const _tchar * pLayerTag)
{
	auto iter = m_mapLayer[eType].find(pLayerTag);

	if (iter == m_mapLayer[eType].end())
		return;

	// 레이어를 삭제한다ㅣ.
	Safe_Release(iter->second);

	m_mapLayer[eType].erase(iter);

	return;
}

CLayer * CObject_Manager::Find_Layer(OBJECTTYPE eType, const _tchar * pLayerTag)
{
	//auto iter = m_mapLayer[eType].find(pLayerTag);

	//if (iter == m_mapLayer[eType].end())
	//	return nullptr;

	//return iter->second;

	auto iter = find_if(m_mapLayer[eType].begin(), m_mapLayer[eType].end(), CFinder_Tag(pLayerTag));

	if (iter == m_mapLayer[eType].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free(void)
{
	for (size_t i = 0; i < TYPE_END; i++)
	{
		for (auto& Pair : m_mapLayer[i])
		{
			Safe_Release(Pair.second);
		}
		m_mapLayer[i].clear();
	}
}
