// $Id$
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B9C0EB9A_AD28_11D3_BF1F_00C0F03C565A__INCLUDED_)
#define AFX_STDAFX_H__B9C0EB9A_AD28_11D3_BF1F_00C0F03C565A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers

#if !defined (ACE_HAS_MFC) || (ACE_HAS_MFC == 0)
#error You must build your ACE/TAO with MFC support (by defining"ACE_HAS_MFC=1")
#endif

#include "tao/corba.h"

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__B9C0EB9A_AD28_11D3_BF1F_00C0F03C565A__INCLUDED_)
