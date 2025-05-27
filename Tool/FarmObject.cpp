#include "stdafx.h"
#include "FarmObject.h"
#include "Resource_Manager.h"

#include "MainFrm.h"

CFarmObject::CFarmObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

OBJECTINFO CFarmObject::Get_ObjectInfo()
{
	OBJECTINFO			ObjectInfo;

	ObjectInfo.vPosition = m_vPosition;
	ObjectInfo.vSize = m_vScale;
	ObjectInfo.iIndex = m_iTextureIdx;

	return ObjectInfo;
}

void CFarmObject::Set_RenderPos(_int iScollX, _int iScollY)
{
	m_vRenderPos = m_vPosition;

	m_vRenderPos.x -= iScollX;
	m_vRenderPos.y -= iScollY;
}

HRESULT CFarmObject::Ready_Object(const _vec3* pPosition)
{
	if (FAILED(Ready_Resource()))
		return E_FAIL;

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();
	_int iIndex = pMainFrm->Get_ObjectIndex();
	m_iTextureIdx = iIndex;

	m_vPosition = *pPosition;
	//Farm
	/*if (0 == iIndex)
		m_vScale = (_vec3(128.f, 128.f, 1.0f));
	else if (1 == iIndex || 2 == iIndex)
		m_vScale = (_vec3(64.f, 64.f, 1.0f));
	else if (3 == iIndex)
		m_vScale = (_vec3(128.f, 128.f, 1.0f));
	else if (4 == iIndex)
		m_vScale = (_vec3(128.f, 128.f, 1.0f));
	else if (5 == iIndex)
		m_vScale = (_vec3(256.f, 256.f, 1.0f));
	else if (6 == iIndex || 7 == iIndex)
		m_vScale = (_vec3(64.f, 64.f, 1.0f));
	else if (8 == iIndex || 9 == iIndex || 10 == iIndex)
		m_vScale = (_vec3(256.f, 512.f, 1.0f));
	else if (11 == iIndex || 12 == iIndex || 13 == iIndex)
		m_vScale = (_vec3(256.f, 512.f, 1.0f));
	else if (14 == iIndex || 15 == iIndex || 16 == iIndex || 17 == iIndex)
		m_vScale = (_vec3(64.f, 64.f, 1.0f));*/

	//Shop
	/*if (0 == iIndex )
		m_vScale = (_vec3(64.f * 4.f, 64.f* 4.f, 1.0f));
	else if (1 == iIndex || 2 == iIndex || 8 == iIndex)
		m_vScale = (_vec3(16.f* 4.f, 32.f* 4.f, 1.0f));
	else if (3 == iIndex )
		m_vScale = (_vec3(16.f* 4.f, 64.f* 4.f, 1.0f));
	else if (4 == iIndex)
		m_vScale = (_vec3(16.f* 4.f, 128.f* 4.f, 1.0f));
	else if (5 == iIndex)
		m_vScale = (_vec3(32.f* 4.f, 64.f* 4.f, 1.0f));
	else if (6 == iIndex || 7 ==iIndex)
		m_vScale = (_vec3(128.f* 4.f, 64.f* 4.f, 1.0f));
	else if (9 == iIndex || 10 == iIndex || 12 == iIndex || 13 == iIndex || 14 == iIndex)
		m_vScale = (_vec3(16.f* 4.f, 16.f* 4.f, 1.0f));
	else if( 11== iIndex)
		m_vScale = (_vec3(32.f * 4.f, 32.f* 4.f, 1.0f));
	*/

	//Town
	if(0==iIndex || 1 == iIndex || 5 == iIndex || 21 == iIndex)
		m_vScale = (_vec3(32.f * 4.f, 32.f* 4.f, 1.0f));
	else if(2 == iIndex || 7 == iIndex || 11 == iIndex)
		m_vScale = (_vec3(64.f * 4.f, 64.f* 4.f, 1.0f));
	else if (6 == iIndex || 8 == iIndex)
		m_vScale = (_vec3(16.f * 4.f, 64.f* 4.f, 1.0f));
	else if (9 == iIndex || 12 == iIndex || 13 == iIndex || 16 == iIndex || 17 == iIndex || 18 == iIndex ||
			24 == iIndex || 25 == iIndex || 26 == iIndex || 27 == iIndex || 28 == iIndex || 29 == iIndex)
		m_vScale = (_vec3(16.f * 4.f, 32.f* 4.f, 1.0f));
	else if (10 == iIndex || 14 == iIndex || 15 == iIndex || 30 == iIndex || 31 == iIndex ||
			32 == iIndex || 33 == iIndex || 34 == iIndex)
		m_vScale = (_vec3(16.f * 4.f, 16.f* 4.f, 1.0f));
	else if (19 == iIndex)
		m_vScale = (_vec3(32.f * 4.f, 16.f* 4.f, 1.0f));
	else if (20 == iIndex || 22 ==iIndex)
		m_vScale = (_vec3(256.f * 4.f, 128.f* 4.f, 1.0f));
	else if (23 == iIndex || 35 == iIndex || 36 == iIndex || 37 == iIndex || 38 == iIndex)
		m_vScale = (_vec3(64.f * 4.f, 128.f* 4.f, 1.0f));
	else if (39 == iIndex || 40 == iIndex || 43 == iIndex)
		m_vScale = (_vec3(256.f * 4.f, 256.f* 4.f, 1.0f));
	else if (41 == iIndex || 42 == iIndex)
		m_vScale = (_vec3(128.f * 4.f, 256.f* 4.f, 1.0f));
	else if(3 == iIndex || 4 == iIndex)
		m_vScale = (_vec3(16.f * 4.f, 128.f* 4.f, 1.0f));

	D3DXMatrixIdentity(&m_matTransform);

	m_vRenderPos = m_vPosition;
	
	m_vPoint[0] = _vec3(m_vPosition.x - m_vScale.x * 0.5f, m_vPosition.y - m_vScale.y * 0.5f, 0.f);
	m_vPoint[1] = _vec3(m_vPosition.x + m_vScale.x * 0.5f, m_vPosition.y - m_vScale.y * 0.5f, 0.f);
	m_vPoint[2] = _vec3(m_vPosition.x + m_vScale.x * 0.5f, m_vPosition.y + m_vScale.y * 0.5f, 0.f);
	m_vPoint[3] = _vec3(m_vPosition.x - m_vScale.x * 0.5f, m_vPosition.y + m_vScale.y *0.5f, 0.f);

	m_vDir[0] = m_vPoint[1] - m_vPoint[0];
	m_vDir[1] = m_vPoint[2] - m_vPoint[1];
	m_vDir[2] = m_vPoint[3] - m_vPoint[2];
	m_vDir[3] = m_vPoint[0] - m_vPoint[3];

	m_vNormal[0] = _vec3(m_vDir[0].y * -1.f, m_vDir[0].x, 0.f);
	m_vNormal[1] = _vec3(m_vDir[1].y * -1.f, m_vDir[1].x, 0.f);
	m_vNormal[2] = _vec3(m_vDir[2].y * -1.f, m_vDir[2].x, 0.f);
	m_vNormal[3] = _vec3(m_vDir[3].y * -1.f, m_vDir[3].x, 0.f);

	for (size_t i = 0; i < 4; ++i)
		D3DXVec3Normalize(&m_vNormal[i], &m_vNormal[i]);

	return NOERROR;
}

