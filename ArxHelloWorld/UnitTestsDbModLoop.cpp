#include "StdAfx.h"
#include <list>
#include <sstream>
#include <acedCmdNF.h> // required for acadCommandS

#define _OPEN_SYS_ITOA_EXT

using namespace NUnit::Framework;

using namespace System;
using namespace System::Net;
using namespace System::Windows::Forms;

using System::IntPtr;
using System::Runtime::InteropServices::Marshal;

namespace Tests {
	[TestFixture]
	public ref class UnitTestsDbModLoop
	{
	public:
		[Test]
		void Test_dbModZero_ShouldPass()
		{

			AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
			AcApDocument* doc = acDocManager->curDocument();
			std::list<int> drawingVersions{ 25, 26, 27, 28, 29, 30, 31, 32, (int)Autodesk::AutoCAD::DatabaseServices::DwgVersion::Current };
			std::list<int> dbModValues;

			CString path = L"C:\\Temp\\";
			CString originalFileName = L"Drawing-saved-v";
			CString fileExtension = L".dwg";
			CString filePath;
			CString versionString;
			for (int versionNumber : drawingVersions) {
				char buffer[sizeof(int) * 8 + 1];
				itoa(versionNumber, buffer, 10);
				std::wstring wString = CharToWString(buffer);
				const ACHAR* aCharStar = wString.c_str();
				acutPrintf(L"\n----------Loop iteration started----------\n");
				acutPrintf(L"\nDrawing version number for saving later: ");
				acutPrintf(L"\n");
				acutPrintf(aCharStar);
				acutPrintf(L"\n");

				acutPrintf(L"\nCurrent db mod before making changes: ");
				PrintDbMod();
				DrawCircle();
				acutPrintf(L"\nNew db mod after drawing circle: ");
				PrintDbMod();
				filePath = path;
				filePath.Append(originalFileName);
				versionString.Format(_T("%d"), versionNumber);
				filePath.Append(versionString);
				filePath.Append(fileExtension);
				SetDbMod(0);
				acutPrintf(L"\nNew db mod after attempting to set dbmod to 0: ");
				PrintDbMod();
				db->saveAs((const TCHAR*)filePath, true, (AcDb::AcDbDwgVersion)versionNumber, 0);

				db->saveAs((const TCHAR*)filePath, true, (AcDb::AcDbDwgVersion)versionNumber, 0);
				db->save();
				// look into how qsave working with Options menu
				// look into saving to current 21 version, and then backsaving

				//make sure DBMOD is set to 0 after saving to ANY version
				acedCommandS(RTSTR, _T("qsave"), RTNONE);

				acutPrintf(L"\nNew db mod after saving document: ");
				PrintDbMod();
				SetDbMod(0);
				acutPrintf(L"\nNew db mod after attempting to set dbmod to 0: ");
				PrintDbMod();
				int actualDbMod = GetDbMod();
				dbModValues.push_back(actualDbMod);
				acDocManager->appContextNewDocument(L"acad.dwt");
				acutPrintf(L"\nOpened new document........\n");
				acutPrintf(L"\n----------Loop iteration complete----------\n");
			}

			int i = 0;
			std::list<int>::iterator it = dbModValues.begin();
			std::advance(it, i);
			for (int versionNumber : drawingVersions) {
				int actualDbMod = *it;
				Assert::AreEqual(0, actualDbMod);
				i = i + 1;
				auto it1 = std::next(it, i);
			}
			
		}

		void SetDbMod(int newValue)
		{
			acdbSetDbmod(acDocManager->curDocument()->database(), newValue);
			acDocManager->curDocument()->pushDbmod();
		}

		static std::wstring CharToWString(char charArray[]) {
			std::wstringstream stringStream;
			stringStream << charArray;
			std::wstring output = stringStream.str();
			return output;
		}

		static void DrawCircle() {
			AcDbBlockTableRecord* pBlock = new AcDbBlockTableRecord();
			
			AcDbObjectId* id = new AcDbObjectId();
			Acad::ErrorStatus es;
			AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
			AcDbBlockTableRecord* modelSpace = getModelSpace(AcDb::kForWrite);

			es = acdbOpenObject(pBlock, modelSpace->id(), AcDb::kForWrite);
			modelSpace->close();

			AcGePoint3d* startPoint = new AcGePoint3d(1.0, 1.0, 1.0);
			es = AddCircle(*startPoint, 5.0, *id, pBlock);

			es = Acad::ErrorStatus::eOk;
		}


		static Acad::ErrorStatus AddCircle(AcGePoint3d center, double r, AcDbObjectId& id, AcDbBlockTableRecord* pBlock)
		{
			AcDbCircle* pCirEnt = NULL;
			pCirEnt = new AcDbCircle();
			pCirEnt->setCenter(center);
			pCirEnt->setRadius(r);
			Acad::ErrorStatus es = Add(pCirEnt, pBlock);
			es = pCirEnt->close();
			id = pCirEnt->objectId();
			return es;
		}


		static Acad::ErrorStatus Add(AcDbEntity* pEnt, AcDbBlockTableRecord* Blok)
		{
			if (!pEnt) {
				return Acad::eNullEntityPointer;
			}

			Acad::ErrorStatus es;
			if (!Blok)
			{
				Blok = getModelSpace(AcDb::kForWrite);
				if (!Blok) return Acad::eInvalidOwnerObject;
			}
			if (Blok->isWriteEnabled() == Adesk::kFalse)
			{
				AcDbObject* pObj = NULL;
				es = acdbOpenObject(pObj, Blok->objectId(), AcDb::kForWrite);
				Blok = AcDbBlockTableRecord::cast(pObj);
			}
			if ((es = Blok->appendAcDbEntity(pEnt)) != Acad::eOk)  // eAlreadyInDb = wcześniej wstawione do innego bloku.
			{
				Blok->close();
				return es;
			}
			Blok->close();
			return Acad::eOk;
		}

		static AcDbBlockTableRecord* getModelSpace(AcDb::OpenMode mode)
		{
			AcDbBlockTableRecord* Blok = NULL;
			Acad::ErrorStatus es;
			AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
			if (!pDb) return NULL;
			AcDbBlockTable* pTbl = NULL;
			if ((es = pDb->getBlockTable(pTbl, AcDb::kForRead)) != Acad::eOk)
				return NULL;
			if ((es = pTbl->getAt(ACDB_MODEL_SPACE, Blok, mode)) != Acad::eOk)
			{
				pTbl->close();
				return NULL;
			}
			pTbl->close();

			return Blok;
		}


		static int GetDbMod() {
			struct resbuf dbmod;
			acedGetVar(L"DBMOD", &dbmod);
			int iDbMod = dbmod.resval.rint;
			return iDbMod;
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



}
