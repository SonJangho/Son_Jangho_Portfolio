#include "stdafx.h"
#include "MainApp.h"


CMainApp::CMainApp(HWND hWnd, HDC hDC)
	: m_hWnd(hWnd)
	, m_hDC(hDC)
{

}
CMainApp::~CMainApp()
{

}
void CMainApp::Ready_MainApp() 
{

}

void CMainApp::Update_MainApp() 
{
	D3DXVECTOR3			vDir[2];

	vDir[0] = D3DXVECTOR3(0.f, 0.f, 1.f);
	vDir[1] = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXVECTOR3			vResult;
	D3DXVec3Cross(&vResult, &vDir[1], &vDir[0]);

	D3DXVec3Normalize(&vDir[0], &vDir[0]);
	D3DXVec3Normalize(&vDir[1], &vDir[1]);

	float fCos = D3DXVec3Dot(&vDir[0], &vDir[1]);

	float fAngle = D3DXToDegree(acosf(fCos));	

	int a = 10;
}


void CMainApp::Render_MainApp() 
{
	Rectangle(m_hDC, 0, 0, 800, 600);

	Ellipse(m_hDC, m_Info[0].vPosition.x - m_Info[0].fSize * 0.5f,
		m_Info[0].vPosition.y - m_Info[0].fSize * 0.5f,
		m_Info[0].vPosition.x + m_Info[0].fSize * 0.5f,
		m_Info[0].vPosition.y + m_Info[0].fSize * 0.5f);

	Rectangle(m_hDC, m_Info[1].vPosition.x - m_Info[1].fSize * 0.5f,
		m_Info[1].vPosition.y - m_Info[1].fSize * 0.5f,
		m_Info[1].vPosition.x + m_Info[1].fSize * 0.5f,
		m_Info[1].vPosition.y + m_Info[1].fSize * 0.5f);
}
