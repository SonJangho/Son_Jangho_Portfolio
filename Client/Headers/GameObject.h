#pragma once

#include "Base.h"
#include "Object_Manager.h"
#include "Resource_Manager.h"
#include "Render_Manager.h"

// 게임내에서 활용될 객체들의 부모클래스다.
class CCamera;
class CGameObject abstract : public CBase
{
public:
	enum STATE {STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION};
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CGameObject() = default;
public:
	const _vec3* Get_Infomation(STATE eState) {
		return (_vec3*)&m_matTransform.m[eState][0];
	}
	_float Get_Scale(STATE eState) {
		return D3DXVec3Length((_vec3*)&m_matTransform.m[eState][0]);
	}
	_float Get_X() const {
		return m_fX;
	}
	_float Get_Y() const {
		return m_fY;
	}
	_float Get_X2() const {
		return m_matTransform._41;
	}
	_float Get_Y2() const {
		return m_matTransform._42;
	}
	_float Get_CompareY() const {
		return m_matTransform._42 + m_matTransform._32 * 0.5f;
	}
	void Push_TargetRight() {
		m_fX += 6;
	}
	void Push_TargetLeft() {
		m_fX -= 6;
	}
	void Push_TargetUp() {
		m_fY -= 6;
	}
	void Push_TargetDown() {
		m_fY += 6;
	}
	void Push_TargetRight2(_int iNum) {
		m_matTransform._41 += iNum;
	}
	void Push_TargetLeft2(_int iNum) {
		m_matTransform._41 -= iNum;
	}
	void Push_TargetUp2(_int iNum) {
		m_matTransform._42 -= iNum;
	}
	void Push_TargetDown2(_int iNum) {
		m_matTransform._42 += iNum;
	}
	LONG	Get_BottomRect() {
		return m_YsortRect.bottom;
	}
	void Set_Position(_vec3 vPoint);
	void Set_Scale(_vec3 vScale);
	void Set_Right2D(_vec3 vDir); // 내 진행방향(Right)이 임의의 방향벡터벡터를 가지게 만들어준다.
	void Set_LookAt2D(_vec3 vPoint); // 내가 특정 지점을 바라보게 만들어준다.	
	void Set_RotationZ(const _float& fRadian); // 내가 넣어준 각도를 바라보고 회전한다.
	void Set_RotationY(const _float& fRadian);
	void RotationZ(const _float& fRadian);
	RECT Get_Rect() const;
	RECT Get_Rect(_float fX, _float fY, _float m_fSizeX, _float m_fSizeY);
	_bool isIn_Rect(const RECT& rcRect);
public:
	void Move_ToTarget(_vec3 vTargetPos, const _float& fTimeDelta);
	void Faraway_ToTarget(_vec3 vTargetPos, const _float& fTimeDelta);
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
	virtual _int Get_State();
	virtual HRESULT Ready_Camera();
	virtual HRESULT Ready_Location(_int iNumb);
	virtual ITEMINFO* Get_Item();
	virtual ITEMINFO Get_ItemOn();
	virtual _bool Get_ToolOn();
	virtual _bool Get_DirtCheck();
	virtual _bool Get_HarvestCheck();
	virtual _bool Get_HarvestOn();
	virtual _int Get_Crop();
	virtual void Add_Item(wchar_t const* Name, _int Count, _int ItemNum);
	virtual void Use_Item(wchar_t const* Name, _int Count);
	virtual _bool Get_FishingOn();
	virtual _float Get_Frame();
	virtual void Set_FishingOff();
	virtual _bool Get_GetFish();
	virtual void Set_FrontIdle();
	virtual _int Get_FishNumber();
	virtual _int Get_PlayerLook();
	virtual _float Get_TimeDegree();
	virtual _bool Get_NextDay();
	virtual void Set_NextDay();
	virtual _bool Create_DropObject();
	virtual _float Get_fX();
	virtual _float Get_fY();
	virtual void Set_DropObject();
	virtual void Set_Object(_int iFrame);
	virtual void Set_Start();
	virtual _bool Get_Start();
	virtual void Set_StopOn();
	virtual void Set_StopOff();
	virtual _bool Get_ShopOn();
	virtual _int Get_Gold();
	virtual void Set_Gold(_int Gold);
	virtual void Buy_Item(wchar_t const* Name, _int Count, _int ItemNum, _int Idx);
	virtual _bool Get_RainDay();
	virtual void Set_Collision(_bool bColl);
	virtual void Set_Blink(_bool bBlink);
	virtual _bool Get_Hurt();
	virtual void Set_Hurt();
	virtual _bool Get_InvenOn();
	virtual _bool Get_Install();
	virtual void Set_Install();
	virtual _int Get_DirtX();
	virtual _int Get_DirtY();
	virtual _bool Get_ChestOn();
	virtual void Set_ChestOn();
	virtual void Set_UI(_bool UI);
protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
protected:
	CRender_Manager*		m_pRender_Manager = nullptr;
	CResource_Manager*		m_pResource_Manager = nullptr;
	D3DXMATRIX				m_matTransform;
	_bool					m_isMove = false;
	_float					m_fSpeedPerSec;
protected:
	_float					m_fX = 0.f, m_fY = 0.f, m_fSizeX = 0.f, m_fSizeY = 0.f;
	D3DXVECTOR3				m_vecPos;
	RECT					m_rcCollider;
	RECT					m_YsortRect;
protected:
	HRESULT Update_Collider(_float fX, _float fY, _float fSizeX, _float fSizeY);
protected:
	virtual void Free();
};

