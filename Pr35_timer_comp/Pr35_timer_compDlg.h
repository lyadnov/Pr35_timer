// Pr35_timer_compDlg.h : header file
//

#if !defined(AFX_PR35_TIMER_COMPDLG_H__7F0AC654_67DA_47D9_BB93_D898CB1AB495__INCLUDED_)
#define AFX_PR35_TIMER_COMPDLG_H__7F0AC654_67DA_47D9_BB93_D898CB1AB495__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPr35_timer_compDlg dialog

class CPr35_timer_compDlg : public CDialog
{
// Construction
public:
	CPr35_timer_compDlg(CWnd* pParent = NULL);	// standard constructor
	
	void main_init(void);			//инициаллизация моих данных (не MFCшных)
	int timer_data_exchange(void);	//передаем и получаем структуры datain dataout через COM-порт
	void error_out(void);			//выводим номер ошибки
	void timer_data_load(void);		//выводим в диалоговое окно все полученные временные интервалы для таймера
	int timer_data_save(void);		//сохраняем в структуру dataout данные из диалогового окна

	// Dialog Data
	//{{AFX_DATA(CPr35_timer_compDlg)
	enum { IDD = IDD_PR35_TIMER_COMP_DIALOG };
	CButton	m_check_9_control;
	CButton	m_check_8_control;
	CButton	m_check_7_control;
	CButton	m_check_6_control;
	CButton	m_check_5_control;
	CButton	m_check_4_control;
	CButton	m_check_3_control;
	CButton	m_check_16_control;
	CButton	m_check_15_control;
	CButton	m_check_14_control;
	CButton	m_check_13_control;
	CButton	m_check_12_control;
	CButton	m_check_11_control;
	CButton	m_check_10_control;
	CButton	m_check_2_control;
	CButton	m_check_1_control;
	CString	m_Edit_Receive;
	CString	m_combo_comnum;
	CString	m_edit_1_sec;
	CString	m_edit_1_min;
	CString	m_edit_1_mks;
	CString	m_edit_1_ms;
	CString	m_edit_1_ns;
	BOOL	m_check_1;
	CString	m_edit_2_min;
	CString	m_edit_3_min;
	CString	m_edit_4_min;
	CString	m_edit_5_min;
	CString	m_edit_6_min;
	CString	m_edit_7_min;
	CString	m_edit_8_min;
	CString	m_edit_9_min;
	CString	m_edit_10_min;
	CString	m_edit_11_min;
	CString	m_edit_12_min;
	CString	m_edit_13_min;
	CString	m_edit_14_min;
	CString	m_edit_15_min;
	CString	m_edit_16_min;
	CString	m_edit_2_mks;
	CString	m_edit_3_mks;
	CString	m_edit_4_mks;
	CString	m_edit_5_mks;
	CString	m_edit_6_mks;
	CString	m_edit_7_mks;
	CString	m_edit_8_mks;
	CString	m_edit_9_mks;
	CString	m_edit_10_mks;
	CString	m_edit_11_mks;
	CString	m_edit_12_mks;
	CString	m_edit_13_mks;
	CString	m_edit_14_mks;
	CString	m_edit_15_mks;
	CString	m_edit_16_mks;
	CString	m_edit_2_ms;
	CString	m_edit_2_ns;
	CString	m_edit_2_sec;
	BOOL	m_check_2;
	BOOL	m_check_10;
	BOOL	m_check_11;
	BOOL	m_check_12;
	BOOL	m_check_13;
	BOOL	m_check_14;
	BOOL	m_check_15;
	BOOL	m_check_16;
	BOOL	m_check_3;
	BOOL	m_check_4;
	BOOL	m_check_5;
	BOOL	m_check_6;
	BOOL	m_check_7;
	BOOL	m_check_8;
	BOOL	m_check_9;
	CString	m_edit_10_ms;
	CString	m_edit_10_ns;
	CString	m_edit_10_sec;
	CString	m_edit_11_ms;
	CString	m_edit_11_ns;
	CString	m_edit_11_sec;
	CString	m_edit_12_ms;
	CString	m_edit_12_ns;
	CString	m_edit_12_sec;
	CString	m_edit_13_ms;
	CString	m_edit_13_ns;
	CString	m_edit_13_sec;
	CString	m_edit_14_ms;
	CString	m_edit_14_ns;
	CString	m_edit_14_sec;
	CString	m_edit_15_ms;
	CString	m_edit_15_ns;
	CString	m_edit_15_sec;
	CString	m_edit_16_ms;
	CString	m_edit_16_ns;
	CString	m_edit_16_sec;
	CString	m_edit_3_ms;
	CString	m_edit_3_ns;
	CString	m_edit_3_sec;
	CString	m_edit_4_ms;
	CString	m_edit_4_ns;
	CString	m_edit_4_sec;
	CString	m_edit_5_ms;
	CString	m_edit_5_ns;
	CString	m_edit_5_sec;
	CString	m_edit_6_ms;
	CString	m_edit_6_ns;
	CString	m_edit_6_sec;
	CString	m_edit_7_ms;
	CString	m_edit_7_ns;
	CString	m_edit_7_sec;
	CString	m_edit_8_ms;
	CString	m_edit_8_ns;
	CString	m_edit_8_sec;
	CString	m_edit_9_ms;
	CString	m_edit_9_ns;
	CString	m_edit_9_sec;
	CString	m_static_state;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPr35_timer_compDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPr35_timer_compDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonSave();
	afx_msg void OnKillfocusComboComnum();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonTestStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PR35_TIMER_COMPDLG_H__7F0AC654_67DA_47D9_BB93_D898CB1AB495__INCLUDED_)
