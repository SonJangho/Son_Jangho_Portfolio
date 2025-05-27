#pragma once

#include "GameObject.h"

class CInventory final : public CGameObject
{
private:
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CInventory() = default;
public:
	virtual HRESULT Ready_GameObject();	
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	ITEMINFO*	Get_Item() {
		return pItemInfo;
	}
	// 소비 아이템 사용
	void Use_Item(wchar_t const* Name, _int Count) {
		for (size_t i = 0; i < 36; i++)
		{
			if (0 == wcscmp(pItemInfo[i].szName, Name))
			{
				pItemInfo[i].iCount -= Count;
				if (0 >= pItemInfo[i].iCount)
				{
					wcscpy(pItemInfo[i].szName, L"NULL");
					pItemInfo[i].iCount = 0;
					pItemInfo[i].isHandle = false;
				}
			}
		}
	}
	// 아이템 추가
	void Add_Item(wchar_t const* Name, _int Count, _int ItemNum) {
		// 같은 아이템이 있을때
		for (size_t i = 0; i < 36; i++)
		{
			if (0 == wcscmp(pItemInfo[i].szName, Name))
			{
				pItemInfo[i].iCount += Count;
				return;
			}
		}

		// 같은 아이템이 없을때
		for (size_t i = 0; i < 36; i++)
		{
			if (0 == wcscmp(pItemInfo[i].szName, L"NULL"))
			{
				wcscpy(pItemInfo[i].szName,Name);
				pItemInfo[i].iCount = Count;
				pItemInfo[i].iNum = ItemNum;
				pItemInfo[i].isHandle = true;
				break;
			}
		}
	}
	void Buy_Item(wchar_t const* Name, _int Count, _int ItemNum, _int Idx) {
		wcscpy(pItemInfo[Idx].szName, Name);
		pItemInfo[Idx].iCount = Count;
		pItemInfo[Idx].iNum = ItemNum;
		pItemInfo[Idx].isHandle = true;
	}
	_bool Get_InvenOn() {
		return m_bInventoryOn;
	}
private:
	CTexture*				m_pTexture = nullptr;
	CTexture*				m_pTex[10] = { nullptr };
	CTexture*				m_pIcon[25] = { nullptr };
	CTexture*				m_pSmallNum = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	ITEMINFO*				pItemInfo;
	ITEMINFO				TempItem;
	RECT*					rcRect;
	RECT*					rcMakeRect;
	_float					m_fSlotX[36] = { 0.f };
	_float					m_fSlotY[36] = { 0.f };
	_float					m_fSlotX2[36] = { 0.f };
	_float					m_fSlotY2[36] = { 0.f };
	_bool					m_bItemOn = false;
	_int					m_iSetItem = 0;
	_float					m_fAcc = 0.f;
	_bool					m_bFirst = false;
	_bool					m_bInventoryOn = false;
	_bool					m_bOnce = false;
	_bool					m_bOnce2 = false;
	_bool					m_bRect1 = false;
	_bool					m_bRect2 = false;
	_bool					m_bRect3 = false;
	_bool					m_bChestOn = false;
	_bool					m_bOnce4 = false;
	_bool					m_bOK = false;
	_bool					m_bOK2 = false;
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

