#pragma once

#include "Resource.h"

class CShader final : public CResource
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CShader() = default;
public:
	LPD3DXEFFECT Get_EffectHandle() const {
		return m_pEffect; }
public:
	HRESULT Ready_Shader(const _tchar* pShaderFilePath);
private:
	LPD3DXEFFECT			m_pEffect = nullptr;
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
protected:
	virtual void Free();
};

