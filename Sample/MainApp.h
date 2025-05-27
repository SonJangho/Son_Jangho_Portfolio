#pragma once

class CMainApp
{
private:
	typedef struct tagInfo
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vDirection;
		float			fSpeed;
		float			fSize;
	}INFO;
public:
	CMainApp(HWND hWnd, HDC hDC);
	~CMainApp();
public:
	void Ready_MainApp();
	void Update_MainApp();
	void Render_MainApp();
private:
	HWND				m_hWnd;
	HDC					m_hDC;
	INFO				m_Info[2];
};