#pragma once
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
#include "resource.h"
#include "printButton.h"
#include "acdb.h"

#ifndef ARXDBGDOCUMENTREACTOR_H
#define ARXDBGDOCUMENTREACTOR_H

/****************************************************************************
**
**  CLASS ArxDbgDocumentReactor:
**
**  **jma
**
*************************************/

class ArxDbgDocumentReactor : public AcApDocManagerReactor {

public:
    ACRX_DECLARE_MEMBERS(ArxDbgDocumentReactor);

    // constructor/destructors
    ArxDbgDocumentReactor();
    virtual         ~ArxDbgDocumentReactor();

    // messages that are sent by notification
    virtual void	documentCreateStarted(AcApDocument* doc);
    virtual void	documentCreated(AcApDocument* doc);
    virtual void	documentToBeDestroyed(AcApDocument* doc);
    virtual void	documentCreateCanceled(AcApDocument* doc);

    virtual void    documentLockModeWillChange(AcApDocument* doc,
        AcAp::DocLockMode myCurrentMode,
        AcAp::DocLockMode myNewMode,
        AcAp::DocLockMode currentMode,
        const TCHAR* pGlobalCmdName);
    virtual void    documentLockModeChangeVetoed(AcApDocument* doc,
        const TCHAR* pGlobalCmdName);
    virtual void    documentLockModeChanged(AcApDocument* doc,
        AcAp::DocLockMode myPreviousMode,
        AcAp::DocLockMode myCurrentMode,
        AcAp::DocLockMode currentMode,
        const TCHAR* pGlobalCmdName);

    virtual void    documentBecameCurrent(AcApDocument* doc);

    virtual void    documentToBeActivated(AcApDocument* doc);
    virtual void    documentToBeDeactivated(AcApDocument* doc);
    virtual void	documentActivated(AcApDocument* pActivatedDoc);


private:
    // data members
    bool	m_showDetails;

    // helper functions
    void		printReactorMessage(AcApDocument* doc, LPCTSTR event) const;

    // outlawed functions
    ArxDbgDocumentReactor(const ArxDbgDocumentReactor& copy);
    ArxDbgDocumentReactor& operator=(const ArxDbgDocumentReactor& copy);
};

#endif    // ARXDBGDOCUMENTREACTOR_H
