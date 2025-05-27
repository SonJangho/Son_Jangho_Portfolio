#include "stdafx.h"
#include "..\Headers\FocusCam.h"

CFocusCam::CFocusCam(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

HRESULT CFocusCam::SetUp_Focus(CGameObject * pTarget)
{
	if (nullptr != m_pTarget)
		Safe_Release(m_pTarget);

	m_pTarget = pTarget;
	m_pTarget->AddRef();
	
	m_fX = m_pTarget->Get_X();
	m_fY = m_pTarget->Get_Y();
	m_fSizeX = g_iBackCX;
	m_fSizeY = g_iBackCY;

	Set_Scale(_vec3(m_fSizeX, m_fSizeY, 1.f));
	Set_Position(_vec3(m_fX, m_fY, 0.f));

	m_dwTime = GetTickCount();

	return NOERROR;
}

HRESULT CFocusCam::Ready_GameObject(const _float & fScrollMinX, const _float & fScrollMaxX, const _float & fScrollMinY, const _float & fScrollMaxY)
{
	m_fX = g_iBackCX >> 1;
	m_fY = g_iBackCY >> 1;

	CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr != m_pTarget)
	{
		m_fX = m_pTarget->Get_X();
		m_fY = m_pTarget->Get_Y();
	}

	m_ScrollMin.fX = fScrollMinX;
	m_ScrollMin.fY = fScrollMinY;

	m_ScrollMax.fX = fScrollMaxX;
	m_ScrollMax.fY = fScrollMaxY;

	CCamera::Ready_GameObject();

	return NOERROR;
}

_int CFocusCam::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTarget)	
		return -1;

	// First update: snap to target
	if (false == m_bFirst)
	{
		m_bFirst = true;
		m_X = m_pTarget->Get_X();
		m_Y = m_pTarget->Get_Y();
	}

	// WASD input: offset camera focus
	if (GetKeyState('W') & 0x8000)
	{
		m_X = m_pTarget->Get_X();
		m_Y = m_pTarget->Get_Y() - 64.f;
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_X = m_pTarget->Get_X() + 64.f;
		m_Y = m_pTarget->Get_Y();
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_X = m_pTarget->Get_X();
		m_Y = m_pTarget->Get_Y() + 64.f;
	}
	if (GetKeyState('A') & 0x8000)
	{
		m_X = m_pTarget->Get_X() - 64.f;
		m_Y = m_pTarget->Get_Y();
	}

	// Lerp to target
	m_fX = Linear(m_fX, m_X, 0.05f);
	m_fY = Linear(m_fY, m_Y, 0.05f);

	// Clamp to map bounds
	if (m_fX >= m_ScrollMax.fX)
		m_fX = m_ScrollMax.fX;
	if (m_fX <= m_ScrollMin.fX)
		m_fX = m_ScrollMin.fX;
	if (m_fY >= m_ScrollMax.fY)
		m_fY = m_ScrollMax.fY;
	if (m_fY <= m_ScrollMin.fY)
		m_fY = m_ScrollMin.fY;

	// Apply camera position
	Set_Position(_vec3(m_fX, m_fY, 0.f));

	return _int();
}

_int CFocusCam::LastUpdate_GameObject()
{
	return _int();
}

void CFocusCam::Render_GameObject()
{
}

CFocusCam * CFocusCam::Create(LPDIRECT3DDEVICE9 pGraphic_Device,const _float & fScrollMinX, const _float & fScrollMaxX, const _float & fScrollMinY, const _float & fScrollMaxY)
{
	CFocusCam*		pInstance = new CFocusCam(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject(fScrollMinX, fScrollMaxX, fScrollMinY, fScrollMaxY)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFocusCam::Free()
{
	Safe_Release(m_pTarget);

	CCamera::Free();
}
