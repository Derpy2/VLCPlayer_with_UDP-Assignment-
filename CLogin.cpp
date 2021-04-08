// CLogin.cpp: 实现文件
//

#include "pch.h"
#include "VLC1.h"
#include "CLogin.h"
#include "afxdialogex.h"


// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG11, pParent)
	, m_strUser(_T(""))
	, m_strPassword(_T(""))
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_PASSWORD, m_strUser);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_USERNAME, m_strUser);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
END_MESSAGE_MAP()


// CLogin 消息处理程序
