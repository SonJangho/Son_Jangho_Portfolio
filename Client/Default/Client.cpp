// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"

#include "MainApp.h"
#include "Graphic_Device.h"
#include "Timer_Manager.h"
#include "Frame_Manager.h"
#include "Management.h"
#include "Render_Manager.h"
#include "Object_Manager.h"
#include "Resource_Manager.h"
#include "Font_Manager.h"
#include "Sound_Manager.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND	g_hWnd;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
_ulong				Release_Singleton();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
	}

	// Timer를 생성하자.
	CTimer_Manager* pTimer_Manager = CTimer_Manager::GetInstance();
	pTimer_Manager->AddRef();

	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_Default")))
		return FALSE;

	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_60")))
		return FALSE;

	// Frame을 생성한다.
	CFrame_Manager*	pFrame_Manager = CFrame_Manager::GetInstance();
	pFrame_Manager->AddRef();

	if (FAILED(pFrame_Manager->Add_Frame(L"Frame_60", 60)))
		return FALSE;

	CMainApp*		pMainApp = CMainApp::Create();
	if (nullptr == pMainApp)
		return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if (nullptr == pMainApp)
			break;

		_float fTimeDelta_Default = pTimer_Manager->Compute_Timer(L"Timer_Default");

		if (true == pFrame_Manager->Permit_Call(L"Frame_60", fTimeDelta_Default))
		{
			_float fTimeDelta_60 = pTimer_Manager->Compute_Timer(L"Timer_60");

			pMainApp->Update_MainApp(fTimeDelta_60);
			pMainApp->Render_MainApp();
		}		
	} 


	Safe_Release(pMainApp);

	Safe_Release(pTimer_Manager);
	Safe_Release(pFrame_Manager);

	Release_Singleton();

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = /*MAKEINTRESOURCEW(IDC_CLIENT)*/NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	RECT	rcWindow = { 0, 0, g_iBackCX, g_iBackCY };

	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      80, 10, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   g_hWnd = hWnd;

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

_ulong Release_Singleton()
{
	_ulong		dwRefCnt = 0;

	if (dwRefCnt = CManagement::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("CManagement Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CObject_Manager::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("CObject_Manager Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CRender_Manager::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("CRender_Manager Release Failed");
		return dwRefCnt;
	}

	CSound_Manager::Get_Instance()->Destroy_Instance();

	if (dwRefCnt = CResource_Manager::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("CResource_Manager Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CFont_Manager::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("CFont_Manager Release Failed");
		return dwRefCnt;
	}
	

	if (dwRefCnt = CGraphic_Device::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("CGraphic_Device Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CFrame_Manager::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("CFrame_Manager Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CTimer_Manager::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("CTimer_Manager Release Failed");
		return dwRefCnt;
	}
		

	return dwRefCnt;
}
