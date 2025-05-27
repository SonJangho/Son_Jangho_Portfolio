#include "stdafx.h"
#include "..\Headers\BasicUI.h"
#include "Sound_Manager.h"

CBasicUI::CBasicUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

HRESULT CBasicUI::Ready_GameObject()
{
	CGameObject::Ready_GameObject();	

	CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Inventory");
	if (nullptr == pTarget)
		return E_FAIL;

	m_pInventory = pTarget;
	m_pInventory->AddRef();

	pItemInfo = m_pInventory->Get_Item();

	if (FAILED(SetUp_Resource()))
		return E_FAIL;

	Set_Position(_vec3(g_iBackCX >> 1, g_iBackCY >> 1, 0.f));

	Set_Scale(_vec3(2048.f, 1024.0f, 1.f));

	m_fX = 288.f;
	m_fY = 700.5f;
	m_fTimeDegree = 90.f;
	m_fTimeSpeed = 1.f;
	m_fSelectX = m_fX;
	m_fSelectY = m_fY;

	m_iGold = 3000;

	return NOERROR;
}

_int CBasicUI::Update_GameObject(const _float & fTimeDelta)
{
	if ((GetKeyState(VK_TAB) & 0x8000 && m_bOnce == false))
	{
		m_bOnce = true;
		if(m_bUIOn == true)
			m_bUIOn = false;
		else if (m_bUIOn == false)
			m_bUIOn = true;
	}
	else if (!(GetKeyState(VK_TAB) & 0x8000) && m_bOnce == true)
		m_bOnce = false;

	if(m_bUIOn == true && false==m_pInventory->Get_InvenOn())
	{
		if (GetKeyState('1') & 0x8000)
		{
			m_fSelectX = 288.f;
		}
		else if (GetKeyState('2') & 0x8000)
		{
			m_fSelectX = 288.f + 64 * 1;
		}
		else if (GetKeyState('3') & 0x8000)
		{
			m_fSelectX = 288.f + 64 * 2;
		}
		else if (GetKeyState('4') & 0x8000)
		{
			m_fSelectX = 288.f + 64 * 3;
		}
		else if (GetKeyState('5') & 0x8000)
		{
			m_fSelectX = 288.f + 64 * 4;
		}
		else if (GetKeyState('6') & 0x8000)
		{
			m_fSelectX = 288.f + 64 * 5;
		}
		else if (GetKeyState('7') & 0x8000)
		{
			m_fSelectX = 288.f + 64 * 6;
		}
		else if (GetKeyState('8') & 0x8000)
		{
			m_fSelectX = 288.f + 64 * 7;
		}
		else if (GetKeyState('9') & 0x8000)
		{
			m_fSelectX = 288.f + 64 * 8;
		}
		else if (GetKeyState('0') & 0x8000)
		{
			m_fSelectX = 288.f + 64 * 9;
		}
		else if (GetKeyState(VK_OEM_MINUS) & 0x8000)
		{
			m_fSelectX = 288.f + 64 * 10;
		}
		else if (GetKeyState(VK_OEM_PLUS) & 0x8000)
		{
			m_fSelectX = 288.f + 64 * 11;
		}
		int i = (m_fSelectX - 288) / 64;
		m_SelectItem = pItemInfo[i];
	}

	m_fAcc += fTimeDelta;
	if (m_fAcc >= m_fTimeSpeed)
	{
		m_fAcc = 0.f;
		++m_fTimeDegree;
		m_fTime += 0.6722;
	}

	if (m_fTimeDegree == 270.f)
	{
		CSound_Manager::Get_Instance()->SoundStop(18);
		CSound_Manager::Get_Instance()->SoundPlay(6, FALSE);
		
		CSound_Manager::Get_Instance()->SoundStop(2);
		CSound_Manager::Get_Instance()->SoundStop(3);

		m_fTime = 0;
		++m_fDate;
		m_bNextDay = true;
		m_bRainDay = false;
		_int RanNumb = rand() % 5;
		if (3 <= RanNumb)
		{
			if (false == CSound_Manager::Get_Instance()->SoundPlaying(2))
				CSound_Manager::Get_Instance()->SoundPlay(2, TRUE);
			m_bRainDay = true;
		}
		else
			if (false == CSound_Manager::Get_Instance()->SoundPlaying(3))
				CSound_Manager::Get_Instance()->SoundPlay(3, TRUE);

		if (m_fDate == 28)
			m_fDate = 0;
		m_fTimeDegree = 90.f;
	}

	if (m_fTimeDegree == 90.f+9.f*15.f)
	{
		CSound_Manager::Get_Instance()->SoundStop(3);
		if (false == CSound_Manager::Get_Instance()->SoundPlaying(18))
			CSound_Manager::Get_Instance()->SoundPlay(18, TRUE);
	}

	if (GetKeyState(VK_ADD) & 0x8000)
	{
		m_fTimeSpeed -= 0.1f;
		if (0.01f >= m_fTimeSpeed)
			m_fTimeSpeed = 0.01f;
	}

	if (GetKeyState(VK_SUBTRACT) & 0x8000)
	{
		m_fTimeSpeed += 0.1f;
		if (1.f <= m_fTimeSpeed)
			m_fTimeSpeed = 1.f;
	}

	if (GetKeyState(VK_DIVIDE) & 0x8000)
	{
		CSound_Manager::Get_Instance()->SoundStop(3);
		if (false == CSound_Manager::Get_Instance()->SoundPlaying(2))
			CSound_Manager::Get_Instance()->SoundPlay(2, TRUE);
		m_bRainDay = true;
	}

	if (0 == wcscmp(m_SelectItem.szName, L"Chest"))
	{
		CGameObject* pCamera = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_DYNAMIC, L"Layer_Camera");
		CGameObject* pPlayer = CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Player");

		POINT		ptMouse;
		ZeroMemory(&ptMouse, sizeof(POINT));

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		m_iDirtX = ((pCamera->Get_X() - 640) * 0.015625 + ptMouse.x* 0.015625);
		m_iDirtY = ((pCamera->Get_Y() - 384) * 0.015625 + ptMouse.y* 0.015625);

		m_PlayerX = pPlayer->Get_X() * 0.015625;
		m_PlayerY = (pPlayer->Get_Y() + 32) * 0.015625;

		m_bInstallCheck = false;
		
		if(m_PlayerX+1 == m_iDirtX || m_PlayerX == m_iDirtX || m_PlayerX - 1 == m_iDirtX)
			if(m_PlayerY + 1 == m_iDirtY || m_PlayerY == m_iDirtY || m_PlayerY - 1 == m_iDirtY)
				m_bInstallCheck = true;
		
		if (true == m_bInstallCheck)
		{
			if ((GetKeyState(VK_LBUTTON) & 0x8000 && m_bOnce2 == false))
			{
				m_bOnce2 = true;
				m_bInstall = true;
				m_pInventory->Use_Item(L"Chest", 1);
			}
			if (!(GetKeyState(VK_LBUTTON) & 0x8000) && m_bOnce2 == true)
				m_bOnce2 = false;
		}
		
	}

	return _int();
}

