#include "stdafx.h"
#include "..\Headers\Drop_Object.h"
#include "Camera.h"
#include "Management.h"
#include "Sound_Manager.h"
CDrop_Object::CDrop_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

HRESULT CDrop_Object::Ready_GameObject(_vec3 vPosition, _vec3 vSize, _float idx)
{
	CGameObject::Ready_GameObject();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	m_fFrame = idx;
	m_vSize = vSize;
	if (m_fFrame == 0 || m_fFrame == 1)
		m_vSize = _vec3(64.f, 64.f, 1.f);
	else
		m_vSize = vSize;
	m_vPosition = vPosition;
	m_fSpeedPerSec = 400;

	Set_Scale(m_vSize);
	Set_Position(m_vPosition);

	if (0 == m_fFrame)
		m_State = BRANCH;
	else if (1 == m_fFrame)
		m_State = STONE;
	else if (2 == m_fFrame)
		m_State = WEED;
	else if (3 == m_fFrame)
		m_State = COPPER;
	else if (4 == m_fFrame)
		m_State = SLIME;

	CGameObject* pPlayer = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();

	m_fJump = 7.f;
	int RanNumb = rand() % 6;
	if (0 == RanNumb)
		m_fJumpX = 0.5f;
	else if (1 == RanNumb)
		m_fJumpX = -0.5f;
	else if (2 == RanNumb)
		m_fJumpX = 1.f;
	else if (3 == RanNumb)
		m_fJumpX = -1.f;
	else if (4 == RanNumb)
		m_fJumpX = 1.5f;
	else if (5 == RanNumb)
		m_fJumpX = -1.5f;

	return NOERROR;
}

_int CDrop_Object::Update_GameObject(const _float& fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene() || SCENE_CAVE == CManagement::GetInstance()->Get_Scene())
	{
		if (false == m_bObjectOff)
		{
			if (m_bOff == false)
			{
				m_matTransform._41 += m_fJumpX;
				m_matTransform._42 -= m_fJump;
			}

			if (m_vPosition.y <= m_matTransform._42)
			{
				m_fTime = 0.f;
				m_fJump -= 1.6f;
				if (m_fJump <= 0.f)
					m_bOff = true;
			}

			if (m_bOff == false)
			{
				m_fTime += 0.06f;
				m_matTransform._42 += (9.8f * (m_fTime*m_fTime)) * 0.6f;
			}

			if (true == m_bOff)
			{
				_vec3		vTargetPos = _vec3(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 0.f);

				Move_ToTarget(vTargetPos, fTimeDelta);

				RECT rcTmp = Get_Rect(m_pPlayer->Get_X(), m_pPlayer->Get_Y() + 57.f, 50.f, 32.f);

				RECT rcCollision = { 0 };

				RECT rcObject = Get_Rect(m_matTransform._41, m_matTransform._42, 48.f, 48.f);

				if (true == _bool(IntersectRect(&rcCollision, &rcObject, &rcTmp)))
				{
					m_bObjectOff = true;
					CGameObject* pInven = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Inventory");
					if (nullptr == pInven)
						return E_FAIL;
					CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI",1);
					if (nullptr == pTarget)
						return E_FAIL;

					if (0 == m_fFrame)
					{
						CSound_Manager::Get_Instance()->SoundPlay(13, FALSE);
						pInven->Add_Item(L"Branch", 1, 14);
						pTarget->Set_Object(0);
					}
					else if (1 == m_fFrame)
					{
						CSound_Manager::Get_Instance()->SoundPlay(13, FALSE);
						pInven->Add_Item(L"Stone", 1, 15);
						pTarget->Set_Object(1);
					}
					else if (2 == m_fFrame)
					{
						CSound_Manager::Get_Instance()->SoundPlay(13, FALSE);
						pInven->Add_Item(L"Weed", 1, 16);
						pTarget->Set_Object(2);
					}
					else if (3 == m_fFrame)
					{
						CSound_Manager::Get_Instance()->SoundPlay(13, FALSE);
						pInven->Add_Item(L"Copper", 1, 19);
						pTarget->Set_Object(6);
					}
					else if (4 == m_fFrame)
					{
						CSound_Manager::Get_Instance()->SoundPlay(13, FALSE);
						pInven->Add_Item(L"Slime", 1, 20);
						pTarget->Set_Object(7);
					}
				}
			}
		}
	}
	return _int();
}

_int CDrop_Object::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (SCENE_FARM == CManagement::GetInstance()->Get_Scene() || SCENE_CAVE == CManagement::GetInstance()->Get_Scene())
	{
		if (false == m_bObjectOff)
		{
			if (nullptr != m_pRender_Manager)
				m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_YSORT, this);
		}
	}
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CDrop_Object::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture ||
		nullptr == m_pShader)
		return;

	CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	m_pTexture[m_State]->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
	
	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(0);

	TempMatrix = Render_TransForm(pCamera);

	m_pBuffer->Render_Buffer(TempMatrix);

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CDrop_Object::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture
	m_pTexture[BRANCH] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Branch");
	if (nullptr == m_pTexture[BRANCH])
		return E_FAIL;
	m_pTexture[BRANCH]->AddRef();

	m_pTexture[STONE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Stone");
	if (nullptr == m_pTexture[STONE])
		return E_FAIL;
	m_pTexture[STONE]->AddRef();

	m_pTexture[WEED] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Weed");
	if (nullptr == m_pTexture[WEED])
		return E_FAIL;
	m_pTexture[WEED]->AddRef();

	m_pTexture[COPPER] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Copper");
	if (nullptr == m_pTexture[COPPER])
		return E_FAIL;
	m_pTexture[COPPER]->AddRef();

	m_pTexture[SLIME] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Slime");
	if (nullptr == m_pTexture[SLIME])
		return E_FAIL;
	m_pTexture[SLIME]->AddRef();

	// For.Shader
	m_pShader = (CShader*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Shader_Default");
	if (nullptr == m_pShader)
		return E_FAIL;
	m_pShader->AddRef();

	return NOERROR;
}

HRESULT CDrop_Object::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	// 셰이더 전역변수에 값을 전달한다.
	m_pTexture[m_State]->SetUp_OnShader(pEffect, "g_BaseTexture",m_fFrame);

	Safe_Release(pEffect);

	return NOERROR;
}

_bool CDrop_Object::isDraw(CGameObject * pCamera)
{
	RECT rcTmp = pCamera->Get_Rect();

	return isIn_Rect(rcTmp);
}

_matrix CDrop_Object::Render_TransForm(CGameObject * pCamera)
{
	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_matTransform._41, m_matTransform._42);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	return TempMatrix;
}

CDrop_Object * CDrop_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3 vPosition, _vec3 vSize, _float idx)
{
	CDrop_Object*		pInstance = new CDrop_Object(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject(vPosition, vSize, idx)))
	{
		_MSG_BOX("CDrop_Object Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDrop_Object::Free()
{
	Safe_Release(m_pPlayer);
	for (auto& pTexture : m_pTexture)
		Safe_Release(pTexture);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	
	CGameObject::Free();
}
