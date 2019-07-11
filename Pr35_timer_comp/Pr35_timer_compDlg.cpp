// Pr35_timer_compDlg.cpp : implementation file
//

#include "stdafx.h"
#include <time.h>
#include "Pr35_timer_comp.h"
#include "Pr35_timer_compDlg.h"
#include "source\comm_bulk.h"
#include "source\comm.h"
#include "source\errors.h"
#include "source\timer.h"
#include "source\config.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


unsigned char load_flag=0;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPr35_timer_compDlg dialog

CPr35_timer_compDlg::CPr35_timer_compDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPr35_timer_compDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPr35_timer_compDlg)
	m_Edit_Receive = _T("");
	m_combo_comnum = _T("COM1");
	m_edit_1_sec = _T("");
	m_edit_1_min = _T("");
	m_edit_1_mks = _T("");
	m_edit_1_ms = _T("");
	m_edit_1_ns = _T("");
	m_check_1 = FALSE;
	m_edit_2_min = _T("");
	m_edit_3_min = _T("");
	m_edit_4_min = _T("");
	m_edit_5_min = _T("");
	m_edit_6_min = _T("");
	m_edit_7_min = _T("");
	m_edit_8_min = _T("");
	m_edit_9_min = _T("");
	m_edit_10_min = _T("");
	m_edit_11_min = _T("");
	m_edit_12_min = _T("");
	m_edit_13_min = _T("");
	m_edit_14_min = _T("");
	m_edit_15_min = _T("");
	m_edit_16_min = _T("");
	m_edit_2_mks = _T("");
	m_edit_3_mks = _T("");
	m_edit_4_mks = _T("");
	m_edit_5_mks = _T("");
	m_edit_6_mks = _T("");
	m_edit_7_mks = _T("");
	m_edit_8_mks = _T("");
	m_edit_9_mks = _T("");
	m_edit_10_mks = _T("");
	m_edit_11_mks = _T("");
	m_edit_12_mks = _T("");
	m_edit_13_mks = _T("");
	m_edit_14_mks = _T("");
	m_edit_15_mks = _T("");
	m_edit_16_mks = _T("");
	m_edit_2_ms = _T("");
	m_edit_2_ns = _T("");
	m_edit_2_sec = _T("");
	m_check_2 = FALSE;
	m_check_10 = FALSE;
	m_check_11 = FALSE;
	m_check_12 = FALSE;
	m_check_13 = FALSE;
	m_check_14 = FALSE;
	m_check_15 = FALSE;
	m_check_16 = FALSE;
	m_check_3 = FALSE;
	m_check_4 = FALSE;
	m_check_5 = FALSE;
	m_check_6 = FALSE;
	m_check_7 = FALSE;
	m_check_8 = FALSE;
	m_check_9 = FALSE;
	m_edit_10_ms = _T("");
	m_edit_10_ns = _T("");
	m_edit_10_sec = _T("");
	m_edit_11_ms = _T("");
	m_edit_11_ns = _T("");
	m_edit_11_sec = _T("");
	m_edit_12_ms = _T("");
	m_edit_12_ns = _T("");
	m_edit_12_sec = _T("");
	m_edit_13_ms = _T("");
	m_edit_13_ns = _T("");
	m_edit_13_sec = _T("");
	m_edit_14_ms = _T("");
	m_edit_14_ns = _T("");
	m_edit_14_sec = _T("");
	m_edit_15_ms = _T("");
	m_edit_15_ns = _T("");
	m_edit_15_sec = _T("");
	m_edit_16_ms = _T("");
	m_edit_16_ns = _T("");
	m_edit_16_sec = _T("");
	m_edit_3_ms = _T("");
	m_edit_3_ns = _T("");
	m_edit_3_sec = _T("");
	m_edit_4_ms = _T("");
	m_edit_4_ns = _T("");
	m_edit_4_sec = _T("");
	m_edit_5_ms = _T("");
	m_edit_5_ns = _T("");
	m_edit_5_sec = _T("");
	m_edit_6_ms = _T("");
	m_edit_6_ns = _T("");
	m_edit_6_sec = _T("");
	m_edit_7_ms = _T("");
	m_edit_7_ns = _T("");
	m_edit_7_sec = _T("");
	m_edit_8_ms = _T("");
	m_edit_8_ns = _T("");
	m_edit_8_sec = _T("");
	m_edit_9_ms = _T("");
	m_edit_9_ns = _T("");
	m_edit_9_sec = _T("");
	m_static_state = _T("��������� �� ���������");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPr35_timer_compDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPr35_timer_compDlg)
	DDX_Control(pDX, IDC_CHECK_9, m_check_9_control);
	DDX_Control(pDX, IDC_CHECK_8, m_check_8_control);
	DDX_Control(pDX, IDC_CHECK_7, m_check_7_control);
	DDX_Control(pDX, IDC_CHECK_6, m_check_6_control);
	DDX_Control(pDX, IDC_CHECK_5, m_check_5_control);
	DDX_Control(pDX, IDC_CHECK_4, m_check_4_control);
	DDX_Control(pDX, IDC_CHECK_3, m_check_3_control);
	DDX_Control(pDX, IDC_CHECK_16, m_check_16_control);
	DDX_Control(pDX, IDC_CHECK_15, m_check_15_control);
	DDX_Control(pDX, IDC_CHECK_14, m_check_14_control);
	DDX_Control(pDX, IDC_CHECK_13, m_check_13_control);
	DDX_Control(pDX, IDC_CHECK_12, m_check_12_control);
	DDX_Control(pDX, IDC_CHECK_11, m_check_11_control);
	DDX_Control(pDX, IDC_CHECK_10, m_check_10_control);
	DDX_Control(pDX, IDC_CHECK_2, m_check_2_control);
	DDX_Control(pDX, IDC_CHECK_1, m_check_1_control);
	DDX_CBString(pDX, IDC_COMBO_COMNUM, m_combo_comnum);
	DDX_Text(pDX, IDC_EDIT_1_SEC, m_edit_1_sec);
	DDV_MaxChars(pDX, m_edit_1_sec, 2);
	DDX_Text(pDX, IDC_EDIT_1_MIN, m_edit_1_min);
	DDV_MaxChars(pDX, m_edit_1_min, 2);
	DDX_Text(pDX, IDC_EDIT_1_MKS, m_edit_1_mks);
	DDV_MaxChars(pDX, m_edit_1_mks, 3);
	DDX_Text(pDX, IDC_EDIT_1_MS, m_edit_1_ms);
	DDV_MaxChars(pDX, m_edit_1_ms, 3);
	DDX_Text(pDX, IDC_EDIT_1_NS, m_edit_1_ns);
	DDV_MaxChars(pDX, m_edit_1_ns, 3);
	DDX_Check(pDX, IDC_CHECK_1, m_check_1);
	DDX_Text(pDX, IDC_EDIT_2_MIN, m_edit_2_min);
	DDV_MaxChars(pDX, m_edit_2_min, 2);
	DDX_Text(pDX, IDC_EDIT_3_MIN, m_edit_3_min);
	DDV_MaxChars(pDX, m_edit_3_min, 2);
	DDX_Text(pDX, IDC_EDIT_4_MIN, m_edit_4_min);
	DDV_MaxChars(pDX, m_edit_4_min, 2);
	DDX_Text(pDX, IDC_EDIT_5_MIN, m_edit_5_min);
	DDV_MaxChars(pDX, m_edit_5_min, 2);
	DDX_Text(pDX, IDC_EDIT_6_MIN, m_edit_6_min);
	DDV_MaxChars(pDX, m_edit_6_min, 2);
	DDX_Text(pDX, IDC_EDIT_7_MIN, m_edit_7_min);
	DDV_MaxChars(pDX, m_edit_7_min, 2);
	DDX_Text(pDX, IDC_EDIT_8_MIN, m_edit_8_min);
	DDV_MaxChars(pDX, m_edit_8_min, 2);
	DDX_Text(pDX, IDC_EDIT_9_MIN, m_edit_9_min);
	DDV_MaxChars(pDX, m_edit_9_min, 2);
	DDX_Text(pDX, IDC_EDIT_10_MIN, m_edit_10_min);
	DDV_MaxChars(pDX, m_edit_10_min, 2);
	DDX_Text(pDX, IDC_EDIT_11_MIN, m_edit_11_min);
	DDV_MaxChars(pDX, m_edit_11_min, 2);
	DDX_Text(pDX, IDC_EDIT_12_MIN, m_edit_12_min);
	DDV_MaxChars(pDX, m_edit_12_min, 2);
	DDX_Text(pDX, IDC_EDIT_13_MIN, m_edit_13_min);
	DDV_MaxChars(pDX, m_edit_13_min, 2);
	DDX_Text(pDX, IDC_EDIT_14_MIN, m_edit_14_min);
	DDV_MaxChars(pDX, m_edit_14_min, 2);
	DDX_Text(pDX, IDC_EDIT_15_MIN, m_edit_15_min);
	DDV_MaxChars(pDX, m_edit_15_min, 2);
	DDX_Text(pDX, IDC_EDIT_16_MIN, m_edit_16_min);
	DDV_MaxChars(pDX, m_edit_16_min, 2);
	DDX_Text(pDX, IDC_EDIT_2_MKS, m_edit_2_mks);
	DDV_MaxChars(pDX, m_edit_2_mks, 3);
	DDX_Text(pDX, IDC_EDIT_3_MKS, m_edit_3_mks);
	DDV_MaxChars(pDX, m_edit_3_mks, 3);
	DDX_Text(pDX, IDC_EDIT_4_MKS, m_edit_4_mks);
	DDV_MaxChars(pDX, m_edit_4_mks, 3);
	DDX_Text(pDX, IDC_EDIT_5_MKS, m_edit_5_mks);
	DDV_MaxChars(pDX, m_edit_5_mks, 3);
	DDX_Text(pDX, IDC_EDIT_6_MKS, m_edit_6_mks);
	DDV_MaxChars(pDX, m_edit_6_mks, 3);
	DDX_Text(pDX, IDC_EDIT_7_MKS, m_edit_7_mks);
	DDV_MaxChars(pDX, m_edit_7_mks, 3);
	DDX_Text(pDX, IDC_EDIT_8_MKS, m_edit_8_mks);
	DDV_MaxChars(pDX, m_edit_8_mks, 3);
	DDX_Text(pDX, IDC_EDIT_9_MKS, m_edit_9_mks);
	DDV_MaxChars(pDX, m_edit_9_mks, 3);
	DDX_Text(pDX, IDC_EDIT_10_MKS, m_edit_10_mks);
	DDV_MaxChars(pDX, m_edit_10_mks, 3);
	DDX_Text(pDX, IDC_EDIT_11_MKS, m_edit_11_mks);
	DDV_MaxChars(pDX, m_edit_11_mks, 3);
	DDX_Text(pDX, IDC_EDIT_12_MKS, m_edit_12_mks);
	DDV_MaxChars(pDX, m_edit_12_mks, 3);
	DDX_Text(pDX, IDC_EDIT_13_MKS, m_edit_13_mks);
	DDV_MaxChars(pDX, m_edit_13_mks, 3);
	DDX_Text(pDX, IDC_EDIT_14_MKS, m_edit_14_mks);
	DDV_MaxChars(pDX, m_edit_14_mks, 3);
	DDX_Text(pDX, IDC_EDIT_15_MKS, m_edit_15_mks);
	DDV_MaxChars(pDX, m_edit_15_mks, 3);
	DDX_Text(pDX, IDC_EDIT_16_MKS, m_edit_16_mks);
	DDV_MaxChars(pDX, m_edit_16_mks, 3);
	DDX_Text(pDX, IDC_EDIT_2_MS, m_edit_2_ms);
	DDV_MaxChars(pDX, m_edit_2_ms, 3);
	DDX_Text(pDX, IDC_EDIT_2_NS, m_edit_2_ns);
	DDV_MaxChars(pDX, m_edit_2_ns, 3);
	DDX_Text(pDX, IDC_EDIT_2_SEC, m_edit_2_sec);
	DDV_MaxChars(pDX, m_edit_2_sec, 2);
	DDX_Check(pDX, IDC_CHECK_2, m_check_2);
	DDX_Check(pDX, IDC_CHECK_10, m_check_10);
	DDX_Check(pDX, IDC_CHECK_11, m_check_11);
	DDX_Check(pDX, IDC_CHECK_12, m_check_12);
	DDX_Check(pDX, IDC_CHECK_13, m_check_13);
	DDX_Check(pDX, IDC_CHECK_14, m_check_14);
	DDX_Check(pDX, IDC_CHECK_15, m_check_15);
	DDX_Check(pDX, IDC_CHECK_16, m_check_16);
	DDX_Check(pDX, IDC_CHECK_3, m_check_3);
	DDX_Check(pDX, IDC_CHECK_4, m_check_4);
	DDX_Check(pDX, IDC_CHECK_5, m_check_5);
	DDX_Check(pDX, IDC_CHECK_6, m_check_6);
	DDX_Check(pDX, IDC_CHECK_7, m_check_7);
	DDX_Check(pDX, IDC_CHECK_8, m_check_8);
	DDX_Check(pDX, IDC_CHECK_9, m_check_9);
	DDX_Text(pDX, IDC_EDIT_10_MS, m_edit_10_ms);
	DDV_MaxChars(pDX, m_edit_10_ms, 3);
	DDX_Text(pDX, IDC_EDIT_10_NS, m_edit_10_ns);
	DDV_MaxChars(pDX, m_edit_10_ns, 3);
	DDX_Text(pDX, IDC_EDIT_10_SEC, m_edit_10_sec);
	DDV_MaxChars(pDX, m_edit_10_sec, 2);
	DDX_Text(pDX, IDC_EDIT_11_MS, m_edit_11_ms);
	DDV_MaxChars(pDX, m_edit_11_ms, 3);
	DDX_Text(pDX, IDC_EDIT_11_NS, m_edit_11_ns);
	DDV_MaxChars(pDX, m_edit_11_ns, 3);
	DDX_Text(pDX, IDC_EDIT_11_SEC, m_edit_11_sec);
	DDV_MaxChars(pDX, m_edit_11_sec, 2);
	DDX_Text(pDX, IDC_EDIT_12_MS, m_edit_12_ms);
	DDV_MaxChars(pDX, m_edit_12_ms, 3);
	DDX_Text(pDX, IDC_EDIT_12_NS, m_edit_12_ns);
	DDV_MaxChars(pDX, m_edit_12_ns, 3);
	DDX_Text(pDX, IDC_EDIT_12_SEC, m_edit_12_sec);
	DDV_MaxChars(pDX, m_edit_12_sec, 2);
	DDX_Text(pDX, IDC_EDIT_13_MS, m_edit_13_ms);
	DDV_MaxChars(pDX, m_edit_13_ms, 3);
	DDX_Text(pDX, IDC_EDIT_13_NS, m_edit_13_ns);
	DDV_MaxChars(pDX, m_edit_13_ns, 3);
	DDX_Text(pDX, IDC_EDIT_13_SEC, m_edit_13_sec);
	DDV_MaxChars(pDX, m_edit_13_sec, 2);
	DDX_Text(pDX, IDC_EDIT_14_MS, m_edit_14_ms);
	DDV_MaxChars(pDX, m_edit_14_ms, 3);
	DDX_Text(pDX, IDC_EDIT_14_NS, m_edit_14_ns);
	DDV_MaxChars(pDX, m_edit_14_ns, 3);
	DDX_Text(pDX, IDC_EDIT_14_SEC, m_edit_14_sec);
	DDV_MaxChars(pDX, m_edit_14_sec, 2);
	DDX_Text(pDX, IDC_EDIT_15_MS, m_edit_15_ms);
	DDV_MaxChars(pDX, m_edit_15_ms, 3);
	DDX_Text(pDX, IDC_EDIT_15_NS, m_edit_15_ns);
	DDV_MaxChars(pDX, m_edit_15_ns, 3);
	DDX_Text(pDX, IDC_EDIT_15_SEC, m_edit_15_sec);
	DDV_MaxChars(pDX, m_edit_15_sec, 2);
	DDX_Text(pDX, IDC_EDIT_16_MS, m_edit_16_ms);
	DDV_MaxChars(pDX, m_edit_16_ms, 3);
	DDX_Text(pDX, IDC_EDIT_16_NS, m_edit_16_ns);
	DDV_MaxChars(pDX, m_edit_16_ns, 3);
	DDX_Text(pDX, IDC_EDIT_16_SEC, m_edit_16_sec);
	DDV_MaxChars(pDX, m_edit_16_sec, 2);
	DDX_Text(pDX, IDC_EDIT_3_MS, m_edit_3_ms);
	DDV_MaxChars(pDX, m_edit_3_ms, 3);
	DDX_Text(pDX, IDC_EDIT_3_NS, m_edit_3_ns);
	DDV_MaxChars(pDX, m_edit_3_ns, 3);
	DDX_Text(pDX, IDC_EDIT_3_SEC, m_edit_3_sec);
	DDV_MaxChars(pDX, m_edit_3_sec, 2);
	DDX_Text(pDX, IDC_EDIT_4_MS, m_edit_4_ms);
	DDV_MaxChars(pDX, m_edit_4_ms, 3);
	DDX_Text(pDX, IDC_EDIT_4_NS, m_edit_4_ns);
	DDV_MaxChars(pDX, m_edit_4_ns, 3);
	DDX_Text(pDX, IDC_EDIT_4_SEC, m_edit_4_sec);
	DDV_MaxChars(pDX, m_edit_4_sec, 2);
	DDX_Text(pDX, IDC_EDIT_5_MS, m_edit_5_ms);
	DDV_MaxChars(pDX, m_edit_5_ms, 3);
	DDX_Text(pDX, IDC_EDIT_5_NS, m_edit_5_ns);
	DDV_MaxChars(pDX, m_edit_5_ns, 3);
	DDX_Text(pDX, IDC_EDIT_5_SEC, m_edit_5_sec);
	DDV_MaxChars(pDX, m_edit_5_sec, 2);
	DDX_Text(pDX, IDC_EDIT_6_MS, m_edit_6_ms);
	DDV_MaxChars(pDX, m_edit_6_ms, 3);
	DDX_Text(pDX, IDC_EDIT_6_NS, m_edit_6_ns);
	DDV_MaxChars(pDX, m_edit_6_ns, 3);
	DDX_Text(pDX, IDC_EDIT_6_SEC, m_edit_6_sec);
	DDV_MaxChars(pDX, m_edit_6_sec, 2);
	DDX_Text(pDX, IDC_EDIT_7_MS, m_edit_7_ms);
	DDV_MaxChars(pDX, m_edit_7_ms, 3);
	DDX_Text(pDX, IDC_EDIT_7_NS, m_edit_7_ns);
	DDV_MaxChars(pDX, m_edit_7_ns, 3);
	DDX_Text(pDX, IDC_EDIT_7_SEC, m_edit_7_sec);
	DDV_MaxChars(pDX, m_edit_7_sec, 2);
	DDX_Text(pDX, IDC_EDIT_8_MS, m_edit_8_ms);
	DDV_MaxChars(pDX, m_edit_8_ms, 3);
	DDX_Text(pDX, IDC_EDIT_8_NS, m_edit_8_ns);
	DDV_MaxChars(pDX, m_edit_8_ns, 3);
	DDX_Text(pDX, IDC_EDIT_8_SEC, m_edit_8_sec);
	DDV_MaxChars(pDX, m_edit_8_sec, 2);
	DDX_Text(pDX, IDC_EDIT_9_MS, m_edit_9_ms);
	DDV_MaxChars(pDX, m_edit_9_ms, 3);
	DDX_Text(pDX, IDC_EDIT_9_NS, m_edit_9_ns);
	DDV_MaxChars(pDX, m_edit_9_ns, 3);
	DDX_Text(pDX, IDC_EDIT_9_SEC, m_edit_9_sec);
	DDV_MaxChars(pDX, m_edit_9_sec, 2);
	DDX_Text(pDX, IDC_STATIC_STATE, m_static_state);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPr35_timer_compDlg, CDialog)
	//{{AFX_MSG_MAP(CPr35_timer_compDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_CBN_KILLFOCUS(IDC_COMBO_COMNUM, OnKillfocusComboComnum)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_TESTSTART, OnButtonTestStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPr35_timer_compDlg message handlers

