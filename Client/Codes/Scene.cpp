#include "stdafx.h"
#include "..\Headers\Scene.h"
#include "Object_Manager.h"
#include "Render_Manager.h"
#include "Resource_Manager.h"


CScene::CScene(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pRender_Manager(CRender_Manager::GetInstance())
	, m_pResource_Manager(CResource_Manager::GetInstance())
{
	m_pGraphic_Device->AddRef();
	m_pObject_Manager->AddRef();
	m_pRender_Manager->AddRef();
	m_pResource_Manager->AddRef();
}

HRESULT CScene::Ready_Scene()
{

	return NOERROR;
}

_int CScene::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	return m_pObject_Manager->Update_Layer(fTimeDelta);
}

_int CScene::LastUpdate_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	return m_pObject_Manager->LastUpdate_Layer(fTimeDelta);
}

void CScene::Render_Scene()
{
	if (nullptr == m_pRender_Manager)
		return;

	m_pRender_Manager->Render_Object();
}

void CScene::Free()
{
	if ( nullptr == m_pObject_Manager ||		
		nullptr == m_pRender_Manager)
		return;

	// 현재 씬에서 사용하기위해 만들었던 객체들을 해제한다.
	m_pObject_Manager->Clear_Dynamic_Object();	

	m_pRender_Manager->Clear_RenderList();

	Safe_Release(m_pGraphic_Device);

	Safe_Release(m_pResource_Manager);

	Safe_Release(m_pRender_Manager);

	Safe_Release(m_pObject_Manager);
}

