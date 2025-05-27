#include "stdafx.h"
#include "..\Headers\Camera.h"


CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CCamera::Ready_GameObject()
{
	m_StartPos.fX = m_fX;
	m_StartPos.fY = m_fY;

	return NOERROR;
}

_int CCamera::Update_GameObject()
{
	return _int();
}

_int CCamera::LastUpdate_GameObject()
{
	return _int();
}

void CCamera::Render_GameObject()
{
}

POSITION CCamera::Transform(const _float & fX, const _float & fY) const
{
	// Converts world position to screen position based on current camera coordinates
	POSITION		Position_Transform;
	ZeroMemory(&Position_Transform, sizeof(POSITION));
	
	// Calculate the object's position relative to the camera's current location
	Position_Transform.fX = fX - m_fX + m_StartPos.fX;
	Position_Transform.fY = fY - m_fY + m_StartPos.fY;

	return Position_Transform;
}

void CCamera::Free()
{
	CGameObject::Free();
}

