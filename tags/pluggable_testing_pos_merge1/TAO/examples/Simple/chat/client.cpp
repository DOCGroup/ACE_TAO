// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//    The Chat client program entry point.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#include "Client_i.h"

int
main (int argc, char *argv[])
{
  TAO_TRY
    {
      Client_i client_i;

      if (client_i.init (argc, argv) == -1
	  || client_i.run () == -1)
	return -1;

      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("client::main\t\n");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
