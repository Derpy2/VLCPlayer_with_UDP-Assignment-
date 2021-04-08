
// VLC1Dlg.cpp: 实现文件
//



#include "pch.h"
#include "framework.h"
#include "VLC1.h"
#include "VLC1Dlg.h"
#include "afxdialogex.h"
#include <string>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnPause();
//	afx_msg void OnPlay();
//	afx_msg void OnStop();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_COMMAND(IDC_PAUSE, &CAboutDlg::OnPause)
//	ON_COMMAND(IDC_PLAY, &CAboutDlg::OnPlay)
//	ON_COMMAND(IDC_STOP, &CAboutDlg::OnStop)
END_MESSAGE_MAP()


// CVLC1Dlg 对话框



CVLC1Dlg::CVLC1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VLC1_DIALOG, pParent)
	, m_isRTSP(FALSE)
	, m_VideoPath(_T(""))
	, Now_Time(_T(""))
	, Tot_Time(_T(""))
{
	//变量赋初值
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_vlcinst = NULL;
	m_vlcMedia = NULL;
	m_vlcMplay = NULL;
	m_bNew = true;
	m_bIsRtsp = false;
	m_TotalTime = 0;
	m_playerTimer = 0;
	m_VideoPath = _T("");
}

void CVLC1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_ISRTSP, m_isRTSP);
	DDX_Control(pDX, IDC_PLAYTIME, m_SliderPlayer);
	DDX_Text(pDX, IDC_VIDEO_PATH, m_VideoPath);
	DDX_Control(pDX, IDC_VOLUMN, m_SliderVolume);
	DDX_Text(pDX, IDC_NowTime, Now_Time);
	DDX_Text(pDX, IDC_TotTime, Tot_Time);
	DDX_Control(pDX, IDC_LIST_FILE, m_List_File);
}

BEGIN_MESSAGE_MAP(CVLC1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PAUSE, &CVLC1Dlg::OnClickedPause)
	ON_BN_CLICKED(IDC_PLAY, &CVLC1Dlg::OnClickedPlay)
	ON_BN_CLICKED(IDC_STOP, &CVLC1Dlg::OnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CVLC1Dlg::OnClickedButtonTest)
	ON_BN_CLICKED(IDC_SELECT_VIDEO, &CVLC1Dlg::OnClickedSelectVideo)
	ON_EN_CHANGE(IDC_VIDEO_PATH, &CVLC1Dlg::OnChangeVideoPath)
	ON_BN_CLICKED(IDC_ISRTSP, &CVLC1Dlg::OnClickedIsrtsp)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_GoBack, &CVLC1Dlg::OnClickedGoback)
	ON_BN_CLICKED(IDC_FastForward, &CVLC1Dlg::OnClickedFastforward)
	ON_LBN_SELCHANGE(IDC_LIST1, &CVLC1Dlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_AddBut, &CVLC1Dlg::OnClickedAddbut)
	ON_BN_CLICKED(IDC_DelBut, &CVLC1Dlg::OnClickedDelbut)
	ON_BN_CLICKED(IDC_BUTTON_PlaySel, &CVLC1Dlg::OnClickedButtonPlaysel)
	ON_BN_CLICKED(IDC_BUTTON_CLR, &CVLC1Dlg::OnClickedButtonClr)
	ON_BN_CLICKED(IDC_PLAY_FLV, &CVLC1Dlg::OnBnClickedPlayFlv)
	ON_BN_CLICKED(IDC_START_LIVE, &CVLC1Dlg::OnBnClickedStartLive)
	ON_BN_CLICKED(IDC_STOP_LIVE, &CVLC1Dlg::OnBnClickedStopLive)
END_MESSAGE_MAP()

