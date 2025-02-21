// -*- C++ -*-
#include "Ptest.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Ptest ptest;

  if (ptest.init (argc, argv) == -1)
    return 1;

  if (ptest.run () == -1)
    return 1;

  if (ptest.shutdown () == -1)
    return 1;

  return 0;
}
