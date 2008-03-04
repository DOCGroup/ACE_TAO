// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    Entry point for the chat server.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#include "Server_i.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      Server_i server_i;

      int ret = server_i.init (argc, argv);
      if (ret != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\n error in init.\n"),
                          1);
      ret = server_i.run ();
      if (ret != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\n error in run.\n"),
                          1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server::main\t\n");
      return 1;
    }

  return 0;
}