char* EncodeToUTF8(const wchar_t* mbcsStr);
// CVLC1Dlg 消息处理程序
BOOL contectToDataBase(CString userName, CString userPassword) {
	MYSQL* con; //= mysql_init((MYSQL*) 0); 
	MYSQL_RES* res = NULL;
	MYSQL_ROW row;

	//database configuartion
	char dbuser[30] = "root";
	char dbpasswd[30] = "CyanCloud"; // it must be    changed
	char dbip[30] = "localhost";
	char dbname[50] = "vlc";
	char tablename[50] = "tb_user";
	BOOL flag = FALSE;

	int count = 0;
	//初始化及连接数据库
	con = mysql_init((MYSQL*)0);

	if (!mysql_real_connect(con, dbip, dbuser, dbpasswd, dbname, 3306, NULL, 0))
	{

		printf("Error connection %s\n", mysql_error(con));
		return FALSE;
	}
	CString tmp = _T(" select user_password from tb_user where user_name = '");
	tmp = tmp + userName;
	tmp = tmp + _T("' ");

	if (mysql_query(con, EncodeToUTF8(tmp.GetBuffer(0)))) {
		printf("Query failed %s\n", mysql_error(con));
		AfxMessageBox(_T("请求询问失败"));
	}
	else {
		if (!(res = mysql_store_result(con))) {
			printf("Get result failed %s\n", mysql_error(con));
			AfxMessageBox(_T("获取数据集失败"));
		}
		else {
			row = mysql_fetch_row(res);
			CString realPassword;
			realPassword = (char*)row[0];
			if (realPassword == userPassword) {
				flag = TRUE;
			}
		}
	}

	mysql_free_result(res);  //释放一个结果集合使用的内存。
	mysql_close(con);	 //关闭一个服务器连接。

	return flag;
}

BOOL CVLC1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//设置进度条范围
	m_SliderPlayer.SetRange(0, 1000);
	m_SliderPlayer.SetPos(0);

	m_SliderVolume.SetRange(0, 100);
	m_SliderVolume.SetPos(100);
	m_List_File.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List_File.InsertColumn(0, _T("文件名"), LVCFMT_CENTER, 120);
	m_List_File.InsertColumn(1, _T("路径"), LVCFMT_CENTER, 140);
	//新建vlc实例
	m_vlcinst = libvlc_new(0, NULL);
	//获取控件句柄，设置初始状态
	((CButton*)GetDlgItem(IDC_ISRTSP))->SetCheck(TRUE);
	GetDlgItem(IDC_SELECT_VIDEO)->EnableWindow(TRUE);
	GetDlgItem(IDC_PLAYTIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_PAUSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_LIVE)->EnableWindow(FALSE);
	
	CString Stream_Name[] = {
		_T("本地直播"),
		_T("OBS录屏"), 
		_T("BigBuckBunny_115k.mov"), 
		_T("CCTV1直播"), 
		_T("CCTV3高清"),
		_T("CCTV5高清"),
		_T("CCTV6高清")};
	CString Stream_URL[] = { 
		_T("udp://@224.0.0.1:6666"),
		_T("udp://@224.0.0.1:9999"), 
		_T("rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov"), 
		_T("http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8"),
		_T("http://ivi.bupt.edu.cn/hls/cctv3hd.m3u8"),
		_T("http://ivi.bupt.edu.cn/hls/cctv5hd.m3u8"),
		_T("http://ivi.bupt.edu.cn/hls/cctv6hd.m3u8")};
	for (int i = 0; i < 7; i++)
	{
		m_List_File.InsertItem(0, _T(""));
		m_List_File.SetItemText(0, 0, Stream_Name[i]);
		m_List_File.SetItemText(0, 1, Stream_URL[i]);
	}
	
	Now_Time = "--:--";
	Tot_Time = "--:--";
	UpdateData(FALSE);
	
	while (TRUE) {
		CLogin login_Dlg;

		int nReturn = login_Dlg.DoModal();
		if (nReturn == IDCANCEL) {
			return FALSE;
		}
		if (nReturn == IDOK) {
			HWND login_Hwnd = GetDlgItem(IDD_DIALOG11)->GetSafeHwnd();
			CString str_User = login_Dlg.m_strUser;
			CString str_Password = login_Dlg.m_strPassword;
			BOOL flag = FALSE;
			flag = contectToDataBase(str_User, str_Password);
			if (!flag) {
				if (IDOK == MessageBox(_T("用户ID或密码错误！\n请重新输入！"), _T("登陆失败"), MB_OKCANCEL | MB_ICONERROR))
					continue;
			}
			else return TRUE;
		}
		else break;
	}
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVLC1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVLC1Dlg::OnPaint()
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
	//设置播放窗口为黑色
#if 1
	CBrush* oldBrush, brNew;
	HWND hWnd = GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd();
	brNew.CreateSolidBrush(RGB(0, 0, 0));
	HDC hDC = ::GetDC(hWnd);
	CDC* pDC = CDC::FromHandle(hDC);
	oldBrush = pDC->SelectObject(&brNew);

	CRect rect2;
	::GetClientRect(hWnd, &rect2);
	pDC->Rectangle(rect2);
	pDC->SelectObject(oldBrush);
#endif
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVLC1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CVLC1Dlg::OnClickedPause()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_vlcMplay != NULL) libvlc_media_player_pause(m_vlcMplay);
}

