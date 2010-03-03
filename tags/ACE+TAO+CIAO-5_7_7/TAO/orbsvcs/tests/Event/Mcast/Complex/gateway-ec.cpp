// $Id$

#include "Gateway_EC.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Gateway_EC test;
  if (test.run (argc, argv) == -1)
    return 1;

  return 0;
}
