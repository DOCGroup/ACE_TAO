//=============================================================================
/**
 *  @file    client.cpp
 *
 *   This implements a simple CORBA client for the
 *   corbaname: style IOR parser
 *
 *  @author  Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#include "ior_corbaname_client_i.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      IOR_corbaname_Client_i client;

      if (client.init (argc, argv) == -1)
        return 1;
      return client.run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("client"));
    }
  return 1;
}
