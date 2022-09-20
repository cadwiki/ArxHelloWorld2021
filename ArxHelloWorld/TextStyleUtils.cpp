#include "StdAfx.h"
#include "acdb.h"
using namespace System;


std::string TextStyleUtils::CStringToStdString(CString inputString) {
	CT2CA pszConvertedAnsiString(inputString);
	std::string outputString(pszConvertedAnsiString);
	return outputString;
}

CString TextStyleUtils::GetCurrentTextStyleName()
{
	struct resbuf rb;
	acedGetVar(L"TEXTSTYLE", &rb);
	if (rb.resval.rstring != NULL) {
		// Release memory acquired for string:
		free(rb.resval.rstring);
		return rb.resval.rstring;
	}
	return L"";
}

AcDbTextStyleTable* TextStyleUtils::GetTextStyleTable(AcDbDatabase* db) {
	Acad::ErrorStatus es;
	AcDbTextStyleTable* textStyleTbl;
	es = db->getTextStyleTable(textStyleTbl, AcDb::kForWrite);
	ASSERT(es == Acad::eOk);
	if (es == Acad::eOk) {
		return textStyleTbl;
	}
	return NULL;
}

AcDbSymbolTableRecord* TextStyleUtils::GetSymbolTableRecordByName(const AcDbSymbolTable* symTbl, std::string recordName)
{
	// get an iterator over this symbol Table
	AcDbSymbolTableIterator* tblIter;
	Acad::ErrorStatus es = symTbl->newIterator(tblIter);
	if (symTbl->isKindOf(AcDbLayerTable::desc()))
		static_cast<AcDbLayerTableIterator*>(tblIter)->setSkipHidden(false);
	ASSERT(tblIter != NULL);
	if (es != Acad::eOk) {
		rxErrorMsg(es);
		return NULL;
	}
	// walk table to find record by name
	AcDbSymbolTableRecord* tblRec;
	const TCHAR* symName;
	size_t length;
	std::string currentRecordName;
	for (; !tblIter->done(); tblIter->step()) {
		es = tblIter->getRecord(tblRec, AcDb::kForWrite);
		if (es == Acad::eOk) {
			tblRec->getName(symName);
			size_t length = _tcslen(symName);
			currentRecordName = std::string(symName, symName + length);
			if (currentRecordName.compare(recordName) == 0) {
				return tblRec;
			}
		}
		else
			rxErrorMsg(es);
	}
	delete tblIter;
	return NULL;
}

Double TextStyleUtils::GetTextStyleRecordSize(AcDbTextStyleTableRecord* tblRec) {
	return tblRec->textSize();
}

Boolean TextStyleUtils::SetTextStyleRecordSize(AcDbTextStyleTableRecord* tblRec, double textSize) {
	try {
		tblRec->assertWriteEnabled();
		Double size = 10.1;
		Acad::ErrorStatus es = tblRec->setTextSize(size);
		//Acad::ErrorStatus es = tblRec->setTextSize(textSize);
		if (es == Acad::eOk) {
			return true;
		}
		else {
			rxErrorMsg(es);
			return false;
		}
	}
	catch (int e) {
		CString str;
		str.Format(_T("Error code: %s"), e.ToString());
		acutPrintf(str);
	}
}

void TextStyleUtils::rxErrorMsg(Acad::ErrorStatus msgId)
{
	CString str;

	if (msgId != Acad::eOk) {
		str.Format(_T("ARX ERROR: %s"), acadErrorStatusText(msgId));
		acutPrintf(str);
	}
}