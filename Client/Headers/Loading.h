#pragma once

#include "Base.h"

class CResource_Manager;
class CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading() = default;
public:
	SCENEID Get_SceneID() const {
		return m_eSceneID; }
	LPCRITICAL_SECTION Get_Critical_Section() { 
		return &m_Critical_Section; }
	const _tchar* Get_String() const {
		return m_szString; }
	_bool Get_Finish() const {
		return m_isFinish; }
public:
	HRESULT Ready_Loading(SCENEID eSceneID);
	HRESULT Loading_Resource_Stage();
	static _uint APIENTRY Thread_Main(void* pArg);
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	CResource_Manager*			m_pResource_Manager = nullptr;
	SCENEID						m_eSceneID;
	HANDLE						m_hThread;	
	CRITICAL_SECTION			m_Critical_Section;
private:
	_tchar						m_szString[128];
	_bool						m_isFinish = false;
public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eSceneID);
protected:
	virtual void Free();
};

