#pragma once

#include "GameObject.h"

class CBasicUI final : public CGameObject
{
private:
	explicit CBasicUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBasicUI() = default;
public:
	virtual HRESULT Ready_GameObject();	
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	ITEMINFO	Get_ItemOn() {
		return m_SelectItem;
	}
	_float		Get_TimeDegree() {
		return m_fTimeDegree;
	}
	_bool		Get_NextDay() {
		return m_bNextDay;
	}
	void		Set_NextDay() {
		m_bNextDay = false;
	}
	_int		Get_Gold() {
		return m_iGold;
	}
	void		Set_Gold(_int Gold) {
		m_iGold += Gold;
	}
	_bool		Get_RainDay() {
		return m_bRainDay;
	}
	_bool		Get_Install() {
		return m_bInstall;
	}
	void		Set_Install() {
		m_bInstall = false;
	}
	_int		Get_DirtX() {
		return m_iDirtX;
	}
	_int		Get_DirtY() {
		return m_iDirtY;
	}
	void		Set_UI(_bool UI) {
		m_bUIOn = UI;
	}
private:		
	CTexture*				m_pTexture = nullptr;
	CTexture*				m_pIcon[25] = { nullptr };
	CTexture*				m_pSelect = nullptr;
	CTexture*				m_pCheckTile = nullptr;
	CTexture*				m_pSmallNum = nullptr;
	CTexture*				m_pBigNum = nullptr;
	CTexture*				m_pClockNeedle = nullptr;
	CTexture*				m_pDate = nullptr;
	CTexture*				m_pTime = nullptr;
	CTexture*				m_pRainUI = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	CGameObject*			m_pInventory = nullptr;
	ITEMINFO*				pItemInfo;
private:
	_float					m_fFrame = 0.f;
	_float					m_fSelectX = 0.f;
	_float					m_fSelectY = 0.f;
	ITEMINFO				m_SelectItem;
	_float					m_fTimeDegree = 0.f;
	_float					m_fAcc = 0.f;
	_float					m_fDate = 0.f;
	_float					m_fTime = 0.f;
	_bool					m_bNextDay = false;
	_float					m_fTimeSpeed = 0.f;
	_int					m_iGold = 0;
	_int					m_iGoldCheck = 0;
	_bool					m_bRainDay = false;
	_bool					m_bUIOn = true;
	_bool					m_bOnce = false;
	_bool					m_bOnce2 = false;
	_bool					m_bInstallCheck = false;
	_int					m_iDirtX;
	_int					m_iDirtY;
	_int					m_PlayerX;
	_int					m_PlayerY;
	_bool					m_bInstall = false;
private:				
	HRESULT SetUp_Resource();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();
	HRESULT Animation(_float fTimeDelta);
public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device/*, const MONSTERDESC* pMonsterDesc = nullptr*/);	
protected:
	virtual void Free(void);
};

