
//=============================================================================
/**
 *  @file    client.cpp
 *
 *   This implements a simple CORBA client for the
 *   corbaloc: style IOR parser
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

      if (client.init (argc, argv) == 0)
        {
          client.run ();
        }
      return 0;
    }
  catch (CORBA::SystemException const &)
    {
      //
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("client");
    }

  return 0;
}
