// -*- C++ -*-
// $Id$

#include "Ptest.h"

ACE_RCSID(Persistence_Test, test, "$Id$")

int main (int argc, char *argv[])
{
  Ptest ptest;

  int retval = ptest.init (argc, 
                           argv);

  if (retval == -1)
    return 1;

  retval = ptest.run ();

  return retval;
}
