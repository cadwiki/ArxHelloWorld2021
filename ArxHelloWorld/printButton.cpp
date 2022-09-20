// printButton.cpp : implementation file
//

//#include "pch.h"
#include "StdAfx.h"
#include "printButton.h"
#include "afxdialogex.h"



// printButton dialog

IMPLEMENT_DYNAMIC(printButton, CDialog)

printButton::printButton(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_FORMVIEW, pParent)
{

}

printButton::~printButton()
{
}

void printButton::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(printButton, CDialog)
END_MESSAGE_MAP()


// printButton message handlers
