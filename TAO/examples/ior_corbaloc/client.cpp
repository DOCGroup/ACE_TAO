
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

      int const init_status = client.init (argc, argv);
      if (init_status != 0)
        return init_status;

      return client.run ();
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception ("client");
      return 1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("client");
      return 1;
  }
}
