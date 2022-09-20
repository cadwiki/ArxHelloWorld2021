#pragma once
#include <arxHeaders.h>
using namespace System;

class TextStyleUtils
{
public:
	static std::string CStringToStdString(CString inputString);

	static  CString GetCurrentTextStyleName();

	static  AcDbTextStyleTable* GetTextStyleTable(AcDbDatabase* db);

	static  AcDbSymbolTableRecord* GetSymbolTableRecordByName(const AcDbSymbolTable* symTbl, std::string recordName);

	static Double GetTextStyleRecordSize(AcDbTextStyleTableRecord* tblRec);

	static Boolean SetTextStyleRecordSize(AcDbTextStyleTableRecord* tblRec, double textSize);

	static void rxErrorMsg(Acad::ErrorStatus msgId);
};