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
  ACE_TRY_NEW_ENV
    {
      Client_i client_i;

      if (client_i.init (argc, argv) == -1
	  || client_i.run () == -1)
	return -1;

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "client::main\t\n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
