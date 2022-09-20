#include "StdAfx.h"
#include "resource.h"
#include "printButton.h"
#include "acdb.h"
#include "ArxDbgDocumentReactor.h"
#include <list>

using namespace System;
using namespace System::Net;
using namespace System::Windows::Forms;

class DdModUtils {
	static void DwgSaveLoopPrintDbMod()
	{
		AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
		AcApDocument* doc = acDocManager->curDocument();
		std::list<int> drawingVersions{ 25, 26, 27, 28, 29, 30, 31, 32, (int)Autodesk::AutoCAD::DatabaseServices::DwgVersion::Current };
		CString path = L"C:\\Temp\\";
		CString originalFileName = L"Drawing-saved-v";
		CString fileExtension = L".dwg";
		CString filePath;
		CString versionString;
		for (int versionNumber : drawingVersions) {
			acutPrintf(L"Current db mod: ");
			PrintDbMod();
			filePath = path;
			filePath.Append(originalFileName);
			versionString.Format(_T("%d"), versionNumber);
			filePath.Append(versionString);
			filePath.Append(fileExtension);
			db->saveAs((const TCHAR*)filePath, true, (AcDb::AcDbDwgVersion)versionNumber, 0);
			acdbSetDbmod(acDocManager->curDocument()->database(), 0);  // make sure dbmod stays 0
			acutPrintf(L"New db mod: ");
			PrintDbMod();
			acDocManager->appContextNewDocument(L"acad.dwt");
			acutPrintf(L"New document opened.\n");
		}
	}


	static void PrintDbMod() {
		struct resbuf dbmod;
		acedGetVar(L"DBMOD", &dbmod);
		int idbmod = dbmod.resval.rint;
		CString sdbmod = idbmod.ToString();
		wchar_t* arrayDbMod = T2W(sdbmod.GetBuffer());
		acutPrintf(arrayDbMod);
		acutPrintf(L"\n");
	}
};
