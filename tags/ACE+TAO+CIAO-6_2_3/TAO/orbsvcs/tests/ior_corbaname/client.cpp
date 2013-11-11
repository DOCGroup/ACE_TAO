
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *   This implements a simple CORBA client for the
 *   corbaname: style IOR parser
 *
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
      else
        {
          return client.run ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client");
    }
  return 1;
}
