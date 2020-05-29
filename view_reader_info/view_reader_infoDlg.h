
// view_reader_infoDlg.h : header file
//

#pragma once

#include <string>
#include <list>

// CviewreaderinfoDlg dialog
class CviewreaderinfoDlg : public CDialogEx
{
// Construction
public:
	CviewreaderinfoDlg(CWnd* pParent = nullptr);	// standard constructor

	void add_info(const CString & sInfo)
	{
		m_list_info.AddString(sInfo);
		m_list_info.SetCurSel(m_list_info.GetCount() - 1);
	}

	void add_info(const std::wstring & s_info)
	{
		m_list_info.AddString(s_info.c_str());
		m_list_info.SetCurSel(m_list_info.GetCount() - 1);
	}

	void clear_info()
	{
		m_list_info.ResetContent();
	}

	bool scan_scr( std::list<std::wstring> & list_out_reader);


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW_READER_INFO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list_info;
};
