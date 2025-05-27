#pragma once

#include "Base.h"

class CResource abstract : public CBase
{
protected:
	explicit CResource(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CResource() = default;
protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
protected:
	virtual void Free();
};

