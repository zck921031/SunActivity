
// RecognitionSystemDlg.h : ͷ�ļ�
//

#pragma once

#include "cv.h"
#include "highgui.h"

#include <map>
using namespace std;



// CRecognitionSystemDlg �Ի���
class CRecognitionSystemDlg : public CDialogEx
{
public:
	void myinit();
	map<string, string> pars;
	vector< vector<string> > imageNames;
	void CRecognitionSystemDlg::ParsePars(map<string,string>&pars);
// ����
public:
	CRecognitionSystemDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RECOGNITIONSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