char* EncodeToUTF8(const wchar_t* mbcsStr)
{
	//wchar_t* wideStr;
	char* utf8Str;
	int charLen;
	//赋值宽字符字符串
	//charLen = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)mbcsStr, -1, NULL, 0);
	//wideStr = (wchar_t*)malloc(sizeof(wchar_t)* charLen);
	//MultiByteToWideChar(CP_ACP, 0, (LPCCH)mbcsStr, -1, wideStr, charLen);
	//赋值utf8字符串
	charLen = WideCharToMultiByte(CP_UTF8, 0, mbcsStr, -1, NULL, 0, NULL, NULL);
	utf8Str = (char*)malloc(charLen);
	WideCharToMultiByte(CP_UTF8, 0, mbcsStr, -1, utf8Str, charLen, NULL, NULL);
	//free(wideStr);
	
	return utf8Str;
}





void CVLC1Dlg::OnClickedPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_VideoPath == "")
	{
		return;
	}
	//先释放就的视频资源
	OnClickedStop();
	//为新视频
	if (m_bNew == true)
	{
		if (m_vlcMplay != NULL)
		{
			//释放之前的视频资源
			libvlc_media_player_release(m_vlcMplay);
		}
		//判断播放的是本地MP4还是rtsp流
		if (m_bIsRtsp == false)
		{

			m_vlcMedia = libvlc_media_new_path(m_vlcinst, EncodeToUTF8(m_VideoPath.GetBuffer(0)));
			//m_vlcMedia = libvlc_media_new_path(m_vlcinst, (m_VideoPath.c_str())));
		}
		else
		{
			char* tmp = EncodeToUTF8(m_VideoPath.GetBuffer(0));
			printf("%s\n", tmp);
			//设置新的rtsp流地址，并将其绑定到m_vlcinst这个实例上
			m_vlcMedia = libvlc_media_new_location(m_vlcinst, EncodeToUTF8(m_VideoPath.GetBuffer(0)));
		}
		
		m_vlcMplay = libvlc_media_player_new_from_media(m_vlcMedia);
		HWND hWnd = GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd();
		libvlc_media_player_set_hwnd(m_vlcMplay, hWnd);
		libvlc_media_release(m_vlcMedia);
		m_bNew = false;
	}


	libvlc_media_player_play(m_vlcMplay);
	//Sleep(3000);
	if (m_bIsRtsp == false)
	{
		m_TotalTime = libvlc_media_player_get_length(m_vlcMplay);
		m_playerTimer = SetTimer(100, 1 * 100, NULL);
	}
}





void CVLC1Dlg::OnClickedStop()
{
	//直接结束视频播放
	//无视频在播放
	if (m_vlcMplay == NULL)
	{
		return;
	}
	libvlc_media_player_stop(m_vlcMplay);
	m_bNew = true;
	m_vlcMplay = NULL;

	if (m_vlcMedia != NULL)
	{
		libvlc_media_release(m_vlcMedia);
	}
	if (m_vlcMplay != NULL)
	{
		libvlc_media_player_release(m_vlcMplay);
	}
	KillTimer(100);
	m_SliderPlayer.SetPos(0);
	OnPaint();
}


 
void CVLC1Dlg::OnClickedButtonTest()
{
	//libvlc_instance_t* vlc;
	const char* url;

	/*
	const char* sout = "#transcode{vcodec=h264,fps=25,venc=x264{preset=ultrafast,"\
		"profile=baseline,tune=zerolatency},vb=512,"                              \
		"acodec=mpga,ab=64,channels=2}"                                           \
		":duplicate{dst=display,dst=standard{access=udp,mux=ts,dst=224.0.0.1:6666}}";
	
	*/

	const char* sout = "#transcode{vcodec=h264,acodec=mpga,ab=128,channels=2,samplerate=44100,scodec=none}:rtp{sdp=rtsp://:8554/10.80.171.198}";
	const char* media_name = "Lei's test";

	//Screen Capture
	url = "screen:\\";
	//url = "big_buck_bunny.mp4";
	//url = "Sacred world.flv";

	//vlc = libvlc_new(0, NULL);

	libvlc_vlm_add_broadcast(m_vlcinst, media_name, url, sout, 0, NULL, true, false);
	
	libvlc_vlm_play_media(m_vlcinst, media_name);

	//play 30s
	Sleep(30000);

	libvlc_vlm_stop_media(m_vlcinst, media_name);
	libvlc_vlm_release(m_vlcinst);
}


