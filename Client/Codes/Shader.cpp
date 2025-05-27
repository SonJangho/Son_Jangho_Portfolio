#include "stdafx.h"
#include "..\Headers\Shader.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CResource(pGraphic_Device)
{

}

HRESULT CShader::Ready_Shader(const _tchar * pShaderFilePath)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(D3DXCreateEffectFromFile(m_pGraphic_Device, pShaderFilePath, nullptr, nullptr, 0, nullptr, &m_pEffect, nullptr)))
		return E_FAIL;

	return NOERROR;
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pShaderFilePath)
{
	CShader*		pInstance = new CShader(pGraphic_Device);

	if (FAILED(pInstance->Ready_Shader(pShaderFilePath)))
	{
		_MSG_BOX("CShader Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShader::Free()
{
	Safe_Release(m_pEffect);

	CResource::Free();
}
