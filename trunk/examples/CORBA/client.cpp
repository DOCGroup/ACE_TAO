#include "ace/Log_Msg.h"
// $Id$

#include "Test.hh"

int
main (int argc, char *argv[])
{
  char *host = argc == 2 ? argv[1] : ACE_DEFAULT_SERVER_HOST;

  Test_var my_test;

  TRY {
    my_test = Test::_bind ("", host, IT_X);
    my_test->method (5);
  } CATCHANY {
    cerr << IT_X << endl;
    return -1;
  } ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "everything works!\n"));

  // Memory for my_test is automatically released by destructor of
  // smart pointer.
  return 0;
}