void CVLC1Dlg::OnClickedSelectVideo()
{
	TCHAR BASED_CODE szFilter[] = _T("Video Files All Files (*.*)|*.*||");
	//选择文件对话框
	CFileDialog fileDlg(TRUE, _T("Video file"), _T(""), OFN_FILEMUSTEXIST, szFilter, this);
	if (fileDlg.DoModal() == IDOK)
	{
		m_VideoPath = fileDlg.GetPathName();
		m_bNew = true;
	}
	UpdateData(FALSE);
}


void CVLC1Dlg::OnChangeVideoPath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	m_bNew = true;
}


void CVLC1Dlg::OnClickedIsrtsp()
{
	if (TRUE == ((CButton*)GetDlgItem(IDC_ISRTSP))->GetCheck())
	{
		GetDlgItem(IDC_SELECT_VIDEO)->EnableWindow(false);
		GetDlgItem(IDC_PLAYTIME)->EnableWindow(false);
		GetDlgItem(IDC_PAUSE)->EnableWindow(false);
		GetDlgItem(IDC_GoBack)->EnableWindow(false);
		GetDlgItem(IDC_FastForward)->EnableWindow(false);
		m_bIsRtsp = true;
	}
	else
	{
		GetDlgItem(IDC_SELECT_VIDEO)->EnableWindow(true);
		GetDlgItem(IDC_PLAYTIME)->EnableWindow(true);
		GetDlgItem(IDC_PAUSE)->EnableWindow(true);
		GetDlgItem(IDC_GoBack)->EnableWindow(true);
		GetDlgItem(IDC_FastForward)->EnableWindow(true);
		m_bIsRtsp = false;
	}
}


void CVLC1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 进度条拖动事件
	if (pScrollBar->GetSafeHwnd() == m_SliderPlayer.GetSafeHwnd() && m_vlcMplay != NULL)
	{
		libvlc_media_player_pause(m_vlcMplay);
		libvlc_time_t m_CurTime;
		m_TotalTime = libvlc_media_player_get_length(m_vlcMplay);

		double pos = m_SliderPlayer.GetPos() / 1000.0;
		m_CurTime = (libvlc_time_t)(m_TotalTime * pos);
		libvlc_media_player_set_time(m_vlcMplay, m_CurTime);
		libvlc_media_player_play(m_vlcMplay);
	}
	else if (pScrollBar->GetSafeHwnd() == m_SliderVolume.GetSafeHwnd() && m_vlcMplay != NULL)
	{
		int iVolume = m_SliderVolume.GetPos();
		libvlc_audio_set_volume(m_vlcMplay, iVolume);
	}
	else
	{
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}
	
}


void CVLC1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 100)
	{
		//libvlc_media_parse(m_vlcMedia);
		libvlc_time_t m_CurTime = libvlc_media_player_get_time(m_vlcMplay);
		m_TotalTime = libvlc_media_player_get_length(m_vlcMplay);
		int m_npos = (int)(m_CurTime * 1000 / m_TotalTime);
		//double m_npos = 300;
		m_SliderPlayer.SetPos(m_npos);
		int tmp = (int)(m_TotalTime / 1000);
		int tmp_cur = int(libvlc_media_player_get_time(m_vlcMplay)) / 1000;
		Tot_Time.Format(_T("%02d:%02d"), tmp / 60, tmp % 60);
		Now_Time.Format(_T("%02d:%02d"), tmp_cur / 60, tmp_cur % 60);
		UpdateData(FALSE);
	}
	if (libvlc_Ended == libvlc_media_player_get_state(m_vlcMplay)) OnClickedStop();
	

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CVLC1Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}




void CVLC1Dlg::OnClickedGoback()//后退五秒
{
	libvlc_media_player_pause(m_vlcMplay);
	libvlc_time_t m_CurTime;
	m_TotalTime = libvlc_media_player_get_length(m_vlcMplay);
	double pos = m_SliderPlayer.GetPos() / 1000.0;
	m_CurTime = (libvlc_time_t)max(0,(m_TotalTime * pos-5000));
	libvlc_media_player_set_time(m_vlcMplay, m_CurTime);
	libvlc_media_player_play(m_vlcMplay);
}


void CVLC1Dlg::OnClickedFastforward()//前进五秒
{
	// TODO: 在此添加控件通知处理程序代码
	libvlc_media_player_pause(m_vlcMplay);
	libvlc_time_t m_CurTime;
	m_TotalTime = libvlc_media_player_get_length(m_vlcMplay);
	double pos = m_SliderPlayer.GetPos() / 1000.0;
	m_CurTime = (libvlc_time_t)min(m_TotalTime, (m_TotalTime * pos + 5000));
	libvlc_media_player_set_time(m_vlcMplay, m_CurTime);
	libvlc_media_player_play(m_vlcMplay);
}


