#pragma once

#include "GameObject.h"

// 로고의 배경이다.
class CObject_Farm2 final : public CGameObject
{
	enum STATE {BigStump, Bush,Stone,Branch,BranchCrash, End};
private:
	explicit CObject_Farm2(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CObject_Farm2() = default;
public:
	virtual HRESULT Ready_GameObject(_vec3 vPosition, _vec3 vSize, _uint idx);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	_bool	Create_DropObject() {
		return m_bCreateObject;
	}
	_float	Get_Frame() {
		return m_fObjectNumb;
	}
	_float	Get_fX() {
		return m_matTransform._41;
	}
	_float	Get_fY() {
		return m_matTransform._42;
	}
	void Set_DropObject() {
		m_bCreateObject = false;
	}
	_bool Get_ChestOn() {
		return m_bChestOn;
	}
	void Set_ChestOn() {
		m_bChestOn = false;
	}
private:
	CTexture*				m_pTexture= nullptr;
	CTexture*				m_pDelete[End] = { nullptr };
	STATE					m_State;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	_float					m_fFrame = 0.f;
	CShader*				m_pShader = nullptr;
	D3DXMATRIX				TempMatrix;
	CGameObject*			m_pPlayer = nullptr;
	CGameObject*			m_pCamera = nullptr;
	CGameObject*			m_pAx = nullptr;
	CGameObject*			m_pPick = nullptr;
	CGameObject*			m_pSickle = nullptr;
	CTexture*				m_pRect = nullptr;
	_int					m_iX = 0;
	_int					m_iY = 0;
	_bool					m_bObjectOff = false;
	_float					m_fNumb = 0.f;
	_bool					m_bBigStump = false;
	_bool					m_bBush = false;
	_bool					m_bStone = false;
	_bool					m_bBranch = false;
	_float					m_fFrame1 = 0.f;
	_float					m_fFrame2 = 0.f;
	_float					m_fFalldown = 0.5f;
	_float					m_fFalldownX = 0.f;
	_float					m_fFalldownY = 0.f;
	_bool					m_bFalldown = false;
	_float					m_fNum = 1.f;
	_float					m_AccNum = 0.f;
	_int					m_iCount = 0;
	_bool					m_bFirst = false;
	_float					m_fSX = 0.f;
	_float					m_fSY = 0.f;
	_bool					m_bShake = false;
	_bool					m_bSequence = false;
	_int					m_iShakeTime = 4;
	_float					m_fSSX = 0.f;
	_float					m_fSSY = 0.f;
	_float					m_fShakePower = 3.f;
	_bool					m_bWhat = false;
	_bool					m_bWhat2 = false;
	_vec3					m_vPosition;
	_vec3					m_vSize;
	_bool					m_bTree = false;
	_bool					m_bReturn = false;
	_bool					m_bCreateObject = false;
	_float					m_fObjectNumb = 0.f;
	_bool					m_bbbOnce = false;
	_bool					m_bChestOn = false;
private:
	HRESULT SetUp_Resource(); // 리소스 매니져에서 찾아서 멤버에 대입해놓는다.
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool	isDraw(CGameObject* pCamera);
	_matrix Render_TransForm(CGameObject* pCamera);
	void Object_Shake();
	void Object_Translucent();
public:
	static CObject_Farm2* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3 vPosition, _vec3 vSize, _uint idx);
protected:
	virtual void Free();
};

