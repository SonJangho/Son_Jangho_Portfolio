#include "stdafx.h"
#include "..\Headers\Mouse.h"


CMouse::CMouse(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CMouse::Ready_GameObject()
{
	CGameObject::Ready_GameObject();	

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	m_fX = 0.f;
	m_fY = 0.f;

	Set_Scale(_vec3(128.0f, 128.0f, 1.f));

	Set_Position(_vec3(m_fX, m_fY, 0.f));

	ShowCursor(FALSE);

	return NOERROR;
}

_int CMouse::Update_GameObject(const _float & fTimeDelta)
{
	POINT		ptMouse;
	ZeroMemory(&ptMouse, sizeof(POINT));

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	m_fX = ptMouse.x;
	m_fY = ptMouse.y;

	Set_Position(_vec3(m_fX, m_fY, 0.f));

	RECT rc;
	POINT p1, p2;
	GetClientRect(g_hWnd, &rc);

	p1.x = rc.left;
	p1.y = rc.top;
	p2.x = rc.right;
	p2.y = rc.bottom;

	ClientToScreen(g_hWnd, &p1);
	ClientToScreen(g_hWnd, &p2);

	rc.left = p1.x + 1;
	rc.top = p1.y + 1;
	rc.right = p2.x - 1;
	rc.bottom = p2.y - 1;

	ClipCursor(&rc);
	
	CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Farming");
	CGameObject* pShop = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_ShopObject", 19);

	m_fFrame = 0;

	if (nullptr != pTarget && pTarget->Get_HarvestCheck())
		m_fFrame = 1;
	else if(nullptr != pShop && pShop->Get_ShopOn())
		m_fFrame = 3;

	return _int();
}

_int CMouse::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_MOUSE, this)))
		return -1;

	return _int();
}

void CMouse::Render_GameObject()
{
	if (nullptr == m_pTexture ||
		nullptr == m_pBuffer)
		return;

	SetUp_RenderState();
	
	m_pTexture->SetUp_OnGraphicDev((_uint)m_fFrame);
		
	m_pBuffer->Render_Buffer(m_matTransform);
	
	Reset_RenderState();
}

HRESULT CMouse::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture_Mouse
	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Mouse");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	return NOERROR;
}

HRESULT CMouse::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CMouse::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

HRESULT CMouse::Animation(_float fTimeDelta)
{
	m_fFrame += 4.0f * fTimeDelta;

	if (m_fFrame >= 4.0f)
		m_fFrame = 0.f;

	return NOERROR;
}

CGameObject * CMouse::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMouse*		pInstance = new CMouse(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CMouse Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMouse::Free(void)
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
