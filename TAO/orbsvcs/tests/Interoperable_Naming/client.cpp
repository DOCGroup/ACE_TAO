
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *   This implements a simple CORBA client for the
 *   CosNaming::NamingContextExt example
 *
 *
 *  @author  Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#include "ncontextext_client_i.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = 0;

  try
    {

      NContextExt_Client_i client;

      if (client.init (argc, argv) != 0)
        return 1;

      result = client.run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client");
      return 1;
    }

  return result;
}
