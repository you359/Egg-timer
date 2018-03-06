// Egg-timerDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CEggtimerDlg ��ȭ ����
class CEggtimerDlg : public CDialog
{
// �����Դϴ�.
public:
	CEggtimerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EGGTIMER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CFont m_font;
	bool m_isExceed;
public:
	CStatic m_static_main;
	CEdit m_hour_edit;
	CEdit m_minute_edit;
	CEdit m_second_edit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	int m_hour_value;
	int m_minute_value;
	int m_second_value;
	int m_mSecond;
	int m_timer_value;

	bool m_isplayed;
	bool m_isplaying;
	bool m_playSound;
public:
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CProgressCtrl m_timer_progress;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CButton m_play_puase_btn;
};
