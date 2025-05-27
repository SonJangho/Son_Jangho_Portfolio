#pragma once

#include "GameObject.h"

class CSword final : public CGameObject
{
	enum STATE {
		STATE_FRONT, STATE_BACK, STATE_SIDE, STATE_ON, STATE_OFF, STATE_END
	};
private:
	explicit CSword(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CSword() = default;
public:
	virtual HRESULT Ready_GameObject();	
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	typedef struct tagState
	{
		STATE		eState;
		_uint		iNumTexture;
	}STATEDESC;
public:
	_bool	Get_ToolOn() {
		return m_isToolOn;
	}
private:
	STATEDESC*				m_pStateDesc = nullptr;
	CTexture*				m_pTexture[STATE_END] = { nullptr };
	STATE					m_eCurrentState = STATE(0);
	CBuffer_RcTex*			m_pBuffer = nullptr;
private:
	CGameObject*			m_pPlayer = nullptr;
	CGameObject*			m_pUI = nullptr;
	_float					m_fFrame = 0.f;
	_bool					m_isToolOn = false;
	_int					m_iCount = 0;
	_bool					m_isRight = false;
	_int					i = 0;
private:
	HRESULT SetUp_StateDesc();
	HRESULT SetUp_Resource();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();
	HRESULT Animation(_float fTimeDelta);
public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);	
protected:
	virtual void Free(void);
};

