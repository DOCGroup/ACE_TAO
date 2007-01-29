// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/ior_corbaloc/
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//     This implements a simple CORBA client for the
//     corbaloc: style IOR parser
//
// = AUTHOR
//     Priyanka Gontla <pgontla@ece.uci.edu>
//
// ============================================================================

#include "ior_corbaloc_client_i.h"

int main (int argc, char *argv [])
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
