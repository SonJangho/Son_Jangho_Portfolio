#pragma once

#include "GameObject.h"

// 로고의 배경이다.
class CShopUI final : public CGameObject
{
	enum STATE { BRANCH, STONE, WEED, BASS, SALMON, SUNFISH, END };
private:
	explicit CShopUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CShopUI() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	_bool Get_ShopOn() {
		return m_bShopOn;
	}
private:
	CTexture*				m_pShopUI = nullptr;
	CTexture*				m_pTexture[3] = { nullptr };
	CTexture*				m_pIcon[36] = { nullptr };
	CTexture*				m_pBigNum = nullptr;
	CTexture*				m_pSmallNum = nullptr;
	STATE					m_State;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	_float					m_fFrame = 0.f;
	CShader*				m_pShader = nullptr;
	D3DXMATRIX				TempMatrix;
	CGameObject*			m_pPlayer = nullptr;
	CGameObject*			m_pTarget = nullptr;
	CGameObject*			m_pInventory = nullptr;
	ITEMINFO*				pItemInfo;
	CTexture*				m_pRect = nullptr;
	_float					m_fNum = 0.f;
	_bool					m_bOnce = false;
	_bool					m_bOnce2 = false;
	_bool					m_bOnce3 = false;
	_bool					m_bOnce4 = false;
	_bool					m_bShopOn = false;
	RECT*					rcRect;
	_bool					m_bItemOn = false;
	_int					m_iSetItem = 0;
	ITEMINFO				TempItem;
	ITEMINFO				TempItem2;
	_float					m_fSlotX[36];
	_float					m_fSlotY[36];
	_bool					m_bBoldOn = false;
	_int					m_iNum = 0;
	_bool					m_bShopItemOn = false;
	_int					m_iItemNum = 0;
	_bool					m_bFirst = false;
	_float					m_fAcc = 0.f;
	_int					m_iGold = 0;
	_int					m_iGoldCheck = 0;
	_int					m_iCount = 0;
	_bool					m_bShopping = false;
	_bool					m_bTempOn = false;
private:					
	HRESULT SetUp_Resource(); // 리소스 매니져에서 찾아서 멤버에 대입해놓는다.
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
public:
	static CShopUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

