//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2020 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "ArxDbgDocumentReactor.h"



ACRX_NO_CONS_DEFINE_MEMBERS(ArxDbgDocumentReactor, AcApDocManagerReactor);


ArxDbgDocumentReactor::ArxDbgDocumentReactor()
{
    acDocManager->addReactor(this);
}


ArxDbgDocumentReactor::~ArxDbgDocumentReactor()
{
    acDocManager->removeReactor(this);
}



void
ArxDbgDocumentReactor::documentCreateStarted(AcApDocument* doc)
{
    printReactorMessage(doc, _T("Document Create Started"));
}


void
ArxDbgDocumentReactor::documentCreated(AcApDocument* doc)
{
    printReactorMessage(doc, _T("Document Created"));
}


void
ArxDbgDocumentReactor::documentToBeDestroyed(AcApDocument* doc)
{
    printReactorMessage(doc, _T("Document To Be Destroyed"));
}


void
ArxDbgDocumentReactor::documentCreateCanceled(AcApDocument* doc)
{
    printReactorMessage(doc, _T("Document Create Canceled"));
}


void
ArxDbgDocumentReactor::documentLockModeWillChange(AcApDocument* doc,
    AcAp::DocLockMode myCurrentMode,
    AcAp::DocLockMode myNewMode,
    AcAp::DocLockMode currentMode,
    const TCHAR* pGlobalCmdName)
{
    printReactorMessage(doc, _T("Document Lock Mode Will Change"));

}

void
ArxDbgDocumentReactor::documentLockModeChangeVetoed(AcApDocument* doc,
    const TCHAR* pGlobalCmdName)
{
    CString msg;
    msg.Format(_T("Document Lock Mode Change Vetoed, CmdName = %s"), pGlobalCmdName);
    printReactorMessage(doc, msg);
}



void
ArxDbgDocumentReactor::documentLockModeChanged(AcApDocument* doc,
    AcAp::DocLockMode myPreviousMode,
    AcAp::DocLockMode myCurrentMode,
    AcAp::DocLockMode currentMode,
    const TCHAR* pGlobalCmdName)
{
    printReactorMessage(doc, _T("Document Lock Mode Changed"));
}



void
ArxDbgDocumentReactor::documentBecameCurrent(AcApDocument* doc)
{
    if (doc)
        printReactorMessage(doc, _T("Document Became Current"));
}



void
ArxDbgDocumentReactor::documentToBeActivated(AcApDocument* doc)
{
    printReactorMessage(doc, _T("Document To Be Activated"));
}


void
ArxDbgDocumentReactor::documentToBeDeactivated(AcApDocument* doc)
{
    printReactorMessage(doc, _T("Document To Be De-activated"));
}




void
ArxDbgDocumentReactor::documentActivated(AcApDocument* doc)
{
    if (doc)
        printReactorMessage(doc, _T("Document Activated"));
}


void
ArxDbgDocumentReactor::printReactorMessage(AcApDocument* doc, LPCTSTR event) const
{
    CString str2;
    str2.Format(_T("<DOC REACTOR: %s>"), doc->docTitle());

    acutPrintf(_T("\n%-15s : [%s] "), str2, event);
}



