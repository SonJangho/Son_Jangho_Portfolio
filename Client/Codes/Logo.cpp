#include "stdafx.h"
#include "..\Headers\Logo.h"



CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	
}

HRESULT CLogo::Ready_GameObject()
{
	if (FAILED(CGameObject::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	Set_Scale(_vec3(g_iBackCX * 0.5f, g_iBackCY * 0.5f, 1.f));
	Set_Position(_vec3(g_iBackCX * 0.5f, g_iBackCY * 0.5f, 0.f));

	return NOERROR;
}

_int CLogo::Update_GameObject(const _float & fTimeDelta)
{
	m_fFrame += 4.f * fTimeDelta;

	if (m_fFrame >= 4.0f)
		m_fFrame = 0.f;

	return _int();
}

_int CLogo::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_YSORT, this)))
		return -1;

	return _int();
}

void CLogo::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture)
		return;

	if (FAILED(SetUp_RenderState()))
		return;

	m_pTexture->SetUp_OnGraphicDev(_uint(m_fFrame));

	m_pBuffer->Render_Buffer(m_matTransform);

	if (FAILED(Reset_RenderState()))
		return;
}

HRESULT CLogo::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	return NOERROR;
}

HRESULT CLogo::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//// 알파 테스트를 킨다.
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 알파블렌드를 킨다.
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// 내가 그려야할 픽셀의 색을 어떻게 혼합할까?!
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// 내가 그려야할 위치에 있었던 픽셀의 색을 어떻게 혼합할까?!
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	return NOERROR;
}

HRESULT CLogo::Reset_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return E_NOTIMPL;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLogo*		pInstance = new CLogo(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CLogo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLogo::Free()
{
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);

	CGameObject::Free();
}
