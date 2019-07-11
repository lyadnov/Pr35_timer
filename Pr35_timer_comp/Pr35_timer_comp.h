// Pr35_timer_comp.h : main header file for the PR35_TIMER_COMP application
//

#if !defined(AFX_PR35_TIMER_COMP_H__91839038_B8D9_458C_9FA8_C78E00F0B95B__INCLUDED_)
#define AFX_PR35_TIMER_COMP_H__91839038_B8D9_458C_9FA8_C78E00F0B95B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPr35_timer_compApp:
// See Pr35_timer_comp.cpp for the implementation of this class
//

class CPr35_timer_compApp : public CWinApp
{
public:
	CPr35_timer_compApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPr35_timer_compApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPr35_timer_compApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PR35_TIMER_COMP_H__91839038_B8D9_458C_9FA8_C78E00F0B95B__INCLUDED_)