BOOL CPr35_timer_compDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	main_init();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPr35_timer_compDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPr35_timer_compDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPr35_timer_compDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/////////////////////////////////////////////////////////////////////////////////
//�������������� ���� �����
void CPr35_timer_compDlg::main_init(void)
{
	//unsigned int i;
    unsigned char buf[]="COMx";
    
    config_load();  //��������� ��������� ������
    buf[3]=config.com_num + 0x30;
    m_combo_comnum=buf;
    SetDlgItemText(IDC_COMBO_COMNUM, m_combo_comnum);

    timer_init(); //�������������� �������� �� oil.c
    
    ////��������������� ������
    //for(i=0;i<10000;i++)
    //{
    //   if(SetTimer(i, 1, 0)) break;	//��������� ���������� ������������� ��� �������
   // /}
    //if(i==10000) AfxMessageBox("Error: cannot initialize timer");

    return;
}

///////////////////////////////////////////
//����� ����� ������///////////////////////
///////////////////////////////////////////
void CPr35_timer_compDlg::OnButtonLoad() 
{
	char buf[255];
	
	dataout.flags_control.word=0;
	if(!timer_data_exchange())
	{
		//������� �������� ������
		timer_data_load();
		load_flag=1;
			
		_strtime(buf);
		sprintf(&buf[8],": ��������� ��������� - ��");
		m_static_state=buf;
		SetDlgItemText(IDC_STATIC_STATE, m_static_state );
	}
	else error_out();

}

