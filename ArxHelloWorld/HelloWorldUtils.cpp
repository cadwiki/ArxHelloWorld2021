#include "StdAfx.h"
#include "printButton.h"
#include "acdb.h"

using namespace System;
using namespace System::Net;
using namespace System::Windows::Forms;

class HelloWorldUtils {

	public: static void PrintHelloWorlds() {
		AfxMessageBox(L"mfc hello");
		MessageBox::Show(".net hello world");
		acutPrintf(L"autocad command line hello");
		acedAlert(L"autocad alert hello");
	}

	static void ShowDialog() {
		printButton _dialog;
		_dialog.DoModal();
	}
};