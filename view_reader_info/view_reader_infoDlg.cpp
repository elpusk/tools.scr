
// view_reader_infoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "view_reader_info.h"
#include "view_reader_infoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <algorithm>
#include <winscard.h>
#include <tchar.h>
#pragma comment(lib, "winscard.lib")


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CviewreaderinfoDlg dialog



CviewreaderinfoDlg::CviewreaderinfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIEW_READER_INFO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CviewreaderinfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_list_info);
}

BEGIN_MESSAGE_MAP(CviewreaderinfoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CviewreaderinfoDlg message handlers

BOOL CviewreaderinfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	add_info(std::wstring(L"ready for running ok."));

	std::list<std::wstring> list_reader;

	if (scan_scr(list_reader)) {
		std::for_each(std::begin(list_reader), std::end(list_reader), [&](const std::wstring& s_reader) {
			add_info(s_reader);
		});
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CviewreaderinfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CviewreaderinfoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CviewreaderinfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CviewreaderinfoDlg::scan_scr(std::list<std::wstring>& list_out_reader)
{
	HRESULT           hr = S_OK;
	LPTSTR            szReaders(nullptr), szRdr(nullptr);
	DWORD             cchReaders = SCARD_AUTOALLOCATE;
	DWORD             dwI, dwRdrCount;
	SCARD_READERSTATE rgscState[MAXIMUM_SMARTCARD_READERS];
	SCARDCONTEXT      hSC;
	LONG              lReturn;

	bool b_result = false;
	bool b_need_release = false;
	bool b_need_free = false;

	do {
		list_out_reader.clear();

		// Establish a context.
		lReturn = SCardEstablishContext(SCARD_SCOPE_USER,
			NULL,
			NULL,
			&hSC);
		if (SCARD_S_SUCCESS != lReturn){
			continue;
		}
		b_need_release = true;

		// Determine which readers are available.
		lReturn = SCardListReaders(hSC,
			NULL,
			(LPTSTR)&szReaders,
			&cchReaders);
		if (SCARD_S_SUCCESS != lReturn){
			continue;
		}
		b_need_free = true;

		// Place the readers into the state array.
		szRdr = szReaders;
		for (dwI = 0; dwI < MAXIMUM_SMARTCARD_READERS; dwI++){
			if (0 == *szRdr)
				break;
			rgscState[dwI].szReader = szRdr;
			rgscState[dwI].dwCurrentState = SCARD_STATE_UNAWARE;

			list_out_reader.push_back(std::wstring(szRdr));
			szRdr += lstrlen(szRdr) + 1;
		}
		dwRdrCount = dwI;

		if (dwRdrCount == 0) {
			//printf("No readers available\n");
			continue;
		}

		//
		b_result = true;
	} while (false);

	// Release the context.
	if (b_need_release) {
		lReturn = SCardReleaseContext(hSC);
	}
	if (b_need_free) {
		SCardFreeMemory(hSC, szReaders);
	}
	return b_result;
}