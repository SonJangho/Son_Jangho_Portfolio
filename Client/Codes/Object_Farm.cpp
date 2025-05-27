#include "stdafx.h"
#include "..\Headers\Object_Farm.h"
#include "Camera.h"
#include "Management.h"

CObject_Farm::CObject_Farm(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CObject_Farm::Ready_GameObject()
{
	if (FAILED(CGameObject::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(SetUp_Resource()))
		return E_FAIL;	

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;

	m_pPlayer->AddRef();

	Set_Scale(_vec3(1024.f, 1024.f, 1.f));
	Set_Position(_vec3(31.5f * 64.f, 0.f * 64.f, 0.f));

	return NOERROR;
}

_int CObject_Farm::Update_GameObject(const _float & fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene())
	{
		
	}
	
	return _int();
}

_int CObject_Farm::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene())
	{
		if (nullptr == m_pRender_Manager)
			return -1;

		if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_TWO, this)))
			return -1;
	}

	return _int();
}

void CObject_Farm::Render_GameObject()
{
	if (FAILED(SetUp_RenderState()))
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

	TempMatrix = Render_TransForm(pCamera);

	m_pTexture->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);


	if (FAILED(Reset_RenderState()))
		return;
}

HRESULT CObject_Farm::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;
	
	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_House");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	m_pRect = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Rect");
	if (nullptr == m_pRect)
		return E_FAIL;
	m_pRect->AddRef();

	return NOERROR;
}

HRESULT CObject_Farm::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CObject_Farm::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

_matrix CObject_Farm::Render_TransForm(CGameObject* pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

_bool CObject_Farm::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

CObject_Farm * CObject_Farm::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CObject_Farm*		pInstance = new CObject_Farm(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CObject_Farm Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Farm::Free()
{
	Safe_Release(m_pPlayer);
	
	Safe_Release(m_pTexture);

	Safe_Release(m_pRect);

	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
