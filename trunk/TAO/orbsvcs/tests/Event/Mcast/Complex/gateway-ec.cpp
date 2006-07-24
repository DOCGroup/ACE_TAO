// $Id$

#include "Gateway_EC.h"

int 
main (int argc, char ** argv)
{
  Gateway_EC test;
  if (test.run (argc, argv) == -1)
    return 1;

  return 0;
}