void CPr35_timer_compDlg::OnButtonSave() 
{
	//��������� � ��������� dataout ��������� ������
	char buf[255];

	if(timer_data_save()) return;

	dataout.flags_control.word=0;
	dataout.flags_control.f_write_data=1;
	
	if(!timer_data_exchange())
	{
		_strtime(buf);
		sprintf(&buf[8],": ��������� - ��");
		m_static_state=buf;
		SetDlgItemText(IDC_STATIC_STATE, m_static_state );
	}
	else error_out();
}


void CPr35_timer_compDlg::OnButtonStart() 
{
	char buf[255];
	dataout.flags_control.word=0;
	dataout.flags_control.f_start=1;
	
	if(!timer_data_exchange())
	{
		_strtime(buf);
		sprintf(&buf[8],": �������� �������...");
		m_static_state=buf;
		SetDlgItemText(IDC_STATIC_STATE, m_static_state );	
	}
	else
	{ 
		error_out();
		return;
	}

	///////////
	dataout.flags_control.word=0;
	while(timer_data_exchange());
	
	_strtime(buf);
	sprintf(&buf[8],": ������ ������");
	m_static_state=buf;
	SetDlgItemText(IDC_STATIC_STATE, m_static_state );	

}


void CPr35_timer_compDlg::OnButtonTestStart() 
{
	char buf[255];
	
	dataout.flags_control.word=0;
	dataout.flags_control.f_test_start=1;
	if(!timer_data_exchange())
	{
		_strtime(buf);
		sprintf(&buf[8],": �������� ������� (����)...");
		m_static_state=buf;
		SetDlgItemText(IDC_STATIC_STATE, m_static_state );	
	}
	else
	{
		error_out();
		return;
	}
	
	///////////
	dataout.flags_control.word=0;
	while(timer_data_exchange());
	
	_strtime(buf);
	sprintf(&buf[8],": ������ ������");
	m_static_state=buf;
	SetDlgItemText(IDC_STATIC_STATE, m_static_state );	
	
}


