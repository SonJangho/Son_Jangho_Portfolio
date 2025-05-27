#include "stdafx.h"
#include "..\Headers\Object_House.h"
#include "Camera.h"

CObject_House::CObject_House(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CObject_House::Ready_GameObject()
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

	return NOERROR;
}

_int CObject_House::Update_GameObject(const _float & fTimeDelta)
{
	
	return _int();
}

_int CObject_House::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_YSORT, this)))
		return -1;

	return _int();
}

void CObject_House::Render_GameObject()
{
	if (FAILED(SetUp_RenderState()))
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");


	Set_Scale(_vec3(32.f*4.f, 64.f*4.f, 1.f));
	Set_Position(_vec3(11.f*64.f, 11.f*64.f, 0.f));

	TempMatrix = Render_TransForm(pCamera);

	m_pFurniture[0]->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);

	if (GetKeyState(VK_F1))
	{
		m_pRect->SetUp_OnGraphicDev(0);
		m_pBuffer->Render_Buffer(TempMatrix);
	}

	Set_Scale(_vec3(32.f*4.f, 64.f*4.f, 1.f));
	Set_Position(_vec3(19.f*64.f, 15.f*64.f, 0.f));

	TempMatrix = Render_TransForm(pCamera);

	m_pFurniture[1]->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);

	if (GetKeyState(VK_F1))
	{
		m_pRect->SetUp_OnGraphicDev(0);
		m_pBuffer->Render_Buffer(TempMatrix);
	}
	
	//Bed1
	Set_Scale(_vec3(32.f*4.f, 64.f*4.f, 1.f));
	Set_Position(_vec3(19.f*64.f, 15.f*64.f, 0.f));

	TempMatrix = Render_TransForm(pCamera);

	m_pFurniture[2]->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);

	if (GetKeyState(VK_F1))
	{
		m_pRect->SetUp_OnGraphicDev(0);
		m_pBuffer->Render_Buffer(TempMatrix);
	}

	//Table
	Set_Scale(_vec3(32.f*4.f, 64.f*4.f, 1.f));
	Set_Position(_vec3(15.f*64.f, 11.2f*64.f, 0.f));

	TempMatrix = Render_TransForm(pCamera);

	m_pFurniture[3]->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);

	if (GetKeyState(VK_F1))
	{
		m_pRect->SetUp_OnGraphicDev(0);
		m_pBuffer->Render_Buffer(TempMatrix);
	}


	//Chair
	Set_Scale(_vec3(16.f*4.f, 32.f*4.f, 1.f));
	Set_Position(_vec3(13.5f*64.f, 11.2f*64.f, 0.f));

	TempMatrix = Render_TransForm(pCamera);

	m_pFurniture[4]->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);

	if (GetKeyState(VK_F1))
	{
		m_pRect->SetUp_OnGraphicDev(0);
		m_pBuffer->Render_Buffer(TempMatrix);
	}

	//Fireplace
	Set_Scale(_vec3(32.f*4.f, 128.f*4.f, 1.f));
	Set_Position(_vec3(18.f*64.f, 8.f*64.f, 0.f));

	TempMatrix = Render_TransForm(pCamera);

	m_pFurniture[5]->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);

	if (GetKeyState(VK_F1))
	{
		m_pRect->SetUp_OnGraphicDev(0);
		m_pBuffer->Render_Buffer(TempMatrix);
	}

	CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
	
	//Window
	Set_Scale(_vec3(16.f*4.f, 32.f*4.f, 1.f));
	Set_Position(_vec3(12.5f*64.f, 9.f*64.f, 0.f));

	TempMatrix = Render_TransForm(pCamera);

	if (true == pTarget->Get_RainDay() || 90 + 9 * 15 <=pTarget->Get_TimeDegree())
		m_pFurniture[6]->SetUp_OnGraphicDev(1);
	else
		m_pFurniture[6]->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);

	if (GetKeyState(VK_F1))
	{
		m_pRect->SetUp_OnGraphicDev(0);
		m_pBuffer->Render_Buffer(TempMatrix);
	}

	//Carpet
	Set_Scale(_vec3(64.f*4.f, 32.f*4.f, 1.f));
	Set_Position(_vec3(15.f*64.f, 15.f*64.f, 0.f));

	TempMatrix = Render_TransForm(pCamera);

	m_pFurniture[7]->SetUp_OnGraphicDev(0);
	m_pBuffer->Render_Buffer(TempMatrix);

	if (GetKeyState(VK_F1))
	{
		m_pRect->SetUp_OnGraphicDev(0);
		m_pBuffer->Render_Buffer(TempMatrix);
	}


	if (FAILED(Reset_RenderState()))
		return;
}

HRESULT CObject_House::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;
	
	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	m_pFurniture[0] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_TV");
	if (nullptr == m_pFurniture[0])
		return E_FAIL;
	m_pFurniture[0]->AddRef();

	m_pFurniture[1] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bed0");
	if (nullptr == m_pFurniture[1])
		return E_FAIL;
	m_pFurniture[1]->AddRef();

	m_pFurniture[2] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Bed1");
	if (nullptr == m_pFurniture[2])
		return E_FAIL;
	m_pFurniture[2]->AddRef();

	m_pFurniture[3] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Table");
	if (nullptr == m_pFurniture[3])
		return E_FAIL;
	m_pFurniture[3]->AddRef();

	m_pFurniture[4] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Chair");
	if (nullptr == m_pFurniture[4])
		return E_FAIL;
	m_pFurniture[4]->AddRef();

	m_pFurniture[5] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Fireplace");
	if (nullptr == m_pFurniture[5])
		return E_FAIL;
	m_pFurniture[5]->AddRef();

	m_pFurniture[6] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Window");
	if (nullptr == m_pFurniture[6])
		return E_FAIL;
	m_pFurniture[6]->AddRef();

	m_pFurniture[7] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Carpet");
	if (nullptr == m_pFurniture[7])
		return E_FAIL;
	m_pFurniture[7]->AddRef();

	m_pRect = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Rect");
	if (nullptr == m_pRect)
		return E_FAIL;
	m_pRect->AddRef();

	return NOERROR;
}

HRESULT CObject_House::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CObject_House::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

_matrix CObject_House::Render_TransForm(CGameObject* pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

_bool CObject_House::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

CObject_House * CObject_House::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CObject_House*		pInstance = new CObject_House(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CObject_House Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_House::Free()
{
	Safe_Release(m_pPlayer);
	
	for (auto& pFurniture : m_pFurniture)
		Safe_Release(pFurniture);

	Safe_Release(m_pRect);

	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