_int CBasicUI::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRender_Manager)
		return -1;

	if(m_bUIOn == true && false == m_pInventory->Get_InvenOn())
		if (FAILED(m_pRender_Manager->Add_RenderList(CRender_Manager::RENDER_UI, this)))
			return -1;

	return _int();
}

void CBasicUI::Render_GameObject()
{
	if (nullptr == m_pTexture ||
		nullptr == m_pBuffer)
		return;

	SetUp_RenderState();

	Set_Scale(_vec3(2048.f, 1024.0f, 1.f));
	Set_Position(_vec3(g_iBackCX >> 1, g_iBackCY >> 1, 0.f));
	Set_RotationZ(0 * 3.141592 / 180);
	m_pTexture->SetUp_OnGraphicDev((_uint)m_fFrame);
	m_pBuffer->Render_Buffer(m_matTransform);

	Set_Scale(_vec3(64.f, 64.0f, 1.f));
	Set_Position(_vec3(m_fSelectX, m_fSelectY, 0.f));
	m_pSelect->SetUp_OnGraphicDev((_uint)m_fFrame);
	m_pBuffer->Render_Buffer(m_matTransform);

	for (size_t i = 0; i < 12; i++)
	{
		if (0 != wcscmp(pItemInfo[i].szName, L"NULL"))
		{
			Set_Scale(_vec3(56.f, 56.0f, 1.f));
			Set_Position(_vec3(m_fX + 64.f * i, m_fY, 0.f));
			m_pIcon[pItemInfo[i].iNum]->SetUp_OnGraphicDev((_uint)m_fFrame);
			m_pBuffer->Render_Buffer(m_matTransform);

			if (10 <= pItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(m_fX + 17.f + 64.f * i, m_fY + 26.f, 0.f));
				m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) / 10);
				m_pBuffer->Render_Buffer(m_matTransform);
			}
			if (1 != pItemInfo[i].iCount)
			{
				Set_Scale(_vec3(24.f, 24.0f, 1.f));
				Set_Position(_vec3(m_fX + 31.f + 64.f * i, m_fY + 26.f, 0.f));
				m_pSmallNum->SetUp_OnGraphicDev((_uint)(pItemInfo[i].iCount) % 10);
				m_pBuffer->Render_Buffer(m_matTransform);
			}
		}
	}

	if (0 == wcscmp(m_SelectItem.szName, L"ParsnipSeed") || 0 == wcscmp(m_SelectItem.szName, L"StrawberrySeed") || 0 == wcscmp(m_SelectItem.szName, L"PotatoSeed") || 0 == wcscmp(m_SelectItem.szName, L"Chest"))
	{
		CGameObject* pTarget = (CGameObject*)CObject_Manager::GetInstance()->Get_Object(CObject_Manager::TYPE_STATIC, L"Layer_Farming");
		if (nullptr == pTarget)
			return;

		POINT		ptMouse;
		ZeroMemory(&ptMouse, sizeof(POINT));

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		Set_Scale(_vec3(64.f, 64.0f, 1.f));
		Set_Position(_vec3(ptMouse.x, ptMouse.y, 0.f));
		if (true == pTarget->Get_DirtCheck() || true == m_bInstallCheck)
			m_pCheckTile->SetUp_OnGraphicDev(0);
		else
			m_pCheckTile->SetUp_OnGraphicDev(1);
		m_pBuffer->Render_Buffer(m_matTransform);
	}

	Set_Scale(_vec3(256.f, 32.0f, 1.f));
	Set_Position(_vec3(1168.f, 50.f, 0.f));
	m_pDate->SetUp_OnGraphicDev((_uint)m_fDate);
	m_pBuffer->Render_Buffer(m_matTransform);

	Set_Scale(_vec3(256.f, 64.0f, 1.f));
	Set_Position(_vec3(1166.f, 146.f, 0.f));
	m_pTime->SetUp_OnGraphicDev((_uint)m_fTime);
	m_pBuffer->Render_Buffer(m_matTransform);

	// GOLD
	Set_Scale(_vec3(32.f, 32.0f, 1.f));

	if (0 == m_iGold / 10)
		m_iGoldCheck = 1;
	else if (0 == m_iGold / 100)
		m_iGoldCheck = 2;
	else if (0 == m_iGold / 1000)
		m_iGoldCheck = 3;
	else if (0 == m_iGold / 10000)
		m_iGoldCheck = 4;
	else if (0 == m_iGold / 100000)
		m_iGoldCheck = 5;
	else if (0 == m_iGold / 1000000)
		m_iGoldCheck = 6;
	else if (0 == m_iGold / 10000000)
		m_iGoldCheck = 7;
	else if (0 == m_iGold / 100000000)
		m_iGoldCheck = 8;

	if (m_iGoldCheck >= 8)
	{
		Set_Position(_vec3(1054.f + (0 * 24), 210.f, 0.f));
		m_pBigNum->SetUp_OnGraphicDev(m_iGold / 10000000);
		m_pBuffer->Render_Buffer(m_matTransform);
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7)
	{
		Set_Position(_vec3(1054.f + (1 * 24), 210.f, 0.f));
		m_pBigNum->SetUp_OnGraphicDev((m_iGold / 1000000) % 10);
		m_pBuffer->Render_Buffer(m_matTransform);
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6)
	{
		Set_Position(_vec3(1054.f + (2 * 24), 210.f, 0.f));
		m_pBigNum->SetUp_OnGraphicDev((m_iGold / 100000) % 10);
		m_pBuffer->Render_Buffer(m_matTransform);
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6 || m_iGoldCheck >= 5)
	{
		Set_Position(_vec3(1054.f + (3 * 24), 210.f, 0.f));
		m_pBigNum->SetUp_OnGraphicDev((m_iGold / 10000) % 10);
		m_pBuffer->Render_Buffer(m_matTransform);
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6 || m_iGoldCheck >= 5 || m_iGoldCheck >= 4)
	{
		Set_Position(_vec3(1054.f + (4 * 24), 210.f, 0.f));
		m_pBigNum->SetUp_OnGraphicDev((m_iGold / 1000) % 10);
		m_pBuffer->Render_Buffer(m_matTransform);
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6 || m_iGoldCheck >= 5 || m_iGoldCheck >= 4 || m_iGoldCheck >= 3)
	{
		Set_Position(_vec3(1054.f + (5 * 24), 210.f, 0.f));
		m_pBigNum->SetUp_OnGraphicDev((m_iGold / 100) % 10);
		m_pBuffer->Render_Buffer(m_matTransform);
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6 || m_iGoldCheck >= 5 || m_iGoldCheck >= 4 || m_iGoldCheck >= 3 || m_iGoldCheck >= 2)
	{
		Set_Position(_vec3(1054.f + (6 * 24), 210.f, 0.f));
		m_pBigNum->SetUp_OnGraphicDev((m_iGold / 10) % 10);
		m_pBuffer->Render_Buffer(m_matTransform);
	}

	if (m_iGoldCheck >= 8 || m_iGoldCheck >= 7 || m_iGoldCheck >= 6 || m_iGoldCheck >= 5 || m_iGoldCheck >= 4 || m_iGoldCheck >= 3 || m_iGoldCheck >= 2 || m_iGoldCheck >= 1)
	{
		Set_Position(_vec3(1054.f + (7 * 24), 210.f, 0.f));
		m_pBigNum->SetUp_OnGraphicDev((m_iGold) % 10);
		m_pBuffer->Render_Buffer(m_matTransform);
	}

	
	if (true == m_bRainDay)
	{
		Set_Scale(_vec3(16.f*4.f, 8.f*4.f, 1.f));
		Set_Position(_vec3(1122.f, 94.f, 0.f));
		m_pRainUI->SetUp_OnGraphicDev((_uint)0);
		m_pBuffer->Render_Buffer(m_matTransform);
	}
	
	Set_Scale(_vec3(256.f, 32.0f, 1.f));
	Set_Position(_vec3(1072.f, 93.f, 0.f));
	Set_RotationZ(m_fTimeDegree * 3.141592 / 180);
	m_pClockNeedle->SetUp_OnGraphicDev((_uint)m_fFrame);
	m_pBuffer->Render_Buffer(m_matTransform);
	

	Reset_RenderState();
}

