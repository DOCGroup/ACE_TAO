// $Id$

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7B1C2A48_7230_11D3_B94A_0010A4052E2A__INCLUDED_)
#define AFX_STDAFX_H__7B1C2A48_7230_11D3_B94A_0010A4052E2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// We must include at least one ACE/TAO header before the MFC/Windows headers
// because of the way windows.h is included.  So this must go before the MFC
// headers.

#include "tao\corba.h"
#include "ace\Thread_Manager.h"

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7B1C2A48_7230_11D3_B94A_0010A4052E2A__INCLUDED_)
