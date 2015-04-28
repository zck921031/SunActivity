
// RecognitionSystemDlg.h : 头文件
//

#pragma once

#include "cv.h"
#include "highgui.h"

#include <map>
using namespace std;



// CRecognitionSystemDlg 对话框
class CRecognitionSystemDlg : public CDialogEx
{
public:
	void myinit();
	map<string, string> pars;
	vector< vector<string> > imageNames;
	void CRecognitionSystemDlg::ParsePars(map<string,string>&pars);
// 构造
public:
	CRecognitionSystemDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RECOGNITIONSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
