// $Id$

// This file has the class definitions needed for template generation in
// DLL_Test.cpp.  They have to be in a separate file so AIX xlC can
// find them at auto-instantiate time.

#ifndef ACE_DLL_TEST_H
#define ACE_DLL_TEST_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"

class Hello
{
  // = TITLE
  //    The Hello class in the dynamically linkable library.
  //
  // = DESCRIPTION
  //    This class is used in this example to show how a library can
  //    be loaded on demand and its methods called on getting the
  //    symbols from the library.
public:

  void say_hello (void)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Hello\n"));
    }

  void say_next (void)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "How are you?\n"));
    }
};

#endif /* ACE_MAP_TEST_H */
