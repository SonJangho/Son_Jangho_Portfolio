#include "stdafx.h"
#include "Resources.h"


CResource::CResource(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

void CResource::Free()
{
	Safe_Release(m_pGraphic_Device);
}

