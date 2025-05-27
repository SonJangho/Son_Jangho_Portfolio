#include "stdafx.h"
#include "..\Headers\Player.h"
#include "FocusCam.h"
#include "Shadow.h"
#include "Sound_Manager.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CPlayer::Ready_GameObject()
{
	CGameObject::Ready_GameObject();	

	if (FAILED(SetUp_StateDesc()))
		return E_FAIL;	

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	m_eCurrentState = STATE_FRONT_IDLE;

	Ready_Location(BED);

	m_fSpeed = 6.f;

	Set_Scale(_vec3(128.f, 128.f, 1.f));
	Set_Position(_vec3(m_fX, m_fY, 0.f));

	Ready_Camera();

	CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_BasicUI");
	if (nullptr == pTarget)
		return E_FAIL;

	m_pUI = pTarget;
	m_pUI->AddRef();

	return NOERROR;
}

HRESULT CPlayer::Ready_Camera()
{
	CGameObject* pCamera = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	((CFocusCam*)pCamera)->SetUp_Focus(this);

	m_pCamera = pCamera;

	return NOERROR;
}

HRESULT CPlayer::Ready_Location(_int iNumb)
{
	if (BED == iNumb)
	{
		m_fX = 19.f*64.f;
		m_fY = 15.3f*64.f;
	}
	else if (HOMEIN == iNumb)
	{
		m_fX = 12.5f * 64.f;
		m_fY = 18.5f * 64.f;
	}
	else if (HOMEOUT == iNumb)
	{
		m_fX = 32.5f*64.f;
		m_fY = 5.5f*64.f;
	}
	else if (TOWNIN == iNumb)
	{
		m_fX = 1.f*64.f;
		m_fY = 9.f*64.f;
	}
	else if (FARMIN == iNumb)
	{
		m_fX = 39.f*64.f;
		m_fY = 9.f*64.f;
	}
	else if (SHOPIN == iNumb)
	{
		m_fX = 6.5f*64.f;
		m_fY = 17.f*64.f;
	}
	else if (SHOPOUT == iNumb)
	{
		m_fX = 35.5f*64.f;
		m_fY = 11.f*64.f;
	}
	else if (ROADIN == iNumb)
	{
		m_fX = 8.f*64.f;
		m_fY = 29.f*64.f;
	}
	else if (ROADOUT == iNumb)
	{
		m_fX = 18.5f*64.f;
		m_fY = 1.f*64.f;
	}
	else if (CAVEIN == iNumb)
	{
		m_fX = 6.5f*64.f;
		m_fY = 6.5f*64.f;
	}
	else if (CAVEOUT == iNumb)
	{
		m_fX = 16.5f*64.f;
		m_fY = 5.5f*64.f;
	}

	return NOERROR;
}

