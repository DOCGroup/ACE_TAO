// $Id$

#include "Gateway_EC.h"
#include "ace/Argv_Type_Converter.h"

int 
ACE_TMAIN (int argc, ACE_TCHAR ** argv)
{
  ACE_Argv_Type_Converter convert (argc, argv);

  Gateway_EC test;
  if (test.run (argc, argv) == -1)
    return 1;

  return 0;
}
