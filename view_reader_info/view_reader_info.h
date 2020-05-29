
// view_reader_info.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CviewreaderinfoApp:
// See view_reader_info.cpp for the implementation of this class
//

class CviewreaderinfoApp : public CWinApp
{
public:
	CviewreaderinfoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CviewreaderinfoApp theApp;
