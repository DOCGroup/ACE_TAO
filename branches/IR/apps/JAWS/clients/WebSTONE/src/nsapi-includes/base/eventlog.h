/*
 * Copyright (c) 1994, 1995.  Netscape Communications Corporation.  All
 * rights reserved.
 *
 * Use of this software is governed by the terms of the license agreement for
 * the Netscape Communications or Netscape Comemrce Server between the
 * parties.
 */


/* ------------------------------------------------------------------------ */


// EVENTLOG.H
//
// This file contains the defines that make NT an installable service.
//
// 1/12/95 aruna
//

// Functions in eventlog.c

VOID InitializeAdminLogging(PCHAR ServiceName, PCHAR MessageFile);
VOID InitializeHttpdLogging(PCHAR ServiceName, PCHAR MessageFile);
VOID InitializeHttpsLogging(PCHAR ServiceName, PCHAR MessageFile);

VOID TerminateAdminLogging();
VOID TerminateHttpdLogging();
VOID TerminateHttpsLogging();

VOID LogErrorEvent(PCHAR ServiceName, WORD fwEventType, WORD fwCategory, DWORD IDEvent,
        LPTSTR chMsg, LPTSTR lpszMsg);

// Functions in error.c

VOID ReportError(PCHAR ErrorMsg);

BOOL CALLBACK ErrorDialogProc(
        HWND hDlg,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
        );