HRESULT CBasicUI::SetUp_Resource()
{
	if (nullptr == m_pResource_Manager)
		return E_FAIL;

	// For.Buffer
	m_pBuffer = (CBuffer_RcTex*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Buffer_RcTex");
	if (nullptr == m_pBuffer)
		return E_FAIL;
	m_pBuffer->AddRef();

	m_pTexture = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BasicUI");
	if (nullptr == m_pTexture)
		return E_FAIL;
	m_pTexture->AddRef();

	m_pClockNeedle = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_ClockNeedle");
	if (nullptr == m_pClockNeedle)
		return E_FAIL;
	m_pClockNeedle->AddRef();

	m_pSelect = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SelectItem");
	if (nullptr == m_pSelect)
		return E_FAIL;
	m_pSelect->AddRef();

	m_pCheckTile = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_CheckTile");
	if (nullptr == m_pCheckTile)
		return E_FAIL;
	m_pCheckTile->AddRef();

	m_pSmallNum = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_SmallNumber");
	if (nullptr == m_pSmallNum)
		return E_FAIL;
	m_pSmallNum->AddRef();

	m_pBigNum = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_BigNumber");
	if (nullptr == m_pBigNum)
		return E_FAIL;
	m_pBigNum->AddRef();

	m_pDate = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Date");
	if (nullptr == m_pDate)
		return E_FAIL;
	m_pDate->AddRef();

	m_pTime = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Time");
	if (nullptr == m_pTime)
		return E_FAIL;
	m_pTime->AddRef();

	m_pRainUI = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_RainUI");
	if (nullptr == m_pRainUI)
		return E_FAIL;
	m_pRainUI->AddRef();

	m_pIcon[pItemInfo[0].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Hoe");
	if (nullptr == m_pIcon[pItemInfo[0].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[0].iNum]->AddRef();

	m_pIcon[pItemInfo[1].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Pick");
	if (nullptr == m_pIcon[pItemInfo[1].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[1].iNum]->AddRef();

	m_pIcon[pItemInfo[2].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Ax");
	if (nullptr == m_pIcon[pItemInfo[2].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[2].iNum]->AddRef();

	m_pIcon[pItemInfo[3].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Sickle");
	if (nullptr == m_pIcon[pItemInfo[3].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[3].iNum]->AddRef();

	m_pIcon[pItemInfo[4].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_WateringPot");
	if (nullptr == m_pIcon[pItemInfo[4].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[4].iNum]->AddRef();

	m_pIcon[pItemInfo[5].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Sword");
	if (nullptr == m_pIcon[pItemInfo[5].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[5].iNum]->AddRef();

	m_pIcon[pItemInfo[6].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_FishingRod");
	if (nullptr == m_pIcon[pItemInfo[6].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[6].iNum]->AddRef();

	m_pIcon[pItemInfo[7].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_ParsnipSeed");
	if (nullptr == m_pIcon[pItemInfo[7].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[7].iNum]->AddRef();

	m_pIcon[pItemInfo[8].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Parsnip");
	if (nullptr == m_pIcon[pItemInfo[8].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[8].iNum]->AddRef();

	m_pIcon[pItemInfo[9].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Strawberry");
	if (nullptr == m_pIcon[pItemInfo[9].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[9].iNum]->AddRef();

	m_pIcon[pItemInfo[10].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Potato");
	if (nullptr == m_pIcon[pItemInfo[10].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[10].iNum]->AddRef();

	m_pIcon[pItemInfo[11].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Bass");
	if (nullptr == m_pIcon[pItemInfo[11].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[11].iNum]->AddRef();

	m_pIcon[pItemInfo[12].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Salmon");
	if (nullptr == m_pIcon[pItemInfo[12].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[12].iNum]->AddRef();

	m_pIcon[pItemInfo[13].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Fish_Sunfish");
	if (nullptr == m_pIcon[pItemInfo[13].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[13].iNum]->AddRef();

	m_pIcon[pItemInfo[14].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Branch");
	if (nullptr == m_pIcon[pItemInfo[14].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[14].iNum]->AddRef();

	m_pIcon[pItemInfo[15].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Stone");
	if (nullptr == m_pIcon[pItemInfo[15].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[15].iNum]->AddRef();

	m_pIcon[pItemInfo[16].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Weed");
	if (nullptr == m_pIcon[pItemInfo[16].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[16].iNum]->AddRef();

	m_pIcon[pItemInfo[17].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_PotatoSeed");
	if (nullptr == m_pIcon[pItemInfo[17].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[17].iNum]->AddRef();

	m_pIcon[pItemInfo[18].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_StrawberrySeed");
	if (nullptr == m_pIcon[pItemInfo[18].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[18].iNum]->AddRef();

	m_pIcon[pItemInfo[19].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Copper");
	if (nullptr == m_pIcon[pItemInfo[19].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[19].iNum]->AddRef();

	m_pIcon[pItemInfo[20].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Slime");
	if (nullptr == m_pIcon[pItemInfo[20].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[20].iNum]->AddRef();

	m_pIcon[pItemInfo[21].iNum] = (CTexture*)m_pResource_Manager->Get_Resource(CResource_Manager::TYPE_STATIC, L"Resource_Texture_Icon_Chest");
	if (nullptr == m_pIcon[pItemInfo[21].iNum])
		return E_FAIL;
	m_pIcon[pItemInfo[21].iNum]->AddRef();

	return NOERROR;
}

HRESULT CBasicUI::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return NOERROR;
}

HRESULT CBasicUI::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return NOERROR;
}

HRESULT CBasicUI::Animation(_float fTimeDelta)
{
	return NOERROR;
}

CGameObject * CBasicUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBasicUI*		pInstance = new CBasicUI(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBasicUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBasicUI::Free(void)
{
	Safe_Release(m_pInventory);
	Safe_Release(m_pTexture);
	Safe_Release(m_pClockNeedle);
	Safe_Release(m_pSelect);
	Safe_Release(m_pCheckTile);
	Safe_Release(m_pSmallNum);
	Safe_Release(m_pBigNum);
	Safe_Release(m_pDate);
	Safe_Release(m_pTime);
	Safe_Release(m_pRainUI);

	for (auto& m_pIcon : m_pIcon)
		Safe_Release(m_pIcon);
	Safe_Release(m_pBuffer);

	CGameObject::Free();
}
