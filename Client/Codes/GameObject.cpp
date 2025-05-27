#include "stdafx.h"
#include "..\Headers\GameObject.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pRender_Manager(CRender_Manager::GetInstance())
	, m_pResource_Manager(CResource_Manager::GetInstance())	
{
	m_pGraphic_Device->AddRef();
	m_pResource_Manager->AddRef();
	m_pRender_Manager->AddRef();
}

void CGameObject::Set_Position(_vec3 vPoint)
{
	memcpy(&m_matTransform.m[3][0], &vPoint, sizeof(_vec3));
}

void CGameObject::Set_Scale(_vec3 vScale)
{
	_vec3		vRight;	
	D3DXVec3Normalize(&vRight, (_vec3*)&m_matTransform.m[STATE_RIGHT][0]);
	vRight *= vScale.x;
	memcpy(&m_matTransform.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));

	_vec3		vUp;
	D3DXVec3Normalize(&vUp, (_vec3*)&m_matTransform.m[STATE_UP][0]);
	vUp *= vScale.y;
	memcpy(&m_matTransform.m[STATE_UP][0], &vUp, sizeof(_vec3));

	_vec3		vLook;
	D3DXVec3Normalize(&vLook, (_vec3*)&m_matTransform.m[STATE_LOOK][0]);
	vLook *= vScale.z;
	memcpy(&m_matTransform.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
}

