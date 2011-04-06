/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ACE_Init_Test.h
 *
 *  $Id$
 *
 * Main header file for the ACE_INIT_TEST application.
 *
 *
 *  @author Steve Huston <shuston@cs.wustl.edu>
 */
//=============================================================================


#ifndef AFX_ACE_INIT_TEST_H__64FDC9FE_F7F9_11D2_89B6_00A024CC68DB__INCLUDED_
#define AFX_ACE_INIT_TEST_H__64FDC9FE_F7F9_11D2_89B6_00A024CC68DB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "ACE_Init_Test_Resource.h" // main symbols

/**
 * @class CACE_Init_TestApp
 *
 * @brief See ACE_Init_Test.cpp for the implementation of this class
 */
class CACE_Init_TestApp : public CWinApp
{
public:
  CACE_Init_TestApp (void);

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CACE_Init_TestApp)
public:
  ///}}AFX_VIRTUAL
  virtual BOOL InitInstance();

  // Implementation

  //{{AFX_MSG(CACE_Init_TestApp)
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif /* !defined(AFX_ACE_INIT_TEST_H__64FDC9FE_F7F9_11D2_89B6_00A024CC68DB__INCLUDED_) */
