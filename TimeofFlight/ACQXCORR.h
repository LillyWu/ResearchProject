// ACQXCORR.h : main header file for the ACQXCORR application
//

#if !defined(AFX_ACQXCORR_H__45FDC755_BF06_48F8_A19B_07AFC9BE0A18__INCLUDED_)
#define AFX_ACQXCORR_H__45FDC755_BF06_48F8_A19B_07AFC9BE0A18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CACQXCORRApp:
// See ACQXCORR.cpp for the implementation of this class
//

class CACQXCORRApp : public CWinApp
{
public:
	CACQXCORRApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACQXCORRApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CACQXCORRApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACQXCORR_H__45FDC755_BF06_48F8_A19B_07AFC9BE0A18__INCLUDED_)
