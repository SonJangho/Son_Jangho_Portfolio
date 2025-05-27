#pragma once

#include "GameObject.h"

// �ΰ��� ����̴�.
class CTexture;
class CBuffer_RcTex;
class CBack_Menu final : public CGameObject
{
private:
	explicit CBack_Menu(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBack_Menu() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTexture*				m_pTexture = nullptr;
	CTexture*				m_pTexture1 = nullptr;
	CBuffer_RcTex*			m_pBuffer = nullptr;
	_float					m_fFrame = 0.f;
	CShader*				m_pShader = nullptr;
	_bool					m_bNew = false;
	_bool					m_bExit = false;
private:
	HRESULT SetUp_Resource(); // ���ҽ� �Ŵ������� ã�Ƽ� ����� �����س��´�.
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CBack_Menu* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

