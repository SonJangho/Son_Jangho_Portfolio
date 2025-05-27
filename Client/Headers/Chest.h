#pragma once

#include "GameObject.h"

class CChest final : public CGameObject
{
private:
	explicit CChest(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CChest() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTexture*				m_pTexture = nullptr;
	CTexture*				m_pIcon[30] = { nullptr };
	CTexture*				m_pSmallNum = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	CGameObject*			m_pInventory = nullptr;
	ITEMINFO*				pItemInfo;
	ITEMINFO*				ChestItemInfo;
	RECT					rcRect[36];
	RECT					rcChestRect[36];
	_float					m_fSlotX[36] = { 0.f };
	_float					m_fSlotY[36] = { 0.f };
	_float					m_fSlotX2[36] = { 0.f };
	_float					m_fSlotY2[36] = { 0.f };
	_bool					m_bItemOn = false;
	_int					m_iSetItem = 0;
	_float					m_fAcc = 0.f;
	_bool					m_bFirst = false;
	_bool					m_bFirst2 = false;
	_bool					m_bChestUIOn = false;
	_bool					m_bOnce = false;
	_bool					m_bOnce2 = false;
	_bool					m_bRect1 = false;
	_bool					m_bRect2 = false;
	_bool					m_bRect3 = false;
	_bool					m_bChestOn = false;
	_bool					m_bOnce4 = false;
private:
	_float					m_fFrame = 0.f;
private:
	HRESULT SetUp_Resource();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();
	HRESULT Set_BasicItem();
public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free(void);
};

