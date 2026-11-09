
//=============================================================================
/**
 *  @file    client.cpp
 *
 *   This implements a simple CORBA client for the
 *   CosNaming::NamingContextExt example
 *
 *  @author  Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================

#include "ncontextext_client_i.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      NContextExt_Client_i client;

      if (client.init (argc, argv) != 0)
        return 1;

      return client.run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("client");
      return 1;
    }

  return 0;
}
