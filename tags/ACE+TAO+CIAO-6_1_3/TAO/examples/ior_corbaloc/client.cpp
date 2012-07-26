
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *   This implements a simple CORBA client for the
 *   corbaloc: style IOR parser
 *
 *
 *  @author  Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#include "ior_corbaloc_client_i.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try
    {
      IOR_corbaloc_Client_i client;

      int init_result;
      init_result = client.init (argc, argv);

        if (init_result == 0)
          {
            client.run ();
          }
        return 0;
    }
  catch (const CORBA::SystemException&)
    {
      //
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client");
    }

  return 0;
}
