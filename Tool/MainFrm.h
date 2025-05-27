
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

#include "Defines.h"

class CToolView;
class CForm;
class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:
	RECT Get_ClientRect_FormView();
	_int Get_TileIndex();
	_int Get_ObjectIndex();
public:
	HRESULT Save_TileInfo();
	HRESULT Load_TileInfo();
	HRESULT Save_ObjectInfo();
	HRESULT Load_ObjectInfo();
// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual ~CMainFrame();



#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	HRESULT Ready_Tiles(const _uint& iNumCntX, const _uint& iNumCntY);
	HRESULT Ready_Object();
protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
private: // â������ �������ִ� ��ü�� �����Ѵ�.
	CSplitterWnd	  m_MainSplitter;

	CToolView*			m_pMainView = nullptr;
	CForm*				m_pForm = nullptr;



// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnDestroy();
};


