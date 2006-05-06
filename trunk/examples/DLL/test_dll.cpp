// $Id$

// This program tests out how the various objects can be loaded
// dynamically and method calls made on them.

#include "Magazine.h"
#include "ace/DLL.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

ACE_RCSID(DLL, test_dll, "$Id$")

typedef Magazine* (*Magazine_Creator) (void);

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_DLL dll;

  int retval = dll.open (ACE_TEXT("./") ACE_DLL_PREFIX ACE_TEXT("DLL_Today"));

  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p",
                       "dll.open"),
                      -1);
  Magazine_Creator mc;

  mc = (Magazine_Creator) dll.symbol (ACE_TEXT("create_magazine"));

  if (mc == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p",
                      "dll.symbol"),
                      -1);
  {
    auto_ptr <Magazine> magazine (mc ());

    magazine->title ();
  }

  dll.close ();

  // The other library is now loaded on demand.

  retval = dll.open (ACE_DLL_PREFIX ACE_TEXT("DLL_Newsweek"));

  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p",
                       "dll.open"),
                      -1);

  mc = (Magazine_Creator) dll.symbol (ACE_TEXT("create_magazine"));

  if (mc == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                      "%p",
                       "dll.symbol"),
                      -1);
  {
    auto_ptr <Magazine> magazine (mc ());

    magazine->title ();
  }

  dll.close ();

  return 0;
}

