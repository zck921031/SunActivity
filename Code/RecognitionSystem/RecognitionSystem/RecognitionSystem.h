
// RecognitionSystem.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRecognitionSystemApp:
// �йش����ʵ�֣������ RecognitionSystem.cpp
//

class CRecognitionSystemApp : public CWinApp
{
public:
	CRecognitionSystemApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRecognitionSystemApp theApp;