HRESULT CFarmObject::Ready_Object(const OBJECTINFO * pObjectInfo)
{
	if (FAILED(Ready_Resource()))
		return E_FAIL;

	m_vPosition = pObjectInfo->vPosition;
	m_vScale = pObjectInfo->vSize;

	m_iTextureIdx = pObjectInfo->iIndex;

	D3DXMatrixIdentity(&m_matTransform);

	m_vRenderPos = m_vPosition;

	m_vPoint[0] = _vec3(m_vPosition.x - m_vScale.x * 0.5f, m_vPosition.y - m_vScale.y * 0.5f, 0.f);
	m_vPoint[1] = _vec3(m_vPosition.x + m_vScale.x * 0.5f, m_vPosition.y - m_vScale.y * 0.5f, 0.f);
	m_vPoint[2] = _vec3(m_vPosition.x + m_vScale.x * 0.5f, m_vPosition.y + m_vScale.y * 0.5f, 0.f);
	m_vPoint[3] = _vec3(m_vPosition.x - m_vScale.x * 0.5f, m_vPosition.y + m_vScale.y *0.5f, 0.f);


	m_vDir[0] = m_vPoint[1] - m_vPoint[0];
	m_vDir[1] = m_vPoint[2] - m_vPoint[1];
	m_vDir[2] = m_vPoint[3] - m_vPoint[2];
	m_vDir[3] = m_vPoint[0] - m_vPoint[3];

	m_vNormal[0] = _vec3(m_vDir[0].y * -1.f, m_vDir[0].x, 0.f);
	m_vNormal[1] = _vec3(m_vDir[1].y * -1.f, m_vDir[1].x, 0.f);
	m_vNormal[2] = _vec3(m_vDir[2].y * -1.f, m_vDir[2].x, 0.f);
	m_vNormal[3] = _vec3(m_vDir[3].y * -1.f, m_vDir[3].x, 0.f);

	for (size_t i = 0; i < 4; ++i)
		D3DXVec3Normalize(&m_vNormal[i], &m_vNormal[i]);


	return NOERROR;
}

void CFarmObject::Update_Object()
{
	_matrix			matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vRenderPos.x, m_vRenderPos.y, m_vRenderPos.z);

	m_matTransform = matScale * matTrans;	
}

void CFarmObject::Render_Object()
{
	if (nullptr == m_pTexture || 
		nullptr == m_pBuffer)
		return;

	if (FAILED(SetUp_RenderState()))
		return;

	m_pTexture->SetUp_OnGraphicDev(m_iTextureIdx);
	m_pBuffer->Render_Buffer(m_matTransform);

	if (FAILED(ReSet_RenderState()))
		return;
}

_bool CFarmObject::Clicked_Mouse(_vec3 vMousePos)
{
	// 네개 변에 대해서 모두 오른쪽에 있는지를 체크하자.
	// 네개 변에 대해서 한번이라돠 왼쪽에 있으면 false 

	m_vPosition.x = vMousePos.x;
	m_vPosition.y = vMousePos.y;

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	_int iIndex = pMainFrm->Get_ObjectIndex();

	if (iIndex & 0x80000000)
		return false;

	m_iTextureIdx = iIndex;

	return _bool(true);
}

HRESULT CFarmObject::Ready_Resource()
{
	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)CResource_Manager::GetInstance()->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture = (CTexture*)CResource_Manager::GetInstance()->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Object");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	return NOERROR;
}

HRESULT CFarmObject::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CFarmObject::ReSet_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return NOERROR;
}

CFarmObject * CFarmObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pPosition)
{
	CFarmObject*	pInstance = new CFarmObject(pGraphic_Device);

	if (FAILED(pInstance->Ready_Object(pPosition)))
	{
		_MSG_BOX("CFarmObject Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;	
}

CFarmObject * CFarmObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const OBJECTINFO* pObjectInfo)
{
	CFarmObject*	pInstance = new CFarmObject(pGraphic_Device);

	if (FAILED(pInstance->Ready_Object(pObjectInfo)))
	{
		_MSG_BOX("CFarmObject Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFarmObject::Free()
{
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pGraphic_Device);
}