void CPr35_timer_compDlg::OnKillfocusComboComnum() 
{
	const char* str;
	// TODO: Add your control notification handler code here
	UpdateData();	//��������� ������� �������� ���� ������ ����� �� ����������� ���� � ���������� ���� m_xxxx..
    str=m_combo_comnum;
    if((str[3]-0x30)!=config.com_num)
    {
        config.com_num=str[3]-0x30;
        config_save();
    }	
	
}


int CPr35_timer_compDlg::timer_data_exchange(void)
{
	unsigned char len;
	const char* str;
	int result=0;

	str=m_combo_comnum;
    InitComm((char*)str);


	if(CommTxPacket((unsigned char*)&dataout,sizeof(data_struct)))
	{
		result=1;	//������
	}		
	else
	{		
	    
        if(CommRxPacket((unsigned char*)&datain,&len,sizeof(data_struct)))
		{
			result=1;	//������
		}
		else
		{
		
            packet_count++;
          	result=0;	//��� ������
		}
	}

	CloseComm(); 

	return(result);
}

void CPr35_timer_compDlg::error_out(void)
{

	char buf[255];
	
	_strtime(buf);
	sprintf(&buf[8],": ������(%.2x): ��� �����",error_num);
	m_static_state=buf;
	SetDlgItemText(IDC_STATIC_STATE, m_static_state );

}

