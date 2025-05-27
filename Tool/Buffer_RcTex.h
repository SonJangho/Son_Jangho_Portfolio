#pragma once

#include "VIBuffer.h"

class CBuffer_RcTex final : public CVIBuffer
{
private:
	explicit CBuffer_RcTex(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBuffer_RcTex() = default;
public:
	HRESULT Ready_Buffer_RcTex();
public:
	static CBuffer_RcTex* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

