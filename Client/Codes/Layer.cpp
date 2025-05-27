#include "stdafx.h"
#include "..\Headers\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

CGameObject * CLayer::Get_Object(const _uint & iIndex) const
{
	if (m_ObjectList.size() <= iIndex)
		return nullptr;

	auto iter = m_ObjectList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return (*iter);
}

HRESULT CLayer::Ready_Layer()
{
	return NOERROR;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int		iExitCode = 0;

	for (auto& pGameObject : m_ObjectList)
	{
		if (nullptr != pGameObject)
		{	
			iExitCode = pGameObject->Update_GameObject(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}
	
	return iExitCode;
}

_int CLayer::LastUpdate_Layer(const _float& fTimeDelta)
{
	_int		iExitCode = 0;

	for (auto& pGameObject : m_ObjectList)
	{
		if (nullptr != pGameObject)
		{
			iExitCode = pGameObject->LastUpdate_GameObject(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}

	return iExitCode;
}

void CLayer::Render_Layer()
{
	for (auto& pGameObject : m_ObjectList)
	{
		if (nullptr != pGameObject)		
			pGameObject->Render_GameObject();					
	}
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
	{
		_MSG_BOX("CLayer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CLayer::Free()
{
	for (auto& pGameObject : m_ObjectList)
	{
		Safe_Release(pGameObject);
	}
	m_ObjectList.clear();
}

