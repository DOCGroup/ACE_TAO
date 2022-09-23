/* -*- C++ -*-  */

#include "Test_i.h"

CORBA::Short
Test_i::get_server_num ()
{
  return 1;
}

void
Test_i::terminate ()
{
  exit (0);
}