void CPr35_timer_compDlg::timer_data_load(void)
{

	unsigned char i;
	__int64 x,n,ost;
	
	unsigned char min,sec;
	unsigned short ms,mks,ns,on_off;
	unsigned char buf[255];


	for(i=0;i<16;i++)
	{
		x= *((__int64 *) &datain.t_global[i][0]);
		//������
		n=x/(60000000000);
		ost=x%60000000000;
		x=ost;
		min=(unsigned char)n;
		//�������
		n=x/1000000000;
		ost=x%1000000000;
		x=ost;
		sec=(unsigned char)n;
		//������������
		n=x/1000000;
		ost=x%1000000;
		x=ost;
		ms=(unsigned short)n;
		//������������
		n=x/1000;
		ost=x%1000;
		x=ost;
		mks=(unsigned short)n;
		//�����������
		ns=(unsigned short)x;
		//���\���� ������
		on_off= *((unsigned short *) &datain.channel_onoff[0]);
		if( on_off & ((unsigned short)1<<i) ) on_off=1;	
		else on_off=0;
		
	
		//������� ������
		switch(i)
		{
			case 0:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_1_min=buf;
				SetDlgItemText(IDC_EDIT_1_MIN, m_edit_1_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_1_sec=buf;
				SetDlgItemText(IDC_EDIT_1_SEC, m_edit_1_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_1_ms=buf;
				SetDlgItemText(IDC_EDIT_1_MS, m_edit_1_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_1_mks=buf;
				SetDlgItemText(IDC_EDIT_1_MKS, m_edit_1_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_1_ns=buf;
				SetDlgItemText(IDC_EDIT_1_NS, m_edit_1_ns);
				//���\����
				if(on_off) m_check_1_control.SetCheck(1);
				else       m_check_1_control.SetCheck(0);
				break;
			}
			case 1:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_2_min=buf;
				SetDlgItemText(IDC_EDIT_2_MIN, m_edit_2_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_2_sec=buf;
				SetDlgItemText(IDC_EDIT_2_SEC, m_edit_2_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_2_ms=buf;
				SetDlgItemText(IDC_EDIT_2_MS, m_edit_2_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_2_mks=buf;
				SetDlgItemText(IDC_EDIT_2_MKS, m_edit_2_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_2_ns=buf;
				SetDlgItemText(IDC_EDIT_2_NS, m_edit_2_ns);
				//���\����
				if(on_off) m_check_2_control.SetCheck(1);
				else       m_check_2_control.SetCheck(0);
				break;
			}
			case 2:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_3_min=buf;
				SetDlgItemText(IDC_EDIT_3_MIN, m_edit_3_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_3_sec=buf;
				SetDlgItemText(IDC_EDIT_3_SEC, m_edit_3_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_3_ms=buf;
				SetDlgItemText(IDC_EDIT_3_MS, m_edit_3_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_3_mks=buf;
				SetDlgItemText(IDC_EDIT_3_MKS, m_edit_3_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_3_ns=buf;
				SetDlgItemText(IDC_EDIT_3_NS, m_edit_3_ns);
				//���\����
				if(on_off) m_check_3_control.SetCheck(1);
				else       m_check_3_control.SetCheck(0);
				break;
			}
			case 3:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_4_min=buf;
				SetDlgItemText(IDC_EDIT_4_MIN, m_edit_4_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_4_sec=buf;
				SetDlgItemText(IDC_EDIT_4_SEC, m_edit_4_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_4_ms=buf;
				SetDlgItemText(IDC_EDIT_4_MS, m_edit_4_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_4_mks=buf;
				SetDlgItemText(IDC_EDIT_4_MKS, m_edit_4_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_4_ns=buf;
				SetDlgItemText(IDC_EDIT_4_NS, m_edit_4_ns);
				//���\����
				if(on_off) m_check_4_control.SetCheck(1);
				else       m_check_4_control.SetCheck(0);
				break;
			}
			case 4:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_5_min=buf;
				SetDlgItemText(IDC_EDIT_5_MIN, m_edit_5_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_5_sec=buf;
				SetDlgItemText(IDC_EDIT_5_SEC, m_edit_5_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_5_ms=buf;
				SetDlgItemText(IDC_EDIT_5_MS, m_edit_5_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_5_mks=buf;
				SetDlgItemText(IDC_EDIT_5_MKS, m_edit_5_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_5_ns=buf;
				SetDlgItemText(IDC_EDIT_5_NS, m_edit_5_ns);
				//���\����
				if(on_off) m_check_5_control.SetCheck(1);
				else       m_check_5_control.SetCheck(0);
				break;
			}
			case 5:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_6_min=buf;
				SetDlgItemText(IDC_EDIT_6_MIN, m_edit_6_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_6_sec=buf;
				SetDlgItemText(IDC_EDIT_6_SEC, m_edit_6_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_6_ms=buf;
				SetDlgItemText(IDC_EDIT_6_MS, m_edit_6_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_6_mks=buf;
				SetDlgItemText(IDC_EDIT_6_MKS, m_edit_6_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_6_ns=buf;
				SetDlgItemText(IDC_EDIT_6_NS, m_edit_6_ns);
				//���\����
				if(on_off) m_check_6_control.SetCheck(1);
				else       m_check_6_control.SetCheck(0);
				break;
			}
			case 6:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_7_min=buf;
				SetDlgItemText(IDC_EDIT_7_MIN, m_edit_7_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_7_sec=buf;
				SetDlgItemText(IDC_EDIT_7_SEC, m_edit_7_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_7_ms=buf;
				SetDlgItemText(IDC_EDIT_7_MS, m_edit_7_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_7_mks=buf;
				SetDlgItemText(IDC_EDIT_7_MKS, m_edit_7_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_7_ns=buf;
				SetDlgItemText(IDC_EDIT_7_NS, m_edit_7_ns);
				//���\����
				if(on_off) m_check_7_control.SetCheck(1);
				else       m_check_7_control.SetCheck(0);
				break;
			}
			case 7:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_8_min=buf;
				SetDlgItemText(IDC_EDIT_8_MIN, m_edit_8_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_8_sec=buf;
				SetDlgItemText(IDC_EDIT_8_SEC, m_edit_8_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_8_ms=buf;
				SetDlgItemText(IDC_EDIT_8_MS, m_edit_8_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_8_mks=buf;
				SetDlgItemText(IDC_EDIT_8_MKS, m_edit_8_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_8_ns=buf;
				SetDlgItemText(IDC_EDIT_8_NS, m_edit_8_ns);
				//���\����
				if(on_off) m_check_8_control.SetCheck(1);
				else       m_check_8_control.SetCheck(0);
				break;
			}
			case 8:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_9_min=buf;
				SetDlgItemText(IDC_EDIT_9_MIN, m_edit_9_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_9_sec=buf;
				SetDlgItemText(IDC_EDIT_9_SEC, m_edit_9_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_9_ms=buf;
				SetDlgItemText(IDC_EDIT_9_MS, m_edit_9_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_9_mks=buf;
				SetDlgItemText(IDC_EDIT_9_MKS, m_edit_9_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_9_ns=buf;
				SetDlgItemText(IDC_EDIT_9_NS, m_edit_9_ns);
				//���\����
				if(on_off) m_check_9_control.SetCheck(1);
				else       m_check_9_control.SetCheck(0);
				break;
			}
			case 9:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_10_min=buf;
				SetDlgItemText(IDC_EDIT_10_MIN, m_edit_10_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_10_sec=buf;
				SetDlgItemText(IDC_EDIT_10_SEC, m_edit_10_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_10_ms=buf;
				SetDlgItemText(IDC_EDIT_10_MS, m_edit_10_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_10_mks=buf;
				SetDlgItemText(IDC_EDIT_10_MKS, m_edit_10_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_10_ns=buf;
				SetDlgItemText(IDC_EDIT_10_NS, m_edit_10_ns);
				//���\����
				if(on_off) m_check_10_control.SetCheck(1);
				else       m_check_10_control.SetCheck(0);
				break;
			}
			case 10:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_11_min=buf;
				SetDlgItemText(IDC_EDIT_11_MIN, m_edit_11_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_11_sec=buf;
				SetDlgItemText(IDC_EDIT_11_SEC, m_edit_11_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_11_ms=buf;
				SetDlgItemText(IDC_EDIT_11_MS, m_edit_11_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_11_mks=buf;
				SetDlgItemText(IDC_EDIT_11_MKS, m_edit_11_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_11_ns=buf;
				SetDlgItemText(IDC_EDIT_11_NS, m_edit_11_ns);
				//���\����
				if(on_off) m_check_11_control.SetCheck(1);
				else       m_check_11_control.SetCheck(0);
				break;
			}
			case 11:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_12_min=buf;
				SetDlgItemText(IDC_EDIT_12_MIN, m_edit_12_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_12_sec=buf;
				SetDlgItemText(IDC_EDIT_12_SEC, m_edit_12_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_12_ms=buf;
				SetDlgItemText(IDC_EDIT_12_MS, m_edit_12_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_12_mks=buf;
				SetDlgItemText(IDC_EDIT_12_MKS, m_edit_12_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_12_ns=buf;
				SetDlgItemText(IDC_EDIT_12_NS, m_edit_12_ns);
				//���\����
				if(on_off) m_check_12_control.SetCheck(1);
				else       m_check_12_control.SetCheck(0);
				break;
			}
			case 12:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_13_min=buf;
				SetDlgItemText(IDC_EDIT_13_MIN, m_edit_13_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_13_sec=buf;
				SetDlgItemText(IDC_EDIT_13_SEC, m_edit_13_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_13_ms=buf;
				SetDlgItemText(IDC_EDIT_13_MS, m_edit_13_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_13_mks=buf;
				SetDlgItemText(IDC_EDIT_13_MKS, m_edit_13_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_13_ns=buf;
				SetDlgItemText(IDC_EDIT_13_NS, m_edit_13_ns);
				//���\����
				if(on_off) m_check_13_control.SetCheck(1);
				else       m_check_13_control.SetCheck(0);
				break;
			}
			case 13:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_14_min=buf;
				SetDlgItemText(IDC_EDIT_14_MIN, m_edit_14_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_14_sec=buf;
				SetDlgItemText(IDC_EDIT_14_SEC, m_edit_14_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_14_ms=buf;
				SetDlgItemText(IDC_EDIT_14_MS, m_edit_14_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_14_mks=buf;
				SetDlgItemText(IDC_EDIT_14_MKS, m_edit_14_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_14_ns=buf;
				SetDlgItemText(IDC_EDIT_14_NS, m_edit_14_ns);
				//���\����
				if(on_off) m_check_14_control.SetCheck(1);
				else       m_check_14_control.SetCheck(0);
				break;
			}
			case 14:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_15_min=buf;
				SetDlgItemText(IDC_EDIT_15_MIN, m_edit_15_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_15_sec=buf;
				SetDlgItemText(IDC_EDIT_15_SEC, m_edit_15_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_15_ms=buf;
				SetDlgItemText(IDC_EDIT_15_MS, m_edit_15_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_15_mks=buf;
				SetDlgItemText(IDC_EDIT_15_MKS, m_edit_15_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_15_ns=buf;
				SetDlgItemText(IDC_EDIT_15_NS, m_edit_15_ns);
				//���\����
				if(on_off) m_check_15_control.SetCheck(1);
				else       m_check_15_control.SetCheck(0);
				break;
			}
			case 15:
			{
				//���
				sprintf((char *)buf,"%.2u",min);
				m_edit_16_min=buf;
				SetDlgItemText(IDC_EDIT_16_MIN, m_edit_16_min);
				//���
				sprintf((char *)buf,"%.2u",sec);
				m_edit_16_sec=buf;
				SetDlgItemText(IDC_EDIT_16_SEC, m_edit_16_sec);
				//��
				sprintf((char *)buf,"%.3u",ms);
				m_edit_16_ms=buf;
				SetDlgItemText(IDC_EDIT_16_MS, m_edit_16_ms);
				//���
				sprintf((char *)buf,"%.3u",mks);
				m_edit_16_mks=buf;
				SetDlgItemText(IDC_EDIT_16_MKS, m_edit_16_mks);
				//��
				sprintf((char *)buf,"%.3u",ns);
				m_edit_16_ns=buf;
				SetDlgItemText(IDC_EDIT_16_NS, m_edit_16_ns);
				//���\����
				if(on_off) m_check_16_control.SetCheck(1);
				else       m_check_16_control.SetCheck(0);
				break;
			}
		
		}	
	}

	return;
}

int CPr35_timer_compDlg::timer_data_save(void)
{

	unsigned char i;
	__int64 x,*x_ptr;
	unsigned char min,sec;
	unsigned short ms,mks,ns,on_off,*on_off_ptr;
	

	UpdateData();

	for(i=0;i<16;i++)
	{
		
		switch(i)
		{
			case 0:
			{
				//���
				min=atoi(m_edit_1_min);
				//���
				sec=atoi(m_edit_1_sec);
				//��
				ms=atoi(m_edit_1_ms);
				//���
				mks=atoi(m_edit_1_mks);
				//��
				ns=atoi(m_edit_1_ns);
				//���\����
				if(m_check_1==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 1:
			{
				//���
				min=atoi(m_edit_2_min);
				//���
				sec=atoi(m_edit_2_sec);
				//��
				ms=atoi(m_edit_2_ms);
				//���
				mks=atoi(m_edit_2_mks);
				//��
				ns=atoi(m_edit_2_ns);
				//���\����
				if(m_check_2==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 2:
			{
				//���
				min=atoi(m_edit_3_min);
				//���
				sec=atoi(m_edit_3_sec);
				//��
				ms=atoi(m_edit_3_ms);
				//���
				mks=atoi(m_edit_3_mks);
				//��
				ns=atoi(m_edit_3_ns);
				//���\����
				if(m_check_3==1) on_off=1;
				else             on_off=0;
				break;
			}	
			case 3:
			{
				//���
				min=atoi(m_edit_4_min);
				//���
				sec=atoi(m_edit_4_sec);
				//��
				ms=atoi(m_edit_4_ms);
				//���
				mks=atoi(m_edit_4_mks);
				//��
				ns=atoi(m_edit_4_ns);
				//���\����
				if(m_check_4==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 4:
			{
				//���
				min=atoi(m_edit_5_min);
				//���
				sec=atoi(m_edit_5_sec);
				//��
				ms=atoi(m_edit_5_ms);
				//���
				mks=atoi(m_edit_5_mks);
				//��
				ns=atoi(m_edit_5_ns);
				//���\����
				if(m_check_5==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 5:
			{
				//���
				min=atoi(m_edit_6_min);
				//���
				sec=atoi(m_edit_6_sec);
				//��
				ms=atoi(m_edit_6_ms);
				//���
				mks=atoi(m_edit_6_mks);
				//��
				ns=atoi(m_edit_6_ns);
				//���\����
				if(m_check_6==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 6:
			{
				//���
				min=atoi(m_edit_7_min);
				//���
				sec=atoi(m_edit_7_sec);
				//��
				ms=atoi(m_edit_7_ms);
				//���
				mks=atoi(m_edit_7_mks);
				//��
				ns=atoi(m_edit_7_ns);
				//���\����
				if(m_check_7==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 7:
			{
				//���
				min=atoi(m_edit_8_min);
				//���
				sec=atoi(m_edit_8_sec);
				//��
				ms=atoi(m_edit_8_ms);
				//���
				mks=atoi(m_edit_8_mks);
				//��
				ns=atoi(m_edit_8_ns);
				//���\����
				if(m_check_8==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 8:
			{
				//���
				min=atoi(m_edit_9_min);
				//���
				sec=atoi(m_edit_9_sec);
				//��
				ms=atoi(m_edit_9_ms);
				//���
				mks=atoi(m_edit_9_mks);
				//��
				ns=atoi(m_edit_9_ns);
				//���\����
				if(m_check_9==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 9:
			{
				//���
				min=atoi(m_edit_10_min);
				//���
				sec=atoi(m_edit_10_sec);
				//��
				ms=atoi(m_edit_10_ms);
				//���
				mks=atoi(m_edit_10_mks);
				//��
				ns=atoi(m_edit_10_ns);
				//���\����
				if(m_check_10==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 10:
			{
				//���
				min=atoi(m_edit_11_min);
				//���
				sec=atoi(m_edit_11_sec);
				//��
				ms=atoi(m_edit_11_ms);
				//���
				mks=atoi(m_edit_11_mks);
				//��
				ns=atoi(m_edit_11_ns);
				//���\����
				if(m_check_11==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 11:
			{
				//���
				min=atoi(m_edit_12_min);
				//���
				sec=atoi(m_edit_12_sec);
				//��
				ms=atoi(m_edit_12_ms);
				//���
				mks=atoi(m_edit_12_mks);
				//��
				ns=atoi(m_edit_12_ns);
				//���\����
				if(m_check_12==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 12:
			{
				//���
				min=atoi(m_edit_13_min);
				//���
				sec=atoi(m_edit_13_sec);
				//��
				ms=atoi(m_edit_13_ms);
				//���
				mks=atoi(m_edit_13_mks);
				//��
				ns=atoi(m_edit_13_ns);
				//���\����
				if(m_check_13==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 13:
			{
				//���
				min=atoi(m_edit_14_min);
				//���
				sec=atoi(m_edit_14_sec);
				//��
				ms=atoi(m_edit_14_ms);
				//���
				mks=atoi(m_edit_14_mks);
				//��
				ns=atoi(m_edit_14_ns);
				//���\����
				if(m_check_14==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 14:
			{
				//���
				min=atoi(m_edit_15_min);
				//���
				sec=atoi(m_edit_15_sec);
				//��
				ms=atoi(m_edit_15_ms);
				//���
				mks=atoi(m_edit_15_mks);
				//��
				ns=atoi(m_edit_15_ns);
				//���\����
				if(m_check_15==1) on_off=1;
				else             on_off=0;
				break;
			}
			case 15:
			{
				//���
				min=atoi(m_edit_16_min);
				//���
				sec=atoi(m_edit_16_sec);
				//��
				ms=atoi(m_edit_16_ms);
				//���
				mks=atoi(m_edit_16_mks);
				//��
				ns=atoi(m_edit_16_ns);
				//���\����
				if(m_check_16==1) on_off=1;
				else             on_off=0;
				break;
			}
		}
		
		//��������
		if(load_flag==0)
		{
			MessageBox("������ ������� ������ \'��������� ���������\'", "������",/*MB_ICONERROR*/MB_ICONWARNING | MB_OK);
			return(1);
		}
		if(min>59)
		{
			MessageBox("�������� ����� �� ������ ��������� 59.", "������",/*MB_ICONERROR*/MB_ICONWARNING | MB_OK);
			return(1);
		}
		if(sec>59)
		{
			MessageBox("�������� ������ �� ������ ��������� 59.", "������",/*MB_ICONERROR*/MB_ICONWARNING | MB_OK);
			return(1);
		}
		if((ns%25)!=0)
		{
			MessageBox("�������� ���������� ������ ���� ������ 25.", "������",/*MB_ICONERROR*/MB_ICONWARNING | MB_OK);
			return(1);
		}
		//AfxMessageBox("Simple message box.");
	

		//t_global
		x=((__int64)min*60000000000)+((__int64)sec*1000000000)+((__int64)ms*1000000)+((__int64)mks*1000)+(__int64)ns;
		x_ptr=(__int64 *) &dataout.t_global[i][0];
		*x_ptr=x;
		//channel_onoff
		on_off_ptr=(unsigned short *)&dataout.channel_onoff[0];
		if(on_off)	*on_off_ptr= *on_off_ptr | (1<<i);
		else        *on_off_ptr= *on_off_ptr & ~(1<<i);
				
	}

	return(0);
}