void CVLC1Dlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CVLC1Dlg::OnClickedAddbut()//给listBox添加文件
{
	if (m_bIsRtsp)
	{
		m_List_File.InsertItem(0, _T(""));
		m_List_File.SetItemText(0, 0, m_VideoPath);
		m_List_File.SetItemText(0, 1, m_VideoPath);
		UpdateData(FALSE);
	}
	else
	{
	// TODO: 在此添加控件通知处理程序代码
		TCHAR BASED_CODE szFilter[] = _T("Video Files All Files (*.*)|*.*||");
		//选择文件对话框
		CFileDialog fileDlg(TRUE, _T("Video file"), _T(""), OFN_FILEMUSTEXIST, szFilter, this);
		if (fileDlg.DoModal() == IDOK)
		{
			m_VideoPath = fileDlg.GetPathName();
			m_bNew = true;
			m_List_File.InsertItem(0, _T(""));
			CString V_Name = _T("");
			int len = m_VideoPath.ReverseFind('\\');
			V_Name = m_VideoPath.Right(m_VideoPath.GetLength() - len - 1);
			m_List_File.SetItemText(0, 0, V_Name);
			m_List_File.SetItemText(0, 1, m_VideoPath);
		}
		UpdateData(FALSE);
	}
}


void CVLC1Dlg::OnClickedDelbut()//删除文件列表中的文件
{
	CString str;
	int CurSel = m_List_File.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (CurSel >= 0)
	{
		str = m_List_File.GetItemText(CurSel, 3);
		DeleteFile(str);
		m_List_File.DeleteItem(CurSel);
	}
}




void CVLC1Dlg::OnClickedButtonPlaysel()
{
	POSITION pos = m_List_File.GetFirstSelectedItemPosition();
	CString str;
	int CurSel = m_List_File.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	m_VideoPath= m_List_File.GetItemText(CurSel, 1);
	TRACE0(m_List_File.GetItemText(CurSel, 1));
	UpdateData(FALSE);
}


void CVLC1Dlg::OnClickedButtonClr()
{
	m_List_File.DeleteAllItems();
	UpdateData(FALSE);
}


void CVLC1Dlg::OnBnClickedPlayFlv()
{
	UpdateData(TRUE);
	TCHAR BASED_CODE szFilter[] = _T("FLV (*.flv)|*.flv||");
	
	CFileDialog fileDlg(TRUE, _T("Video file"), _T(""), OFN_FILEMUSTEXIST, szFilter, this);
	if (fileDlg.DoModal() == IDOK)
	{
		m_VideoPath = fileDlg.GetPathName();
		m_VideoPath = _T("file:///") + m_VideoPath;
		m_bNew = true;
	}
	UpdateData(FALSE);
}


void CVLC1Dlg::OnBnClickedStartLive()
{
	//开始直播
	//libvlc_instance_t* vlc;
	const char* url;

	const char* sout = "#transcode{vcodec=h264,fps=25,venc=x264{preset=ultrafast,"\
		"profile=baseline,tune=zerolatency},vb=512,"                              \
		"acodec=mpga,ab=64,channels=2}"                                           \
		":duplicate{dst=display,dst=standard{access=udp,mux=ts,dst=224.0.0.1:6666}}";
	const char* media_name = "Lei's test";

	//获得视频地址
	UpdateData(true);
	if (m_VideoPath == "") return;


	//Screen Capture
	//url = "screen://";
	url = EncodeToUTF8(m_VideoPath.GetBuffer(0));
	//AfxMessageBox(m_VideoPath);
	//Sleep(3000);
	//url = "Sacred world.flv";
	
	//vlc = libvlc_new(0, NULL);
	libvlc_vlm_add_broadcast(m_vlcinst, media_name, url, sout, 0, NULL, true, false);
	libvlc_vlm_play_media(m_vlcinst, media_name);

	GetDlgItem(IDC_START_LIVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_LIVE)->EnableWindow(TRUE);
	AfxMessageBox(m_VideoPath);
	UpdateData(false);
	m_isLive = TRUE;
	//play 30s
	//Sleep(30000);
	
	
	
}


void CVLC1Dlg::OnBnClickedStopLive()
{
	m_isLive = FALSE;
	libvlc_vlm_stop_media(m_vlcinst, "Lei's test");
	libvlc_vlm_release(m_vlcinst);
	UpdateData(true);
	GetDlgItem(IDC_START_LIVE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_LIVE)->EnableWindow(FALSE);
	UpdateData(false);
}
