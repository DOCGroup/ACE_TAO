// $Id$

// This program tests out how the various objects can be loaded
// dynamically and method calls made on them.

#include "Magazine.h"
#include "ace/DLL.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"



typedef Magazine* (*Magazine_Creator) (void);

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_DLL dll;

  int retval = dll.open (ACE_DLL_PREFIX ACE_TEXT("DLL_Today"));

  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p",
                       "dll.open"),
                      -1);
  Magazine_Creator mc;

  // Cast the void* to non-pointer type first - it's not legal to
  // cast a pointer-to-object directly to a pointer-to-function.
  void *void_ptr = dll.symbol (ACE_TEXT ("create_magazine"));
  ptrdiff_t tmp = reinterpret_cast<ptrdiff_t> (void_ptr);
  mc = reinterpret_cast<Magazine_Creator> (tmp);

  if (mc == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p",
                         "dll.symbol"),
                        -1);
    }

  {
    auto_ptr <Magazine> magazine (mc ());

    magazine->title ();
  }

  dll.close ();

  // The other library is now loaded on demand.

  retval = dll.open (ACE_DLL_PREFIX ACE_TEXT ("DLL_Newsweek"));

  if (retval != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p",
                         "dll.open"),
                        -1);
    }

  // Cast the void* to non-pointer type first - it's not legal to
  // cast a pointer-to-object directly to a pointer-to-function.
  void_ptr = dll.symbol (ACE_TEXT ("create_magazine"));
  tmp = reinterpret_cast<ptrdiff_t> (void_ptr);
  mc = reinterpret_cast<Magazine_Creator> (tmp);

  if (mc == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p",
                         "dll.symbol"),
                        -1);
    }

  {
    auto_ptr <Magazine> magazine (mc ());

    magazine->title ();
  }

  dll.close ();

  return 0;
}

