#pragma once

#include "Base.h"
#include "Graphic_Device.h"

class CManagement;
class CResource_Manager;
class CMainApp final : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT Ready_MainApp();
	_int Update_MainApp(const _float& fTimeDelta);
	void Render_MainApp();
private:
	CGraphic_Device*		m_pGraphic_Instance = nullptr;
	CManagement*			m_pManagement = nullptr;
	CResource_Manager*		m_pResource_Manager = nullptr;
private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
private: // For.FPS
	_tchar					m_szFPS[128] = L"";
	_uint					m_iRenderCnt = 0;
	_float					m_fTimeAcc = 0.f;
private:
	HRESULT Ready_Default_Setting(CGraphic_Device::WINMODE eMode, const _uint& iBackCX, const _uint& iBackCY);
	HRESULT Ready_Render_State(); // 
	HRESULT Ready_Resource_Static();
	HRESULT Ready_Start_Scene(SCENEID eID);
	
public:
	static CMainApp* Create();
protected:
	virtual void Free();	
};

