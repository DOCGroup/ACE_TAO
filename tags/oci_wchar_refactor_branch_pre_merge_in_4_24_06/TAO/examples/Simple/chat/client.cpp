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
#include "ace/Argv_Type_Converter.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);
  ACE_TRY_NEW_ENV
    {
      Client_i client_i;

      if (client_i.init (convert.get_argc(), convert.get_ASCII_argv()) == -1
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