_int CPlayer::Update_GameObject(const _float & fTimeDelta)
{
	if (false == m_bStop)
	{
		Animation(fTimeDelta);

		if ((GetKeyState(VK_LBUTTON) & 0x8000) && false == m_isClick)
		{
			m_isClick = true;
			POINT			ptMouse;

			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);

			if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Hoe")) || (0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Ax")) ||
				(0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Pick")))
			{
				m_fFrame = 0;

				if (m_eCurrentState == STATE_SIDE_IDLE || m_eCurrentState == STATE_SIDE_RUN)
					m_eCurrentState = STATE_SIDE_SWING;
				else if (m_eCurrentState == STATE_BACK_IDLE || m_eCurrentState == STATE_BACK_RUN)
					m_eCurrentState = STATE_BACK_SWING;
				else if (m_eCurrentState == STATE_FRONT_IDLE || m_eCurrentState == STATE_FRONT_RUN)
					m_eCurrentState = STATE_FRONT_SWING;
			}
			else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Sickle")) || (0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Sword")))
			{
				m_fFrame = 0;
				if (m_eCurrentState == STATE_SIDE_IDLE || m_eCurrentState == STATE_SIDE_RUN)
					m_eCurrentState = STATE_SIDE_SLASH;
				else if (m_eCurrentState == STATE_BACK_IDLE || m_eCurrentState == STATE_BACK_RUN)
					m_eCurrentState = STATE_BACK_SLASH;
				else if (m_eCurrentState == STATE_FRONT_IDLE || m_eCurrentState == STATE_FRONT_RUN)
					m_eCurrentState = STATE_FRONT_SLASH;
			}
			else if (0 == wcscmp(m_pUI->Get_ItemOn().szName, L"WateringPot")) // 물뿌리개
			{
				m_fFrame = 0;
				if (m_eCurrentState == STATE_SIDE_IDLE || m_eCurrentState == STATE_SIDE_RUN)
					m_eCurrentState = STATE_SIDE_WATERING;
				else if (m_eCurrentState == STATE_BACK_IDLE || m_eCurrentState == STATE_BACK_RUN)
					m_eCurrentState = STATE_BACK_WATERING;
				else if (m_eCurrentState == STATE_FRONT_IDLE || m_eCurrentState == STATE_FRONT_RUN)
					m_eCurrentState = STATE_FRONT_WATERING;
			}
			else if (0 == wcscmp(m_pUI->Get_ItemOn().szName, L"FishingRod")) // 낚시대
			{
				m_fFrame = 0;
				if (m_eCurrentState == STATE_SIDE_IDLE || m_eCurrentState == STATE_SIDE_RUN)
					m_eCurrentState = STATE_SIDE_FISHSWING;
				else if (m_eCurrentState == STATE_BACK_IDLE || m_eCurrentState == STATE_BACK_RUN)
					m_eCurrentState = STATE_BACK_FISHSWING;
				else if (m_eCurrentState == STATE_FRONT_IDLE || m_eCurrentState == STATE_FRONT_RUN)
					m_eCurrentState = STATE_FRONT_FISHSWING;
			}
		}
		else if (m_eCurrentState != STATE_SIDE_SWING && m_eCurrentState != STATE_BACK_SWING && m_eCurrentState != STATE_FRONT_SWING &&
			m_eCurrentState != STATE_SIDE_SLASH && m_eCurrentState != STATE_BACK_SLASH &&m_eCurrentState != STATE_FRONT_SLASH &&
			m_eCurrentState != STATE_SIDE_WATERING && m_eCurrentState != STATE_BACK_WATERING &&m_eCurrentState != STATE_FRONT_WATERING &&
			m_eCurrentState != STATE_BACK_HARVEST && m_eCurrentState != STATE_SIDE_HARVEST && m_eCurrentState != STATE_FRONT_HARVEST&&
			m_eCurrentState != STATE_SIDE_FISHSWING && m_eCurrentState != STATE_BACK_FISHSWING && m_eCurrentState != STATE_FRONT_FISHSWING &&
			m_eCurrentState != STATE_SIDE_FISHING && m_eCurrentState != STATE_BACK_FISHING && m_eCurrentState != STATE_FRONT_FISHING &&
			m_eCurrentState != STATE_GET_FISH)
		{
			if (true == (m_pUI->Get_ItemOn().isHandle))
			{
				if (STATE_BACK_IDLE == m_eCurrentState)
					m_eCurrentState = STATE_BACK_HIDLE;
				else if (STATE_FRONT_IDLE == m_eCurrentState)
					m_eCurrentState = STATE_FRONT_HIDLE;
				else if (STATE_SIDE_IDLE == m_eCurrentState)
					m_eCurrentState = STATE_SIDE_HIDLE;

				if (GetKeyState('W') & 0x8000)
				{
					m_isRight = true;
					m_eCurrentState = STATE_BACK_HRUN;

					if (GetKeyState('D') & 0x8000)
					{
						m_isRight = true;
						m_eCurrentState = STATE_SIDE_HRUN;
						m_fX += (atan2(m_fSpeed, m_fSpeed))*m_fSpeed;
						m_fY -= (atan2(m_fSpeed, m_fSpeed))*m_fSpeed;
					}
					else if (GetKeyState('A') & 0x8000)
					{
						m_isRight = false;
						m_eCurrentState = STATE_SIDE_HRUN;
						m_fX -= (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
						m_fY -= (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
					}
					else
						m_fY -= m_fSpeed;
				}
				else if (GetKeyState('S') & 0x8000)
				{
					m_isRight = true;
					m_eCurrentState = STATE_FRONT_HRUN;

					if (GetKeyState('D') & 0x8000)
					{
						m_isRight = true;
						m_eCurrentState = STATE_SIDE_HRUN;
						m_fX += (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
						m_fY += (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
					}
					else if (GetKeyState('A') & 0x8000)
					{
						m_isRight = false;
						m_eCurrentState = STATE_SIDE_HRUN;
						m_fX -= (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
						m_fY += (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
					}
					else
						m_fY += m_fSpeed;
				}

				if (GetKeyState('A') & 0x8000 && !(GetKeyState('W') & 0x8000) && !(GetKeyState('S') & 0x8000))
				{
					m_isRight = false;
					m_eCurrentState = STATE_SIDE_HRUN;
					m_fX -= m_fSpeed;
				}

				if (GetKeyState('D') & 0x8000 && !(GetKeyState('W') & 0x8000) && !(GetKeyState('S') & 0x8000))
				{
					m_isRight = true;
					m_eCurrentState = STATE_SIDE_HRUN;
					m_fX += m_fSpeed;
				}

				if (!(GetKeyState('W') & 0x8000) && !(GetKeyState('S') & 0x8000) && !(GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
				{
					if (STATE_FRONT_SWING != m_eCurrentState && STATE_BACK_SWING != m_eCurrentState && STATE_SIDE_SWING != m_eCurrentState &&
						STATE_FRONT_SLASH != m_eCurrentState && STATE_BACK_SLASH != m_eCurrentState &&STATE_SIDE_SLASH != m_eCurrentState)
						m_fFrame = 0;

					if (STATE_BACK_HRUN == m_eCurrentState)
						m_eCurrentState = STATE_BACK_HIDLE;
					else if (STATE_FRONT_HRUN == m_eCurrentState)
						m_eCurrentState = STATE_FRONT_HIDLE;
					else if (STATE_SIDE_HRUN == m_eCurrentState)
						m_eCurrentState = STATE_SIDE_HIDLE;
				}
			}
			else
			{ 
				if (STATE_BACK_HIDLE == m_eCurrentState)
					m_eCurrentState = STATE_BACK_IDLE;
				else if (STATE_FRONT_HIDLE == m_eCurrentState)
					m_eCurrentState = STATE_FRONT_IDLE;
				else if (STATE_SIDE_HIDLE == m_eCurrentState)
					m_eCurrentState = STATE_SIDE_IDLE;
				/////
				if (GetKeyState('W') & 0x8000) // W key pressed: Move upward
				{
					m_isRight = true; // Player is facing right
					
					// m_eCurrentState: Variable to store the player's current animation state (using enum STATE)
					m_eCurrentState = STATE_BACK_RUN; // Play "run back" animation

					if (GetKeyState('D') & 0x8000) // W + D pressed: Move diagonally (up-right)
					{
						m_isRight = true;
						m_eCurrentState = STATE_SIDE_RUN; // Play "run side" animation
						m_fX += (atan2(m_fSpeed, m_fSpeed))*m_fSpeed; // Increase X (move right)
						m_fY -= (atan2(m_fSpeed, m_fSpeed))*m_fSpeed; // Decrease Y (move up)
					}
					else if (GetKeyState('A') & 0x8000) // W + A pressed: Move diagonally (up-left)
					{
						m_isRight = false;
						m_eCurrentState = STATE_SIDE_RUN;
						m_fX -= (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
						m_fY -= (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
					}
					else
						m_fY -= m_fSpeed;  // Only W pressed: Move strictly upward
				}
				else if (GetKeyState('S') & 0x8000)
				{
					m_isRight = true;
					m_eCurrentState = STATE_FRONT_RUN;

					if (GetKeyState('D') & 0x8000)
					{
						m_isRight = true;
						m_eCurrentState = STATE_SIDE_RUN;
						m_fX += (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
						m_fY += (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
					}
					else if (GetKeyState('A') & 0x8000)
					{
						m_isRight = false;
						m_eCurrentState = STATE_SIDE_RUN;
						m_fX -= (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
						m_fY += (atan2(m_fSpeed, m_fSpeed)) * m_fSpeed;
					}
					else
						m_fY += m_fSpeed;
				}

				// Only A key pressed: Move strictly left (not combined with W or S)
				if (GetKeyState('A') & 0x8000 && !(GetKeyState('W') & 0x8000) && !(GetKeyState('S') & 0x8000))
				{
					m_isRight = false; // Player is facing left
					m_eCurrentState = STATE_SIDE_RUN; // Play "run side" animation
					m_fX -= m_fSpeed; // Decrease X (move left)
				}

				// Only D key pressed: Move strictly right (not combined with W or S)
				if (GetKeyState('D') & 0x8000 && !(GetKeyState('W') & 0x8000) && !(GetKeyState('S') & 0x8000))
				{
					m_isRight = true;// Player is facing right
					m_eCurrentState = STATE_SIDE_RUN;
					m_fX += m_fSpeed; // Increase X (move right)
				}

				// None of the movement keys (W, A, S, D) are pressed: Stop movement
				if (!(GetKeyState('W') & 0x8000) && !(GetKeyState('S') & 0x8000) && !(GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
				{
					// Reset frame if not swinging/slashing
					if (STATE_FRONT_SWING != m_eCurrentState && STATE_BACK_SWING != m_eCurrentState && STATE_SIDE_SWING != m_eCurrentState &&
						STATE_FRONT_SLASH != m_eCurrentState && STATE_BACK_SLASH != m_eCurrentState &&STATE_SIDE_SLASH != m_eCurrentState)
						m_fFrame = 0; // Reset animation frame to 0

					// Switch to idle state if player was running
					if (STATE_BACK_RUN == m_eCurrentState)
						m_eCurrentState = STATE_BACK_IDLE;
					else if (STATE_FRONT_RUN == m_eCurrentState)
						m_eCurrentState = STATE_FRONT_IDLE;
					else if (STATE_SIDE_RUN == m_eCurrentState)
						m_eCurrentState = STATE_SIDE_IDLE;
				}
			}/////

			Set_Position(_vec3(m_fX, m_fY, 0.f));
			m_YsortRect.bottom = m_matTransform._42 + m_matTransform._22*0.5f;
		}
		else if (m_eCurrentState == STATE_SIDE_FISHSWING || m_eCurrentState == STATE_BACK_FISHSWING || m_eCurrentState == STATE_FRONT_FISHSWING)
		{
			if (5 <= m_fFrame)
				if (GetKeyState(VK_LBUTTON) & 0x8000)
					if (STATE_BACK_FISHSWING == m_eCurrentState)
					{
						m_fFrame = 0.f;
						m_eCurrentState = STATE_BACK_IDLE;
					}
					else if (STATE_FRONT_FISHSWING == m_eCurrentState)
					{
						m_fFrame = 0.f;
						m_eCurrentState = STATE_FRONT_IDLE;
					}
					else if (STATE_SIDE_FISHSWING == m_eCurrentState)
					{
						m_fFrame = 0.f;
						m_eCurrentState = STATE_SIDE_IDLE;
					}
		}

		CGameObject* pFishing = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Fishing");
		if (true == pFishing->Get_FishingOn())
		{
			if (STATE_BACK_FISHSWING == m_eCurrentState)
			{
				m_fFrame = 0.f;
				m_eCurrentState = STATE_BACK_FISHING;
			}
			else if (STATE_FRONT_FISHSWING == m_eCurrentState)
			{
				m_fFrame = 0.f;
				m_eCurrentState = STATE_FRONT_FISHING;
			}
			else if (STATE_SIDE_FISHSWING == m_eCurrentState)
			{
				m_fFrame = 0.f;
				m_eCurrentState = STATE_SIDE_FISHING;
			}
		}
		else
		{
			if (STATE_BACK_FISHING == m_eCurrentState)
			{
				m_fFrame = 0.f;
				m_eCurrentState = STATE_BACK_IDLE;
			}
			else if (STATE_FRONT_FISHING == m_eCurrentState)
			{
				m_fFrame = 0.f;
				m_eCurrentState = STATE_FRONT_IDLE;
			}
			else if (STATE_SIDE_FISHING == m_eCurrentState)
			{
				m_fFrame = 0.f;
				m_eCurrentState = STATE_SIDE_IDLE;
			}
		}
		if (true == pFishing->Get_GetFish())
		{
			m_fFrame = 0;
			m_isRight = true;
			m_eCurrentState = STATE_GET_FISH;
		}


		if (!(GetKeyState(VK_LBUTTON) & 0x8000) && true == m_isClick && 0 == m_fFrame)
		{
			m_isClick = false;
		}

		CGameObject* pFarming = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Farming");
		if (nullptr != pFarming)
		{
			if (true == pFarming->Get_HarvestOn())
			{
				if (STATE_BACK_IDLE == m_eCurrentState)
				{
					m_fFrame = 0;
					m_eCurrentState = STATE_BACK_HARVEST;
				}
				else if (STATE_FRONT_IDLE == m_eCurrentState)
				{
					m_fFrame = 0;
					m_eCurrentState = STATE_FRONT_HARVEST;
				}
				else if (STATE_SIDE_IDLE == m_eCurrentState)
				{
					m_fFrame = 0;
					m_eCurrentState = STATE_SIDE_HARVEST;
				}
			}
		}

		if (false == m_isRight)
			Set_RotationY(D3DXToRadian(180.f));
		else
			Set_RotationY(D3DXToRadian(0.f));


		if (true == m_bBlink)
		{
			m_fBlinkAcc += fTimeDelta;
			if (m_fBlinkAcc >= 0.03f)
			{
				++m_iBlinkCount;
				m_fBlinkAcc = 0.f;
				if(m_bBlinkOn == false)
					m_bBlinkOn = true;
				else if(m_bBlinkOn == true)
					m_bBlinkOn = false;
				if (30 == m_iBlinkCount)
				{
					m_bBlinkOn = false;
					m_bBlink = false;
					m_iBlinkCount = 0;
				}
			}
		}
	}


	return _int();
}

_int CPlayer::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if (false == m_bBlinkOn)
	{
		if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_YSORT, this)))
			return -1;
	}

	return _int();
}

void CPlayer::Render_GameObject()
{
	if (nullptr == m_pBuffer ||
		nullptr == m_pTexture)
		return;

	const CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");

	POSITION MyMatrix = ((const CCamera*)pCamera)->Transform(m_fX, m_fY);
	_matrix TempMatrix;
	TempMatrix = m_matTransform;

	memcpy(&TempMatrix.m[STATE_POSITION][0], &MyMatrix, sizeof(_vec3));

	m_pTexture[m_eCurrentState]->SetUp_OnGraphicDev(_uint(m_fFrame));

	SetUp_RenderState();

	m_pBuffer->Render_Buffer(TempMatrix);

	if (GetKeyState(VK_F1))
	{
		m_pRect->SetUp_OnGraphicDev(0);
		TempMatrix._11 = 50.f;
		TempMatrix._22 = 32.f;
		TempMatrix._42 = TempMatrix._42 + 57.f;
		m_pBuffer->Render_Buffer(TempMatrix);
	}

	// 아이템 드는 모션
	if (true==m_pUI->Get_ItemOn().isHandle)
	{
		if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"ParsnipSeed")))
		{
			m_pIcon[0]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 55.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"StrawberrySeed")))
		{
			m_pIcon[1]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"PotatoSeed")))
		{
			m_pIcon[2]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Parsnip")))
		{
			m_pIcon[3]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Strawberry")))
		{
			m_pIcon[4]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Potato")))
		{
			m_pIcon[5]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Bass")))
		{
			m_pIcon[6]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Salmon")))
		{
			m_pIcon[7]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Sunfish")))
		{
			m_pIcon[8]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Branch")))
		{
			m_pIcon[9]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Stone")))
		{
			m_pIcon[10]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Weed")))
		{
			m_pIcon[11]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Copper")))
		{
			m_pIcon[12]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Slime")))
		{
			m_pIcon[13]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		else if ((0 == wcscmp(m_pUI->Get_ItemOn().szName, L"Chest")))
		{
			m_pIcon[14]->SetUp_OnGraphicDev(0);
			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._42 = TempMatrix._42 - 60.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
	}
	if(m_eCurrentState == STATE_BACK_HARVEST || m_eCurrentState == STATE_SIDE_HARVEST || m_eCurrentState == STATE_FRONT_HARVEST)
	{
		if (m_fFrame >= 2)
		{
			CGameObject* pFarming = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Farming");

			if (1 == pFarming->Get_Crop())
			{
				m_pIcon[3]->SetUp_OnGraphicDev(0);
				TempMatrix._11 = 64.f;
				TempMatrix._22 = 64.f;
				TempMatrix._42 = TempMatrix._42 - 60.f;
				m_pBuffer->Render_Buffer(TempMatrix);
			}
			else if (2 == pFarming->Get_Crop())
			{
				m_pIcon[4]->SetUp_OnGraphicDev(0);
				TempMatrix._11 = 64.f;
				TempMatrix._22 = 64.f;
				TempMatrix._42 = TempMatrix._42 - 60.f;
				m_pBuffer->Render_Buffer(TempMatrix);
			}
			else if (3 == pFarming->Get_Crop())
			{
				m_pIcon[5]->SetUp_OnGraphicDev(0);
				TempMatrix._11 = 64.f;
				TempMatrix._22 = 64.f;
				TempMatrix._42 = TempMatrix._42 - 60.f;
				m_pBuffer->Render_Buffer(TempMatrix);
			}
		}
	}
	if (m_eCurrentState == STATE_GET_FISH)
	{
		m_fFrame = 0;
		CGameObject* pFishing = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Fishing");

		if (true == pFishing->Get_GetFish())
		{
			if (m_bOnce == false)
			{
				m_bOnce = true;
				m_iFishNumber = rand() % 3;
			}

			if(0 == m_iFishNumber)
				m_pIcon[6]->SetUp_OnGraphicDev(0);
			else if(1 == m_iFishNumber)
				m_pIcon[7]->SetUp_OnGraphicDev(0);
			else if(2 == m_iFishNumber)
				m_pIcon[8]->SetUp_OnGraphicDev(0);

			TempMatrix._11 = 64.f;
			TempMatrix._22 = 64.f;
			TempMatrix._41 = TempMatrix._41 - 30.f;
			TempMatrix._42 = TempMatrix._42 - 30.f;
			m_pBuffer->Render_Buffer(TempMatrix);
		}
		
	}

	Reset_RenderState();
}

HRESULT CPlayer::SetUp_StateDesc()
{
	m_pStateDesc = new STATEDESC[STATE_END];
	ZeroMemory(m_pStateDesc, sizeof(STATEDESC) * STATE_END);

	m_pStateDesc[STATE_FRONT_IDLE].eState = STATE_FRONT_IDLE;
	m_pStateDesc[STATE_FRONT_IDLE].iNumTexture = 1;

	m_pStateDesc[STATE_BACK_IDLE].eState = STATE_BACK_IDLE;
	m_pStateDesc[STATE_BACK_IDLE].iNumTexture = 1;

	m_pStateDesc[STATE_SIDE_IDLE].eState = STATE_SIDE_IDLE;
	m_pStateDesc[STATE_SIDE_IDLE].iNumTexture = 1;

	m_pStateDesc[STATE_FRONT_HIDLE].eState = STATE_FRONT_HIDLE;
	m_pStateDesc[STATE_FRONT_HIDLE].iNumTexture = 1;

	m_pStateDesc[STATE_BACK_HIDLE].eState = STATE_BACK_HIDLE;
	m_pStateDesc[STATE_BACK_HIDLE].iNumTexture = 1;

	m_pStateDesc[STATE_SIDE_HIDLE].eState = STATE_SIDE_HIDLE;
	m_pStateDesc[STATE_SIDE_HIDLE].iNumTexture = 1;

	m_pStateDesc[STATE_FRONT_RUN].eState = STATE_FRONT_RUN;
	m_pStateDesc[STATE_FRONT_RUN].iNumTexture = 8;

	m_pStateDesc[STATE_BACK_RUN].eState = STATE_BACK_RUN;
	m_pStateDesc[STATE_BACK_RUN].iNumTexture = 8;

	m_pStateDesc[STATE_SIDE_RUN].eState = STATE_SIDE_RUN;
	m_pStateDesc[STATE_SIDE_RUN].iNumTexture = 8;

	m_pStateDesc[STATE_FRONT_HRUN].eState = STATE_FRONT_HRUN;
	m_pStateDesc[STATE_FRONT_HRUN].iNumTexture = 8;

	m_pStateDesc[STATE_BACK_HRUN].eState = STATE_BACK_HRUN;
	m_pStateDesc[STATE_BACK_HRUN].iNumTexture = 8;

	m_pStateDesc[STATE_SIDE_HRUN].eState = STATE_SIDE_HRUN;
	m_pStateDesc[STATE_SIDE_HRUN].iNumTexture = 8;

	m_pStateDesc[STATE_FRONT_SWING].eState = STATE_FRONT_SWING;
	m_pStateDesc[STATE_FRONT_SWING].iNumTexture = 5;

	m_pStateDesc[STATE_BACK_SWING].eState = STATE_BACK_SWING;
	m_pStateDesc[STATE_BACK_SWING].iNumTexture = 5;

	m_pStateDesc[STATE_SIDE_SWING].eState = STATE_SIDE_SWING;
	m_pStateDesc[STATE_SIDE_SWING].iNumTexture = 5;

	m_pStateDesc[STATE_FRONT_SLASH].eState = STATE_FRONT_SLASH;
	m_pStateDesc[STATE_FRONT_SLASH].iNumTexture = 6;

	m_pStateDesc[STATE_BACK_SLASH].eState = STATE_BACK_SLASH;
	m_pStateDesc[STATE_BACK_SLASH].iNumTexture = 6;

	m_pStateDesc[STATE_SIDE_SLASH].eState = STATE_SIDE_SLASH;
	m_pStateDesc[STATE_SIDE_SLASH].iNumTexture = 6;

	m_pStateDesc[STATE_FRONT_WATERING].eState = STATE_FRONT_WATERING;
	m_pStateDesc[STATE_FRONT_WATERING].iNumTexture = 3;

	m_pStateDesc[STATE_BACK_WATERING].eState = STATE_BACK_WATERING;
	m_pStateDesc[STATE_BACK_WATERING].iNumTexture = 3;

	m_pStateDesc[STATE_SIDE_WATERING].eState = STATE_SIDE_WATERING;
	m_pStateDesc[STATE_SIDE_WATERING].iNumTexture = 3;

	m_pStateDesc[STATE_FRONT_HARVEST].eState = STATE_FRONT_HARVEST;
	m_pStateDesc[STATE_FRONT_HARVEST].iNumTexture = 4;

	m_pStateDesc[STATE_BACK_HARVEST].eState = STATE_BACK_HARVEST;
	m_pStateDesc[STATE_BACK_HARVEST].iNumTexture = 4;

	m_pStateDesc[STATE_SIDE_HARVEST].eState = STATE_SIDE_HARVEST;
	m_pStateDesc[STATE_SIDE_HARVEST].iNumTexture = 4;

	m_pStateDesc[STATE_FRONT_FISHSWING].eState = STATE_FRONT_FISHSWING;
	m_pStateDesc[STATE_FRONT_FISHSWING].iNumTexture = 6;

	m_pStateDesc[STATE_BACK_FISHSWING].eState = STATE_BACK_FISHSWING;
	m_pStateDesc[STATE_BACK_FISHSWING].iNumTexture = 6;

	m_pStateDesc[STATE_SIDE_FISHSWING].eState = STATE_SIDE_FISHSWING;
	m_pStateDesc[STATE_SIDE_FISHSWING].iNumTexture = 6;

	m_pStateDesc[STATE_FRONT_FISHING].eState = STATE_FRONT_FISHING;
	m_pStateDesc[STATE_FRONT_FISHING].iNumTexture = 2;

	m_pStateDesc[STATE_BACK_FISHING].eState = STATE_BACK_FISHING;
	m_pStateDesc[STATE_BACK_FISHING].iNumTexture = 2;

	m_pStateDesc[STATE_SIDE_FISHING].eState = STATE_SIDE_FISHING;
	m_pStateDesc[STATE_SIDE_FISHING].iNumTexture = 2;

	m_pStateDesc[STATE_GET_FISH].eState = STATE_GET_FISH;
	m_pStateDesc[STATE_GET_FISH].iNumTexture = 1;

	return NOERROR;
}



HRESULT CPlayer::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	// For.Texture_Stand
	m_pTexture[STATE_FRONT_IDLE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Idle");
	if (nullptr == m_pTexture[STATE_FRONT_IDLE])
		return E_FAIL;
	m_pTexture[STATE_FRONT_IDLE]->AddRef();

	m_pTexture[STATE_BACK_IDLE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Idle");
	if (nullptr == m_pTexture[STATE_BACK_IDLE])
		return E_FAIL;
	m_pTexture[STATE_BACK_IDLE]->AddRef();

	m_pTexture[STATE_SIDE_IDLE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Idle");
	if (nullptr == m_pTexture[STATE_SIDE_IDLE])
		return E_FAIL;
	m_pTexture[STATE_SIDE_IDLE]->AddRef();

	m_pTexture[STATE_FRONT_HIDLE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_HIdle");
	if (nullptr == m_pTexture[STATE_FRONT_HIDLE])
		return E_FAIL;
	m_pTexture[STATE_FRONT_HIDLE]->AddRef();

	m_pTexture[STATE_BACK_HIDLE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_HIdle");
	if (nullptr == m_pTexture[STATE_BACK_HIDLE])
		return E_FAIL;
	m_pTexture[STATE_BACK_HIDLE]->AddRef();

	m_pTexture[STATE_SIDE_HIDLE] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_HIdle");
	if (nullptr == m_pTexture[STATE_SIDE_HIDLE])
		return E_FAIL;
	m_pTexture[STATE_SIDE_HIDLE]->AddRef();

	// For.Texture_Run
	m_pTexture[STATE_FRONT_RUN] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Run");
	if (nullptr == m_pTexture[STATE_FRONT_RUN])
		return E_FAIL;
	m_pTexture[STATE_FRONT_RUN]->AddRef();

	m_pTexture[STATE_BACK_RUN] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Run");
	if (nullptr == m_pTexture[STATE_BACK_RUN])
		return E_FAIL;
	m_pTexture[STATE_BACK_RUN]->AddRef();

	m_pTexture[STATE_SIDE_RUN] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Run");
	if (nullptr == m_pTexture[STATE_SIDE_RUN])
		return E_FAIL;
	m_pTexture[STATE_SIDE_RUN]->AddRef();

	m_pTexture[STATE_FRONT_HRUN] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_HRun");
	if (nullptr == m_pTexture[STATE_FRONT_HRUN])
		return E_FAIL;
	m_pTexture[STATE_FRONT_HRUN]->AddRef();

	m_pTexture[STATE_BACK_HRUN] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_HRun");
	if (nullptr == m_pTexture[STATE_BACK_HRUN])
		return E_FAIL;
	m_pTexture[STATE_BACK_HRUN]->AddRef();

	m_pTexture[STATE_SIDE_HRUN] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_HRun");
	if (nullptr == m_pTexture[STATE_SIDE_HRUN])
		return E_FAIL;
	m_pTexture[STATE_SIDE_HRUN]->AddRef();

	// For.Texture_Swing
	m_pTexture[STATE_FRONT_SWING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Swing");
	if (nullptr == m_pTexture[STATE_FRONT_SWING])
		return E_FAIL;
	m_pTexture[STATE_FRONT_SWING]->AddRef();

	m_pTexture[STATE_BACK_SWING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Swing");
	if (nullptr == m_pTexture[STATE_BACK_SWING])
		return E_FAIL;
	m_pTexture[STATE_BACK_SWING]->AddRef();

	m_pTexture[STATE_SIDE_SWING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Swing");
	if (nullptr == m_pTexture[STATE_SIDE_SWING])
		return E_FAIL;
	m_pTexture[STATE_SIDE_SWING]->AddRef();

	// For.Texture_Slash
	m_pTexture[STATE_FRONT_SLASH] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Slash");
	if (nullptr == m_pTexture[STATE_FRONT_SLASH])
		return E_FAIL;
	m_pTexture[STATE_FRONT_SLASH]->AddRef();

	m_pTexture[STATE_BACK_SLASH] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Slash");
	if (nullptr == m_pTexture[STATE_BACK_SLASH])
		return E_FAIL;
	m_pTexture[STATE_BACK_SLASH]->AddRef();

	m_pTexture[STATE_SIDE_SLASH] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Slash");
	if (nullptr == m_pTexture[STATE_SIDE_SLASH])
		return E_FAIL;
	m_pTexture[STATE_SIDE_SLASH]->AddRef();

	// For.Texture_Watering
	m_pTexture[STATE_FRONT_WATERING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Watering");
	if (nullptr == m_pTexture[STATE_FRONT_WATERING])
		return E_FAIL;
	m_pTexture[STATE_FRONT_WATERING]->AddRef();

	m_pTexture[STATE_BACK_WATERING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Watering");
	if (nullptr == m_pTexture[STATE_BACK_WATERING])
		return E_FAIL;
	m_pTexture[STATE_BACK_WATERING]->AddRef();

	m_pTexture[STATE_SIDE_WATERING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Watering");
	if (nullptr == m_pTexture[STATE_SIDE_WATERING])
		return E_FAIL;
	m_pTexture[STATE_SIDE_WATERING]->AddRef();

	// For.Texture_Harvest
	m_pTexture[STATE_FRONT_HARVEST] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Harvest");
	if (nullptr == m_pTexture[STATE_FRONT_HARVEST])
		return E_FAIL;
	m_pTexture[STATE_FRONT_HARVEST]->AddRef();

	m_pTexture[STATE_BACK_HARVEST] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Harvest");
	if (nullptr == m_pTexture[STATE_BACK_HARVEST])
		return E_FAIL;
	m_pTexture[STATE_BACK_HARVEST]->AddRef();

	m_pTexture[STATE_SIDE_HARVEST] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Harvest");
	if (nullptr == m_pTexture[STATE_SIDE_HARVEST])
		return E_FAIL;
	m_pTexture[STATE_SIDE_HARVEST]->AddRef();

	// For.Texture_Fishing
	m_pTexture[STATE_FRONT_FISHSWING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_FishSwing");
	if (nullptr == m_pTexture[STATE_FRONT_FISHSWING])
		return E_FAIL;
	m_pTexture[STATE_FRONT_FISHSWING]->AddRef();

	m_pTexture[STATE_BACK_FISHSWING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_FishSwing");
	if (nullptr == m_pTexture[STATE_BACK_FISHSWING])
		return E_FAIL;
	m_pTexture[STATE_BACK_FISHSWING]->AddRef();

	m_pTexture[STATE_SIDE_FISHSWING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_FishSwing");
	if (nullptr == m_pTexture[STATE_SIDE_FISHSWING])
		return E_FAIL;
	m_pTexture[STATE_SIDE_FISHSWING]->AddRef();

	m_pTexture[STATE_FRONT_FISHING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Front_Fishing");
	if (nullptr == m_pTexture[STATE_FRONT_FISHING])
		return E_FAIL;
	m_pTexture[STATE_FRONT_FISHING]->AddRef();

	m_pTexture[STATE_BACK_FISHING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Back_Fishing");
	if (nullptr == m_pTexture[STATE_BACK_FISHING])
		return E_FAIL;
	m_pTexture[STATE_BACK_FISHING]->AddRef();

	m_pTexture[STATE_SIDE_FISHING] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_Side_Fishing");
	if (nullptr == m_pTexture[STATE_SIDE_FISHING])
		return E_FAIL;
	m_pTexture[STATE_SIDE_FISHING]->AddRef();

	m_pTexture[STATE_GET_FISH] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Player_GetFishing");
	if (nullptr == m_pTexture[STATE_GET_FISH])
		return E_FAIL;
	m_pTexture[STATE_GET_FISH]->AddRef();


	// For. Texture_Icon
	m_pIcon[0] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_ParsnipSeed");
	if (nullptr == m_pIcon[0])
		return E_FAIL;
	m_pIcon[0]->AddRef();

	m_pIcon[1] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_StrawberrySeed");
	if (nullptr == m_pIcon[1])
		return E_FAIL;
	m_pIcon[1]->AddRef();

	m_pIcon[2] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_PotatoSeed");
	if (nullptr == m_pIcon[2])
		return E_FAIL;
	m_pIcon[2]->AddRef();

	m_pIcon[3] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Parsnip");
	if (nullptr == m_pIcon[3])
		return E_FAIL;
	m_pIcon[3]->AddRef();

	m_pIcon[4] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Strawberry");
	if (nullptr == m_pIcon[4])
		return E_FAIL;
	m_pIcon[4]->AddRef();

	m_pIcon[5] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Potato");
	if (nullptr == m_pIcon[5])
		return E_FAIL;
	m_pIcon[5]->AddRef();

	m_pIcon[6] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Bass");
	if (nullptr == m_pIcon[6])
		return E_FAIL;
	m_pIcon[6]->AddRef();

	m_pIcon[7] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Salmon");
	if (nullptr == m_pIcon[7])
		return E_FAIL;
	m_pIcon[7]->AddRef();

	m_pIcon[8] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Sunfish");
	if (nullptr == m_pIcon[8])
		return E_FAIL;
	m_pIcon[8]->AddRef();

	m_pIcon[9] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Branch");
	if (nullptr == m_pIcon[9])
		return E_FAIL;
	m_pIcon[9]->AddRef();

	m_pIcon[10] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Stone");
	if (nullptr == m_pIcon[10])
		return E_FAIL;
	m_pIcon[10]->AddRef();

	m_pIcon[11] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Weed");
	if (nullptr == m_pIcon[11])
		return E_FAIL;
	m_pIcon[11]->AddRef();

	m_pIcon[12] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Copper");
	if (nullptr == m_pIcon[12])
		return E_FAIL;
	m_pIcon[12]->AddRef();

	m_pIcon[13] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Slime");
	if (nullptr == m_pIcon[13])
		return E_FAIL;
	m_pIcon[13]->AddRef();

	m_pIcon[14] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Chest");
	if (nullptr == m_pIcon[14])
		return E_FAIL;
	m_pIcon[14]->AddRef();

	// Rect
	m_pRect = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Rect");
	if (nullptr == m_pRect)
		return E_FAIL;
	m_pRect->AddRef();

	return NOERROR;
}

HRESULT CPlayer::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return NOERROR;
}

HRESULT CPlayer::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	

	return NOERROR;
}

HRESULT CPlayer::Animation(_float fTimeDelta)
{
	if(m_eCurrentState == STATE_SIDE_SWING || m_eCurrentState == STATE_SIDE_SLASH ||
		m_eCurrentState == STATE_SIDE_HARVEST)
	{ 
		m_fFrame += m_pStateDesc[m_eCurrentState].iNumTexture * fTimeDelta * 3.f;


		if (m_fFrame >= m_pStateDesc[m_eCurrentState].iNumTexture)
		{
			++i;
			if (2 == i)
			{
				m_fFrame = 0;
				m_pPlayerLook = 0;
				m_eCurrentState = STATE_SIDE_IDLE;
				i = 0;
			}
			else
				--m_fFrame;
		}
	}
	else if (m_eCurrentState == STATE_FRONT_SWING || m_eCurrentState == STATE_FRONT_SLASH ||
		m_eCurrentState == STATE_FRONT_HARVEST)
	{
		m_fFrame += m_pStateDesc[m_eCurrentState].iNumTexture * fTimeDelta * 3.f;
		if (m_fFrame >= m_pStateDesc[m_eCurrentState].iNumTexture)
		{
			++i;
			if (2 == i)
			{
				m_fFrame = 0;
				m_pPlayerLook = 0;
				m_eCurrentState = STATE_FRONT_IDLE;
				i = 0;
			}
			else
				--m_fFrame;
		}
	}
	else if (m_eCurrentState == STATE_BACK_SWING || m_eCurrentState == STATE_BACK_SLASH ||
		m_eCurrentState == STATE_BACK_HARVEST)
	{
		m_fFrame += m_pStateDesc[m_eCurrentState].iNumTexture * fTimeDelta * 3.f;
		if (m_fFrame >= m_pStateDesc[m_eCurrentState].iNumTexture)
		{
			++i;
			if (2 == i)
			{
				m_fFrame = 0;
				m_pPlayerLook = 0;
				m_eCurrentState = STATE_BACK_IDLE;
				i = 0;
			}
			else
				--m_fFrame;
		}
	}
	else if (m_eCurrentState == STATE_BACK_WATERING || m_eCurrentState == STATE_FRONT_WATERING || m_eCurrentState == STATE_SIDE_WATERING)
	{
		m_fFrame += m_pStateDesc[m_eCurrentState].iNumTexture * fTimeDelta * 3.f;
		if (m_fFrame >= m_pStateDesc[m_eCurrentState].iNumTexture)
		{
			++i;
			if (4 == i)
			{
				m_fFrame = 0;
				m_pPlayerLook = 0;
				if(m_eCurrentState == STATE_BACK_WATERING)
					m_eCurrentState = STATE_BACK_IDLE;
				else if(m_eCurrentState == STATE_SIDE_WATERING)
					m_eCurrentState = STATE_SIDE_IDLE;
				else if (m_eCurrentState == STATE_FRONT_WATERING)
					m_eCurrentState = STATE_FRONT_IDLE;

				i = 0;
			}
			else
				--m_fFrame;
		}
	}
	else if (m_eCurrentState == STATE_FRONT_FISHSWING || m_eCurrentState == STATE_BACK_FISHSWING|| m_eCurrentState == STATE_SIDE_FISHSWING)
	{
		m_fFrame += m_pStateDesc[m_eCurrentState].iNumTexture * fTimeDelta * 3.f;
		if (m_fFrame >= m_pStateDesc[m_eCurrentState].iNumTexture)
			--m_fFrame;
	}
	else if (m_eCurrentState == STATE_FRONT_FISHING || m_eCurrentState == STATE_SIDE_FISHING || m_eCurrentState == STATE_BACK_FISHING)
	{
		m_fFrame += m_pStateDesc[m_eCurrentState].iNumTexture * fTimeDelta * 5.f;
		if (m_fFrame >= m_pStateDesc[m_eCurrentState].iNumTexture)
			m_fFrame = 0;
	}
	else
	{
		m_fFrame += m_pStateDesc[m_eCurrentState].iNumTexture * fTimeDelta * 1.5f;

		if (m_fFrame >= m_pStateDesc[m_eCurrentState].iNumTexture)
			m_fFrame = 0;
	}
	
	return NOERROR;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*		pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CPlayer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	Safe_Delete_Array(m_pStateDesc);

	Safe_Release(m_pUI);
	for (auto& pTexture : m_pTexture)	
		Safe_Release(pTexture);	
	for (auto& pIcon : m_pIcon)
		Safe_Release(pIcon);

	Safe_Release(m_pRect);

	Safe_Release(m_pBuffer);

	CGameObject::Free();
}