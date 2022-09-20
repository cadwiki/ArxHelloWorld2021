#include "StdAfx.h"
using namespace NUnit::Framework;

namespace Tests {
	[TestFixture]
	public ref class UnitTestsDocument
	{
	public:
		[Test]
		void Test_setSDIToZero_ShouldReturnZero()
		{
			// set sdi to zero
			struct resbuf sdiZero;
			sdiZero.restype = RTSHORT;
			sdiZero.resval.rint = 0;
			acedSetVar(L"SDI", &sdiZero);
			// get current sdi value
			struct resbuf sdiValue;
			acedGetVar(L"SDI", &sdiValue);
			int intSdi = sdiValue.resval.rint;
			CString stringSDI = intSdi.ToString();
			wchar_t* arraySdi = T2W(stringSDI.GetBuffer());
			// assert current sdi value is zero
			Assert::AreEqual(0, intSdi);
		}

		[Test]
		void Test_setCurrentTextStyleHeightTo10_ShouldSetCurrentTextStyleHeightTo10()
		{
			AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
			CString currentTextStyleName = TextStyleUtils::GetCurrentTextStyleName();
			AcDbTextStyleTable* textStyleTbl = TextStyleUtils::GetTextStyleTable(db);
			AcDbSymbolTableRecord* smybolTableRecord = TextStyleUtils::GetSymbolTableRecordByName(textStyleTbl, TextStyleUtils::CStringToStdString(currentTextStyleName));
			AcDbTextStyleTableRecord* textStyleRecord = AcDbTextStyleTableRecord::cast(smybolTableRecord);
			Double textSize = TextStyleUtils::GetTextStyleRecordSize(textStyleRecord);
			TextStyleUtils::SetTextStyleRecordSize(textStyleRecord, 10.0);
			textSize = TextStyleUtils::GetTextStyleRecordSize(textStyleRecord);
			textStyleTbl->close();
			smybolTableRecord->close();
			textStyleRecord->close();
			// assert current sdi value is zero
			Assert::AreEqual(10.0, textSize);
		}
	};



}
