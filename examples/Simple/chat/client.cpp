
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *  The Chat client program entry point.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#include "Client_i.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_i client_i;

      if (client_i.init (argc, argv) == -1 || client_i.run () == -1)
        return -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client::main\t\n");
      return -1;
    }

  return 0;
}
