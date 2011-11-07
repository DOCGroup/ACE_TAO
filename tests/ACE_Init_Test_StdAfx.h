/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ACE_Init_Test_StdAfx.h
 *
 *  $Id$
 *
 *  stdafx.h : include file for standard system include files,
 *  or project specific include files that are used frequently, but
 *  are changed infrequently
 *
 *
 *  @author Steve Huston <shuston@cs.wustl.edu>
 */
//=============================================================================


#ifndef AFX_STDAFX_H__64FDCA02_F7F9_11D2_89B6_00A024CC68DB__INCLUDED_
#define AFX_STDAFX_H__64FDCA02_F7F9_11D2_89B6_00A024CC68DB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN

// MFC core and standard components
#include <afxwin.h>
// MFC extensions
#include <afxext.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
// MFC support for Windows Common Controls
#include <afxcmn.h>
#endif /* _AFX_NO_AFXCMN_SUPPORT */

//{{AFX_INSERT_LOCATION}} Microsoft Developer Studio will insert
//additional declarations immediately before the previous line.

#endif /* !defined(AFX_STDAFX_H__64FDCA02_F7F9_11D2_89B6_00A024CC68DB__INCLUDED_) */