void CGameObject::Set_Right2D(_vec3 vDir)
{
	_vec3		vRight = *D3DXVec3Normalize(&vDir, &vDir) 
		* D3DXVec3Length((_vec3*)&m_matTransform.m[STATE_RIGHT][0]);

	_vec3		vLook;
	memcpy(&vLook, &m_matTransform.m[STATE_LOOK][0], sizeof(_vec3));

	_vec3		vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	vUp = *D3DXVec3Normalize(&vUp, &vUp)
		* D3DXVec3Length((_vec3*)&m_matTransform.m[STATE_UP][0]);

	memcpy(&m_matTransform.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
	memcpy(&m_matTransform.m[STATE_UP][0], &vUp, sizeof(_vec3));
}

void CGameObject::Set_LookAt2D(_vec3 vPoint)
{
	_vec3		vRight = vPoint - *(_vec3*)&m_matTransform.m[STATE_POSITION][0];

	Set_Right2D(vRight);
}
void CGameObject::Set_RotationZ(const _float & fRadian) // 항등상태에서에서 더 몇도만큼 회전한다.
{
	_matrix		matRot;

	D3DXMatrixRotationZ(&matRot, fRadian);

	_vec3		vRight, vUp, vLook;
	vRight = _vec3(1.f, 0.f, 0.f) * Get_Scale(STATE_RIGHT);
	vUp = _vec3(0.f, 1.f, 0.f) * Get_Scale(STATE_UP);
	vLook = _vec3(0.f, 0.f, 1.f) * Get_Scale(STATE_LOOK);

	// 1x3벡터를 1x4로 바꿔서 행렬과 곱을 수행한다.
	//D3DXVec3TransformNormal(); // w값을 0으로 채운다.	
	//D3DXVec3TransformCoord(); // w값을 1으로 채운다.

	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	memcpy(&m_matTransform.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
	memcpy(&m_matTransform.m[STATE_UP][0], &vUp, sizeof(_vec3));
	memcpy(&m_matTransform.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
}

void CGameObject::Set_RotationY(const _float & fRadian) // 항등상태에서에서 더 몇도만큼 회전한다.
{
	_matrix		matRot;

	D3DXMatrixRotationY(&matRot, fRadian);

	_vec3		vRight, vUp, vLook;
	vRight = _vec3(1.f, 0.f, 0.f) * Get_Scale(STATE_RIGHT);
	vUp = _vec3(0.f, 1.f, 0.f) * Get_Scale(STATE_UP);
	vLook = _vec3(0.f, 0.f, 1.f) * Get_Scale(STATE_LOOK);

	// 1x3벡터를 1x4로 바꿔서 행렬과 곱을 수행한다.
	//D3DXVec3TransformNormal(); // w값을 0으로 채운다.	
	//D3DXVec3TransformCoord(); // w값을 1으로 채운다.

	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	memcpy(&m_matTransform.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
	memcpy(&m_matTransform.m[STATE_UP][0], &vUp, sizeof(_vec3));
	memcpy(&m_matTransform.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
}

void CGameObject::RotationZ(const _float & fRadian) // 현재 각도에서 추가적으로 더 몇도만큼 회전한다.
{
	_matrix		matRot;

	D3DXMatrixRotationZ(&matRot, fRadian);

	_vec3		vRight, vUp, vLook;
	memcpy(&vRight, &m_matTransform.m[STATE_RIGHT][0], sizeof(_vec3));
	memcpy(&vUp, &m_matTransform.m[STATE_UP][0], sizeof(_vec3));
	memcpy(&vLook, &m_matTransform.m[STATE_LOOK][0], sizeof(_vec3));

	// 1x3벡터를 1x4로 바꿔서 행렬과 곱을 수행한다.
	//D3DXVec3TransformNormal(); // w값을 0으로 채운다.	
	//D3DXVec3TransformCoord(); // w값을 1으로 채운다.

	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	memcpy(&m_matTransform.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
	memcpy(&m_matTransform.m[STATE_UP][0], &vUp, sizeof(_vec3));
	memcpy(&m_matTransform.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
}

RECT CGameObject::Get_Rect() const
{
	RECT rcTmp;

	SetRect(&rcTmp, m_fX - m_fSizeX*0.5f, m_fY - m_fSizeY*0.5f
		, m_fX + m_fSizeX*0.5f, m_fY + m_fSizeY*0.5f);


	return rcTmp;
}

RECT CGameObject::Get_Rect(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	RECT rcTmp;

	SetRect(&rcTmp, fX - fSizeX*0.5f, fY - fSizeY*0.5f
		, fX + fSizeX*0.5f, fY + fSizeY*0.5f);

	return rcTmp;
}

_bool CGameObject::isIn_Rect(const RECT & rcRect)
{
	RECT	rcColl = { m_matTransform._41 - m_matTransform._11*0.5f, m_matTransform._42 - m_matTransform._22*0.5f
		, m_matTransform._41 + m_matTransform._11*0.5f, m_matTransform._42 + m_matTransform._22*0.5f };

	RECT	rcTmp;

	return IntersectRect(&rcTmp, &rcRect, &rcColl);
}



void CGameObject::Move_ToTarget(_vec3 vTargetPos, const _float & fTimeDelta)
{
	_vec3			vDir;
	_vec3			vPosition = *Get_Infomation(STATE_POSITION);

	vDir = vTargetPos - vPosition;

	_float			fDistance = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);

	if (fDistance <= 100.0f)
		vPosition += vDir * m_fSpeedPerSec * fTimeDelta;

	memcpy(&m_matTransform.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));
}

void CGameObject::Faraway_ToTarget(_vec3 vTargetPos, const _float & fTimeDelta)
{
	_vec3			vDir;
	_vec3			vPosition = *Get_Infomation(STATE_POSITION);

	vDir = vTargetPos - vPosition;

	_float			fDistance = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);

	vPosition -= vDir * 2000 * fTimeDelta;

	memcpy(&m_matTransform.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));
}

HRESULT CGameObject::Ready_GameObject()
{
	
	// 항등행렬을 만들어준다.
	D3DXMatrixIdentity(&m_matTransform);
	

	return NOERROR;
}

_int CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

_int CGameObject::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CGameObject::Render_GameObject()
{
}

_int CGameObject::Get_State()
{
	return _int();
}

HRESULT CGameObject::Ready_Camera()
{
	return E_NOTIMPL;
}

HRESULT CGameObject::Ready_Location(_int iNumb)
{
	return E_NOTIMPL;
}

ITEMINFO * CGameObject::Get_Item()
{
	return nullptr;
}

ITEMINFO CGameObject::Get_ItemOn()
{
	return ITEMINFO();
}

_bool CGameObject::Get_ToolOn()
{
	return _bool();
}

_bool CGameObject::Get_DirtCheck()
{
	return _bool();
}

_bool CGameObject::Get_HarvestCheck()
{
	return _bool();
}

_bool CGameObject::Get_HarvestOn()
{
	return _bool();
}

_int CGameObject::Get_Crop()
{
	return _int();
}

void CGameObject::Add_Item(wchar_t const* Name, _int Count, _int ItemNum)
{
}

void CGameObject::Use_Item(wchar_t const * Name, _int Count)
{
}

_bool CGameObject::Get_FishingOn()
{
	return _bool();
}

_float CGameObject::Get_Frame()
{
	return _float();
}

void CGameObject::Set_FishingOff()
{
}

_bool CGameObject::Get_GetFish()
{
	return _bool();
}

void CGameObject::Set_FrontIdle()
{
}

_int CGameObject::Get_FishNumber()
{
	return _int();
}

_int CGameObject::Get_PlayerLook()
{
	return _int();
}

_float CGameObject::Get_TimeDegree()
{
	return _float();
}

_bool CGameObject::Get_NextDay()
{
	return _bool();
}

void CGameObject::Set_NextDay()
{
}

_bool CGameObject::Create_DropObject()
{
	return _bool();
}

_float CGameObject::Get_fX()
{
	return _float();
}

_float CGameObject::Get_fY()
{
	return _float();
}

void CGameObject::Set_DropObject()
{
}

void CGameObject::Set_Object(_int iFrame)
{
}

void CGameObject::Set_Start()
{
}

_bool CGameObject::Get_Start()
{
	return _bool();
}

void CGameObject::Set_StopOn()
{
}

void CGameObject::Set_StopOff()
{
}

_bool CGameObject::Get_ShopOn()
{
	return _bool();
}

_int CGameObject::Get_Gold()
{
	return _int();
}

void CGameObject::Set_Gold(_int Gold)
{
}

void CGameObject::Buy_Item(wchar_t const * Name, _int Count, _int ItemNum, _int Idx)
{
}

_bool CGameObject::Get_RainDay()
{
	return _bool();
}

void CGameObject::Set_Collision(_bool bColl)
{
}

void CGameObject::Set_Blink(_bool bBlink)
{
}

_bool CGameObject::Get_Hurt()
{
	return _bool();
}

void CGameObject::Set_Hurt()
{
}

_bool CGameObject::Get_InvenOn()
{
	return _bool();
}

_bool CGameObject::Get_Install()
{
	return _bool();
}

void CGameObject::Set_Install()
{
}

_int CGameObject::Get_DirtX()
{
	return _int();
}

_int CGameObject::Get_DirtY()
{
	return _int();
}

_bool CGameObject::Get_ChestOn()
{
	return _bool();
}

void CGameObject::Set_ChestOn()
{
}

void CGameObject::Set_UI(_bool UI)
{
}

HRESULT CGameObject::Update_Collider(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	SetRect(&m_rcCollider, fX - fSizeX * 0.5f, fY - fSizeY * 0.5f,
		fX + fSizeX * 0.5f, fY + fSizeY * 0.5f);

	return NOERROR;
}

void CGameObject::Free()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pResource_Manager);
	Safe_Release(m_pRender_Manager);
}