// $Id$

// This program tests out how the various objects can be loaded
// dynamically and method calls made on them.

#include "Magazine.h"
#include "ace/DLL.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(DLL, test_dll, "$Id$")

typedef Magazine* (*Magazine_Creator) (void);

int
main (int argc, char *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_DLL dll;

  int retval = dll.open ("./" ACE_DLL_PREFIX "Today" ACE_DLL_SUFFIX);

  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s",
                       dll.error ()),
                      -1);
  Magazine_Creator mc;

  mc = (Magazine_Creator) dll.symbol ("create_magazine");

  if (mc == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s",
                      dll.error ()),
                      -1);
  {
    auto_ptr <Magazine> magazine (mc ());

    magazine->title ();
  }

  dll.close ();

  // The other library is now loaded on demand.

  retval = dll.open ("./" ACE_DLL_PREFIX "Newsweek" ACE_DLL_SUFFIX);

  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s",
                       dll.error ()),
                      -1);

  mc = (Magazine_Creator) dll.symbol ("create_magazine");

  if (mc == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                      "%s",
                       dll.error ()),
                      -1);
  {
    auto_ptr <Magazine> magazine (mc ());

    magazine->title ();
  }

  dll.close ();

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr <Magazine>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr <Magazine>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
