
// RecognitionSystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RecognitionSystem.h"
#include "RecognitionSystemDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRecognitionSystemDlg 对话框



CRecognitionSystemDlg::CRecognitionSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecognitionSystemDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRecognitionSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRecognitionSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRecognitionSystemDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CRecognitionSystemDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CRecognitionSystemDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRecognitionSystemDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_Open, &CRecognitionSystemDlg::OnBnClickedButtonOpen)
END_MESSAGE_MAP()


// CRecognitionSystemDlg 消息处理程序

BOOL CRecognitionSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	myinit();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRecognitionSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRecognitionSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRecognitionSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRecognitionSystemDlg::ParsePars(map<string,string>&pars){
	CString strTemp;
	stringstream ss;
	((CComboBox*)GetDlgItem(IDC_COMBO1))->GetWindowText(strTemp);
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString (strTemp);
	int modal_idx = find( begin(modal), end(modal), string( pszConvertedAnsiString ) ) - begin(modal);
	ss<<modal_idx;
	pars["modal"] = ss.str();
	ss.clear();

	//parse day
	((CComboBox*)GetDlgItem(IDC_EDIT1))->GetWindowText(strTemp);
	CT2CA pszConvertedAnsiString2 (strTemp);
	if ( pars["day"] != string( pszConvertedAnsiString2 )  ){
		result.clear();
	}
	pars["day"] = string( pszConvertedAnsiString2 );
	
	
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK_flare ) )
	{ pars["flare"] = "true"; }	else{ pars["flare"] = "false"; }
	
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK_CH ) )
	{ pars["CH"] = "true"; }	else{ pars["CH"] = "false"; }
	
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK_SS ) )
	{ pars["SS"] = "true"; }	else{ pars["SS"] = "false"; }

	if ( ((CButton*)GetDlgItem(IDC_RADIO_Euclidean))->GetCheck() ){
		pars["distanceName"] = "Euclidean";
	}else{
		pars["distanceName"] = "L_mmLMNN.txt";
	}
}

void CRecognitionSystemDlg::myinit()
{
	pars["imageNames"] = "C://Users//zck//Documents//GitHub//SunActivity//Code//RecognitionSystem//RecognitionSystem//data//ImageNameByDay.txt";
	pars["imagepath"] = "C://Users//zck//Documents//GitHub//SunActivity//data//regional annotation//img//";
	pars["featurepath"] = "C://Users//zck//Documents//GitHub//SunActivity//Code//AnnotationSystem//AnnotationSystem//feature//";
	pars["distancepath"] = "C://Users//zck//Documents//GitHub//SunActivity//Code//matlab//Lab//embed//";
	pars["distanceName"] = "L_mmLMNN.txt";
	pars["flare"] = "false";
	pars["CH"] = "false";
	pars["SS"] = "false";
	pars["day"] = "0";
	pars["modal"] = "0";

	imageNames.clear();
	ifstream in( pars["imageNames"] );
	char *buf = new char[1<<20];
	while( in.getline(buf, 1<<20) ){
		if ( *buf == '#' ) continue;
		if ( strlen(buf) < 3 ) continue;
		imageNames.push_back( SplitStringByChar(buf, ',' ) );
	}
	delete []buf;
	in.close();


	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_EDIT1))->SetWindowText( _T("0") );
	((CButton*)GetDlgItem(IDC_RADIO_Euclidean))->SetCheck(TRUE);
}

//按下开始识别按钮
void CRecognitionSystemDlg::OnBnClickedButton1()
{
	//cv::Mat img = cv::imread("C://Users//zck//Pictures//样例图片//头像.jpg");
	//cv::Mat img = load_image("H://毕业设计//Flare_Noflare//img//Flare//2//0094.jpg");
	//cv::namedWindow("1");
	//cv::imshow("1", img);
	//vector< vector<double> > x;
	//vector<int> h;
	//load_feature("..//..//..//data//TrainSet//feature//Flare_Noflare//feature//", x, h, 0.5);
	result.clear();
	ParsePars(pars);

	stringstream ss;
	Recognition r("flare", pars, 320, 1800);
	if ( "true" == pars["CH"] ) r = Recognition("Coronal Hole", pars, 512, 1550);
	if ( "true" == pars["SS"] ) r = Recognition("Sunspot", pars, 72, 1400);

	int wave=0, day=3;
	wave = atoi( pars["modal"].c_str() );
	day =  atoi( pars["day"].c_str() );
	cv::Mat img = cv::imread( pars["imagepath"] + "//" + imageNames[day][ wave ] );
	cv::Mat sceen;
	cv::resize(img, sceen, Size(512,512) );

	vector<Rect> res = r.recognition( imageNames[day] );
	for (  Rect t : res ){
		rectangle(sceen, Rect(t.x/8, t.y/8, t.width/8, t.height/8 ), Scalar(0,255,255) );
		result.push_back( make_pair(t, Scalar(0,255,255)) );
	}


	ss<<imageNames[3][0]<<" fisished~";
	
	CEdit* pBoxOne;
	pBoxOne = (CEdit*) GetDlgItem(IDC_EDIT2);
	string sss = ss.str();
	CString cs = CString( sss.c_str() );
	pBoxOne->SetWindowText( cs );

	OnBnClickedButtonOpen();
	// TODO: 在此添加控件通知处理程序代码
}


void CRecognitionSystemDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	ParsePars(pars);
	stringstream ss;
	ss<<pars["day"]<<"  , ";
	ss<<pars["modal"]<<" , "<<pars["flare"]<<" , "<<pars["CH"]<<" , "<<pars["SS"]<<" , "<<pars["distanceName"];
	
	((CComboBox*)GetDlgItem(IDC_EDIT2))->SetWindowText( CString( ss.str().c_str() ) );
}


void CRecognitionSystemDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cs;
	((CComboBox*)GetDlgItem(IDC_EDIT1))->GetWindowText( cs );
	int _day = max(_wtoi(cs) - 1, 0);	
	cs.Format( _T("%d"), _day);
	((CComboBox*)GetDlgItem(IDC_EDIT1))->SetWindowText( cs );
}


void CRecognitionSystemDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cs;
	((CComboBox*)GetDlgItem(IDC_EDIT1))->GetWindowText( cs );
	int _day = min(_wtoi(cs) + 1, 365);
	cs.Format( _T("%d"), _day);
	((CComboBox*)GetDlgItem(IDC_EDIT1))->SetWindowText( cs );
}



void CRecognitionSystemDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码	
	ParsePars(pars);
	stringstream ss;
	int wave=0, day=3;
	wave = atoi( pars["modal"].c_str() );
	day =  atoi( pars["day"].c_str() );
	cv::Mat img = cv::imread( pars["imagepath"] + "//" + imageNames[day][ wave ] );
	cv::Mat sceen;
	cv::resize(img, sceen, Size(512,512) );

	for (  auto t : result ){
		rectangle(sceen, Rect(t.first.x/8, t.first.y/8, t.first.width/8, t.first.height/8 ), t.second );
	}


	cv::namedWindow("1");
	cv::imshow("1", sceen);



}
