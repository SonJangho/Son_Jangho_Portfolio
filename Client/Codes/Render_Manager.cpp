#include "stdafx.h"
#include "..\Headers\Render_Manager.h"
#include "GameObject.h"

_IMPLEMENT_SINGLETON(CRender_Manager)

CRender_Manager::CRender_Manager()
{
}

HRESULT CRender_Manager::Add_RenderList(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	if (RENDER_END <= eGroup)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pGameObject);

	// 원래 객체들의 주소는 오브젝트 매니져들이 가지고 있는것이 정상. 
	// 렌더매니져에 공유.
	pGameObject->AddRef();

	return NOERROR;
}

void CRender_Manager::Render_Object()
{
	Render_Priority();
	Render_Two();
	Render_YSort();
	Render_Cover();
	Render_UI();
	Render_INVENTORY();
	Render_Mouse();
}

void CRender_Manager::Clear_RenderList()
{
	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderList[i])
			Safe_Release(pGameObject);
		m_RenderList[i].clear();
	}
}

void CRender_Manager::Render_Priority()
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_PRIORITY].clear();
}

void CRender_Manager::Render_Two()
{
	for (auto& pGameObject : m_RenderList[RENDER_TWO])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_TWO].clear();
}

_bool Compare(CGameObject* pSour, CGameObject* pDest)
{
	return pSour->Get_CompareY() < pDest->Get_CompareY();
}


void CRender_Manager::Render_YSort()
{	
	m_RenderList[RENDER_YSORT].sort([](CGameObject* pSour, CGameObject* pDest)->_bool {return pSour->Get_BottomRect() < pDest->Get_BottomRect(); });

	for (auto& pGameObject : m_RenderList[RENDER_YSORT])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_YSORT].clear();
}

void CRender_Manager::Render_Cover()
{
	for (auto& pGameObject : m_RenderList[RENDER_COVER])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_COVER].clear();
}

void CRender_Manager::Render_UI()
{
	// 정렬

	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_UI].clear();
}

void CRender_Manager::Render_INVENTORY()
{
	for (auto& pGameObject : m_RenderList[RENDER_INVENTORY])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_INVENTORY].clear();
}

void CRender_Manager::Render_Mouse()
{
	for (auto& pGameObject : m_RenderList[RENDER_MOUSE])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_GameObject();
			Safe_Release(pGameObject);
		}
	}
	m_RenderList[RENDER_MOUSE].clear();
}

void CRender_Manager::Free(void)
{
	Clear_RenderList();
}
