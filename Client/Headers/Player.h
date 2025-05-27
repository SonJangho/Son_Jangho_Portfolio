#pragma once

#include "GameObject.h"
#include "Sound_Manager.h"

class CPlayer final : public CGameObject
{
public:
	enum STATE { STATE_FRONT_IDLE, STATE_BACK_IDLE, STATE_SIDE_IDLE,
				STATE_FRONT_RUN, STATE_BACK_RUN, STATE_SIDE_RUN,
				STATE_FRONT_SWING, STATE_BACK_SWING, STATE_SIDE_SWING,
				STATE_FRONT_SLASH, STATE_BACK_SLASH, STATE_SIDE_SLASH,
				STATE_FRONT_WATERING, STATE_BACK_WATERING, STATE_SIDE_WATERING,
				STATE_FRONT_HIDLE, STATE_BACK_HIDLE, STATE_SIDE_HIDLE,
				STATE_FRONT_HRUN, STATE_BACK_HRUN, STATE_SIDE_HRUN,
				STATE_FRONT_HARVEST, STATE_BACK_HARVEST, STATE_SIDE_HARVEST,
				STATE_FRONT_FISHSWING, STATE_BACK_FISHSWING, STATE_SIDE_FISHSWING,
				STATE_FRONT_FISHING, STATE_BACK_FISHING, STATE_SIDE_FISHING,
				STATE_GET_FISH,
				STATE_END};
	enum LOCATION { BED, HOMEIN, HOMEOUT, FARMIN, TOWNIN, SHOPIN, SHOPOUT, ROADIN, ROADOUT, CAVEIN, CAVEOUT};
private:
	typedef struct tagState
	{
		STATE		eState;
		_uint		iNumTexture;
	}STATEDESC;
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPlayer() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(); 
	HRESULT Ready_Camera();
	HRESULT Ready_Location(_int iNumb);
public:
	_int Get_State() {
		if (STATE_BACK_SWING == m_eCurrentState || STATE_BACK_SLASH == m_eCurrentState || STATE_BACK_WATERING == m_eCurrentState || STATE_BACK_FISHSWING == m_eCurrentState || STATE_BACK_FISHING == m_eCurrentState)
			m_pPlayerLook = 1;
		else if ((STATE_SIDE_SWING == m_eCurrentState || STATE_SIDE_SLASH == m_eCurrentState || STATE_SIDE_WATERING == m_eCurrentState || STATE_SIDE_FISHSWING == m_eCurrentState || STATE_SIDE_FISHING == m_eCurrentState) && true == m_isRight)
			m_pPlayerLook = 2;
		else if ((STATE_SIDE_SWING == m_eCurrentState || STATE_SIDE_SLASH == m_eCurrentState || STATE_SIDE_WATERING == m_eCurrentState || STATE_SIDE_FISHSWING == m_eCurrentState || STATE_SIDE_FISHING == m_eCurrentState) && false == m_isRight)
			m_pPlayerLook = 4;
		else if (STATE_FRONT_SWING == m_eCurrentState || STATE_FRONT_SLASH == m_eCurrentState || STATE_FRONT_WATERING == m_eCurrentState || STATE_FRONT_FISHSWING == m_eCurrentState || STATE_FRONT_FISHING == m_eCurrentState)
			m_pPlayerLook = 3;
		else
			m_pPlayerLook = 0;

		return m_pPlayerLook;
	}
	_int Get_PlayerLook(){
		if (STATE_BACK_IDLE == m_eCurrentState|| STATE_BACK_RUN == m_eCurrentState|| STATE_BACK_HRUN == m_eCurrentState)
			m_iLook = 1;
		else if ((STATE_SIDE_IDLE == m_eCurrentState || STATE_SIDE_RUN == m_eCurrentState || STATE_SIDE_HRUN == m_eCurrentState) && true == m_isRight)
			m_iLook = 2;
		else if ((STATE_SIDE_IDLE == m_eCurrentState || STATE_SIDE_RUN == m_eCurrentState || STATE_SIDE_HRUN == m_eCurrentState) && false == m_isRight)
			m_iLook = 4;
		else if (STATE_FRONT_IDLE == m_eCurrentState || STATE_FRONT_RUN == m_eCurrentState || STATE_FRONT_HRUN == m_eCurrentState)
			m_iLook = 3;

		return m_iLook;
	}
	_float Get_Frame() {
		return m_fFrame;
	}
	void Set_FrontIdle() {
		m_bOnce = false;
		m_eCurrentState = STATE_FRONT_IDLE;
	}
	_int Get_FishNumber() {
		return m_iFishNumber;
	}
	void Set_StopOn() {
		m_bStop = true;
	}
	void Set_StopOff() {
		m_bStop = false;
	}
	void Set_Blink(_bool bBlink){
		CSound_Manager::Get_Instance()->SoundPlay(36, FALSE);
		m_bHurt = true;
		m_bBlink = bBlink;
	}
	_bool Get_Hurt(){
		return m_bHurt;
	}
	void Set_Hurt(){
		m_bHurt = false;
	}
private:
	STATEDESC*				m_pStateDesc = nullptr;
	STATE					m_eCurrentState = STATE(0);
	CTexture*				m_pTexture[STATE_END] = { nullptr };
	CTexture*				m_pIcon[20] = { nullptr };
	CTexture*				m_pRect = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	RECT					m_pBox;
private:
	_vec3					m_vTargetPos;
	_float					m_fFrame = 0.f;
	_bool					m_isRight = false;
	_bool					m_isClicked = false;
	_float					m_fSpeed = 0.f;
	CGameObject*			m_pCamera;
	CGameObject*			m_pUI = nullptr;
	CGameObject*			m_pFarming = nullptr;
	_int					m_pPlayerLook = 0;
	_int					m_iLook = 0;
	_bool					m_isClick = false;
	_int					i = 0;
	_bool					m_bFirst = false;
	_bool					m_bOnce = false;
	_int					m_iFishNumber = 0;
	_bool					m_bStop = false;
	_bool					m_bBlinkOn = false;
	_float					m_fBlinkAcc = 0.f;
	_bool					m_bBlink = false;
	_int					m_iBlinkCount = 0;
	_bool					m_bHurt = false;
private:
	HRESULT SetUp_StateDesc();
	HRESULT SetUp_Resource();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();
	HRESULT Animation(_float fTimeDelta);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};