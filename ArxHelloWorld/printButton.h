#pragma once

#include "StdAfx.h"
#include "resource.h"
// printButton dialog

class printButton : public CDialog
{
	DECLARE_DYNAMIC(printButton)

public:
	printButton(CWnd* pParent = nullptr);   // standard constructor
	virtual ~printButton();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
