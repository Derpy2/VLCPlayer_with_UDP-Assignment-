
// VLC1Dlg.h: 头文件
//
#if !defined(AFX_VLCDEMODLG_H__27B40B69_F36F_4AA6_BB6D_47B061A32F9A__INCLUDED_)
#define AFX_VLCDEMODLG_H__27B40B69_F36F_4AA6_BB6D_47B061A32F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif

#include "vlc/vlc.h"
#include "mysql.h"
//#pragma comment(lib,"libvlc.lib")
//#pragma comment(lib,"libvlccore.lib")
// CVLC1Dlg 对话框
class CVLC1Dlg : public CDialogEx
{
// 构造
public:
	CVLC1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VLC1_DIALOG };
#endif

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
	afx_msg void OnClickedPause();
	afx_msg void OnClickedPlay();
	afx_msg void OnClickedStop();
	afx_msg void OnClickedButtonTest();
	afx_msg void OnClickedSelectVideo();
	afx_msg void OnChangeVideoPath();
	afx_msg void OnClickedIsrtsp();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	bool m_bNew;
	libvlc_instance_t* m_vlcinst;
	libvlc_media_player_t* m_vlcMplay;
	libvlc_media_t* m_vlcMedia;
	libvlc_time_t m_TotalTime;
	BOOL m_isRTSP;
	CSliderCtrl m_SliderPlayer;
	CString m_VideoPath;
	CSliderCtrl m_SliderVolume;
	UINT_PTR m_playerTimer;
	bool m_bIsRtsp;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnClickedGoback();
	afx_msg void OnClickedFastforward();
	CString Now_Time;
	CString Tot_Time;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnClickedAddbut();
	afx_msg void OnClickedDelbut();
	CListCtrl m_List_File;
	afx_msg void OnClickedButtonPlaysel();
	afx_msg void OnClickedButtonClr();
	afx_msg void OnBnClickedPlayFlv();
	afx_msg void OnBnClickedStartLive();
	afx_msg void OnBnClickedStopLive();
	bool m_isLive;
};
#endif