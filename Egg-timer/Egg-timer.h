// Egg-timer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CEggtimerApp:
// �� Ŭ������ ������ ���ؼ��� Egg-timer.cpp�� �����Ͻʽÿ�.
//

class CEggtimerApp : public CWinApp
{
public:
	CEggtimerApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CEggtimerApp theApp;