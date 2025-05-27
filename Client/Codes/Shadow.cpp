#include "stdafx.h"
#include "..\Headers\Shadow.h"
#include "Camera.h"

CShadow::CShadow(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CShadow::Ready_GameObject()
{
	CGameObject::Ready_GameObject();	

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;

	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	m_fX = m_pPlayer->Get_X();
	m_fY = m_pPlayer->Get_Y() + 55.f;

	Set_Scale(_vec3(64.0f, 32.0f, 1.f));

	Set_Position(_vec3(m_fX, m_fY, 0.f));

	return NOERROR;
}

_int CShadow::Update_GameObject(const _float & fTimeDelta)
{
	m_fX = m_pPlayer->Get_X();
	m_fY = m_pPlayer->Get_Y() + 55.f;

	Set_Position(_vec3(m_fX, m_fY, 0.f));

	return _int();
}

_int CShadow::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_TWO, this)))
		return -1;

	return _int();
}

void CShadow::Render_GameObject()
{
	if (nullptr == m_pTexture ||
		nullptr == m_pBuffer)
		return;

	SetUp_RenderState();

	const CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_fX, m_fY);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	m_pTexture->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);

	Reset_RenderState();
}

HRESULT CShadow::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Shadow");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	return NOERROR;
}

HRESULT CShadow::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CShadow::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

HRESULT CShadow::Animation(_float fTimeDelta)
{
	return NOERROR;
}

CGameObject * CShadow::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShadow*		pInstance = new CShadow(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CShadow Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow::Free(void)
{
	Safe_Release(m_pPlayer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
