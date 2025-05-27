#pragma once

#include "GameObject.h"

class CFishing final : public CGameObject
{
	enum STATE {BASE,FISH,CHECKPOINT,HIT,GAUGE,END};
private:
	explicit CFishing(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CFishing() = default;
public:
	virtual HRESULT Ready_GameObject();	
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	_bool Get_FishingOn() {
		return m_bFishing;
	}
	_bool Get_GetFish() {
		return m_bGetFish;
	}
private:
	CTexture*				m_pTexture[END] = { nullptr };
	CShader*				m_pShader = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
private:
	CGameObject*			m_pPlayer = nullptr;
	CGameObject*			m_pTool = nullptr;
	D3DXMATRIX				TempMatrix;
	_float					m_fFrame = 0.f;
	_float					m_fBar = 180.f;
	_float					m_fFish = 180.f;
	_float					m_fCount = 0.f;
	_int					iRanNum = 0;
	_int					iRanNumDown = 0;
	_int					iRanNumUp = 0;
	_float					m_fCheck = 30.f;
	_bool					m_bFishing = false;
	_bool					m_bHit = false;
	_float					m_fHitX = 0.f;
	_float					m_fHitY = 0.f;
	_bool					m_bGetFish = false;
	_float					m_fCnt = 0;
private:
	HRESULT SetUp_Resource();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();
	_matrix Render_TransForm(CGameObject* pCamera);
public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);	
protected:
	virtual void Free(void);
};

