// -*- C++ -*-

// ================================================================
/**
 * @file DLL_Test.h
 *
 * $Id$
 *
 * @author Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
// ================================================================


#ifndef ACE_TESTS_DLL_TEST_H
#define ACE_TESTS_DLL_TEST_H

#include "DLL_Test_Parent.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "ace/Trace.h"
#include "ace/svc_export.h"

/**
 * @class Hello
 *
 * @brief Define the interface used by the DLL_Test
 */
class Hello
{
public:
  /// Destructor
  virtual ~Hello (void)
    {
      ACE_TRACE ("Hello::~Hello");
    }

  /**
   * @name Methods invoked by the test
   *
   * The test invokes four methods, a non-virtual method and a three virtual
   * methods implemented in the shared library.
   */
  //@{
  void say_hello (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Hello\n")));
  }

  virtual void say_next (void) = 0;

  virtual ACE_TCHAR *new_info (void) = 0;

  virtual ACE_TCHAR *malloc_info (void) = 0;
  //@}
};


// Used to test dynamic_cast<> in shared libraries.
class ACE_Svc_Export Child : public Parent
{
public:
  Child (void);

  virtual ~Child (void);

  virtual void test (void);

};

#endif /* ACE_TESTS_DLL_TEST_H */
