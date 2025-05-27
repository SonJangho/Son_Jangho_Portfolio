#pragma once

#include "GameObject.h"
#include "Object_Manager.h"
#include "Resource_Manager.h"
#include "Render_Manager.h"

class CCamera abstract : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCamera() = default;
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject();
	virtual _int LastUpdate_GameObject();
	virtual void Render_GameObject();
protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
protected:
	CRender_Manager*		m_pRender_Manager = nullptr;
	CResource_Manager*		m_pResource_Manager = nullptr;
public:
	// ī�޶� ���� �̵��ؾ��ϴ� ��ü�� ��ġ�� �־��ָ� ���� �̵��ѰͰ� ���� ��ǥ���� �������ش�.
	POSITION Transform(const _float& fX, const _float& fY) const;
protected:
	POSITION		m_StartPos = { 0.0f };
	_float			m_fCamSpeed = 0.f;
protected:
	virtual void Free();

};

