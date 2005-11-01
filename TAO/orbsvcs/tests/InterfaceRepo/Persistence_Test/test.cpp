// -*- C++ -*-
// $Id$

#include "Ptest.h"

ACE_RCSID (Persistence_Test,
           test,
           "$Id$")

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  Ptest ptest;

  int retval = ptest.init (convert.get_argc(), convert.get_ASCII_argv());

  if (retval == -1)
    return 1;

  retval = ptest.run ();

  return retval;
}
