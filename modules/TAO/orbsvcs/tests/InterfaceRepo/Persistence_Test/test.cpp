// -*- C++ -*-
// $Id$

#include "Ptest.h"

ACE_RCSID (Persistence_Test,
           test,
           "$Id$")

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Ptest ptest;

  int retval = ptest.init (argc, argv);

  if (retval == -1)
    return 1;

  retval = ptest.run ();

  return retval;
}
