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
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {

      IOR_corbaloc_Client_i client;

      int init_result;
      init_result = client.init (argc, argv ACE_ENV_ARG_PARAMETER);

        if (init_result == 0)
          {
            client.run ();
          }
        return 0;
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      //
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
    }
  ACE_ENDTRY;

  return 0;
}
