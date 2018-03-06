// Egg-timerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Egg-timer.h"
#include "Egg-timerDlg.h"

#include <cmath>
#pragma comment(lib, "winmm")
#include <mmsystem.h>

#define TIMER_ID 1354

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CEggtimerDlg 대화 상자




CEggtimerDlg::CEggtimerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEggtimerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEggtimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_hour_edit);
	DDX_Control(pDX, IDC_EDIT2, m_minute_edit);
	DDX_Control(pDX, IDC_EDIT3, m_second_edit);
	DDX_Control(pDX, IDC_STATIC_MAIN, m_static_main);
	DDX_Control(pDX, IDC_PROGRESS1, m_timer_progress);
	DDX_Control(pDX, IDC_BUTTON2, m_play_puase_btn);
}

BEGIN_MESSAGE_MAP(CEggtimerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CEggtimerDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CEggtimerDlg::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CEggtimerDlg::OnDeltaposSpin3)
	ON_EN_CHANGE(IDC_EDIT1, &CEggtimerDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CEggtimerDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CEggtimerDlg::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON1, &CEggtimerDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CEggtimerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CEggtimerDlg::OnBnClickedButton3)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CEggtimerDlg 메시지 처리기

BOOL CEggtimerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_hour_value = 0;
	m_minute_value = 0;
	m_second_value = 0;

	m_font.CreatePointFont(900,(LPCTSTR)"굴림");
	GetDlgItem(IDC_STATIC_MAIN)->SetFont(&m_font);

	m_isExceed = false;

	CRect r;
	m_hour_edit.GetRect(r);

	r.left += 5;
	r.right -= 5;
	r.top += 20;
	r.bottom -= 6;

	m_hour_edit.SetRect(r);

	m_hour_edit.SetWindowText(_T("00"));
	m_minute_edit.SetWindowText(_T("00"));
	m_second_edit.SetWindowText(_T("00"));

	m_isplayed = false;
	m_isplaying = false;
	m_playSound = false;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CEggtimerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CEggtimerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CEggtimerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CEggtimerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CEggtimerDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pNMUpDown->iDelta < 0)
	{
		if(m_hour_value >= 60)
			return;

		m_hour_value++;
	}
	else
	{	
		if(m_hour_value <= 0)	
			return;

		m_hour_value--;
	}
	
	CString text;
	text.Format(_T("%02d"), m_hour_value);
	m_hour_edit.SetWindowText(text);
	UpdateData(FALSE);

	*pResult = 0;
}

void CEggtimerDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pNMUpDown->iDelta < 0)
	{
		if(m_minute_value >= 60)
			return;

		m_minute_value++;
	}
	else
	{	
		if(m_minute_value <= 0)	
			return;

		m_minute_value--;
	}

	CString text;
	text.Format(_T("%02d"), m_minute_value);
	m_minute_edit.SetWindowText(text);
	UpdateData(FALSE);

	*pResult = 0;
}

void CEggtimerDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pNMUpDown->iDelta < 0)
	{
		if(m_second_value >= 60)
			return;

		m_second_value++;
	}
	else
	{	
		if(m_second_value <= 0)	
			return;

		m_second_value--;
	}

	CString text;
	text.Format(_T("%02d"), m_second_value);
	m_second_edit.SetWindowText(text);
	UpdateData(FALSE);

	*pResult = 0;
}

void CEggtimerDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString text;
	m_hour_edit.GetWindowText(text);
	
	m_hour_value = _ttoi(text);
}

void CEggtimerDlg::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString text;
	m_minute_edit.GetWindowText(text);

	m_minute_value = _ttoi(text);
}

void CEggtimerDlg::OnEnChangeEdit3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString text;
	m_second_edit.GetWindowText(text);

	m_second_value = _ttoi(text);
}

void CEggtimerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_ID:
		if(m_isplaying)
		{
			m_timer_value -=1;

			int hour;
			int minute;
			int second;
			bool beepSound = false;

			if(m_timer_value >= 0)
			{
				hour = m_timer_value / 3600;
				minute = (m_timer_value % 3600) / 60;
				second = (m_timer_value % 3600) % 60;
				m_timer_progress.StepIt();
			}
			else
			{
				m_isExceed = true;
				hour = (m_timer_value / 3600) * -1;
				minute = ((m_timer_value % 3600) / 60) *-1;
				second = ((m_timer_value % 3600) % 60) * -1;

				//sound start
				//Beep(768, 300);
				if(!m_playSound) {
					CString szSoundPath = _T("bgm.wav");
					PlaySound(szSoundPath, AfxGetInstanceHandle(),  SND_ASYNC | SND_LOOP); // 무한
					//PlaySound(szSoundPath, AfxGetInstanceHandle(),  SND_ASYNC); // 1회 재생
					m_playSound = true;
				}
			}

			CString text;
			text.Format(_T("%02d:%02d:%02d"), hour, minute, second);
			m_static_main.SetWindowText(text);
		}

		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CEggtimerDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(TIMER_ID);
	m_isplayed = false;
	m_play_puase_btn.SetWindowText(_T("Start"));
	m_isplaying = false;
	if(m_playSound) {
		PlaySound(NULL, AfxGetInstanceHandle(),  NULL); // 정지
		m_playSound = false;
	}

	m_mSecond = (m_hour_value*(60*60) + m_minute_value*(60) + m_second_value);
	CString text;
	text.Format(_T("%02d:%02d:%02d"), m_hour_value, m_minute_value, m_second_value);
	m_static_main.SetWindowText(text);
	m_timer_value = m_mSecond;

	m_timer_progress.SetPos(0);
	m_timer_progress.SetRange(0, m_mSecond);
	m_timer_progress.SetStep(1);

	m_isExceed = false;
}

void CEggtimerDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(!m_isplayed)
	{
		m_mSecond = (m_hour_value*(60*60) + m_minute_value*(60) + m_second_value);
		CString text;
		text.Format(_T("%02d:%02d:%02d"), m_hour_value, m_minute_value, m_second_value);
		m_static_main.SetWindowText(text);
		m_timer_value = m_mSecond;

		SetTimer(TIMER_ID, 1000, NULL);
		m_timer_value = m_mSecond;
		m_isExceed = false;
		m_isplayed = true;
	}

	if(m_isplaying)
	{
		m_play_puase_btn.SetWindowText(_T("Start"));
		m_isplaying = false;
		if(m_playSound) {
			PlaySound(NULL, AfxGetInstanceHandle(),  NULL); // 정지
			m_playSound = false;
		}
	}
	else
	{
		m_play_puase_btn.SetWindowText(_T("Pause"));
		m_isplaying = true;
		if(m_playSound) {
			PlaySound(NULL, AfxGetInstanceHandle(),  NULL); // 정지
			m_playSound = false;
		}
	}
}

void CEggtimerDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_isExceed = false;
	m_timer_value = m_mSecond;
	KillTimer(TIMER_ID);

	m_play_puase_btn.SetWindowText(_T("Start"));
	m_isplaying = false;
	m_isplayed = false;
	if(m_playSound) {
		PlaySound(NULL, AfxGetInstanceHandle(),  NULL); // 정지
		m_playSound = false;
	}

	m_timer_value = m_mSecond;
	m_timer_progress.SetPos(0);
	m_timer_progress.SetRange(0, m_mSecond);
	m_timer_progress.SetStep(1);
}

HBRUSH CEggtimerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_MAIN)
	{
		if(m_isExceed)
			pDC->SetTextColor(RGB(255,0,0));
		else
			pDC->SetTextColor(RGB(0,0,0));
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
