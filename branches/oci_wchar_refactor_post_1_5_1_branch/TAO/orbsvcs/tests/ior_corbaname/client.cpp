// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO/tests/ior_corbaname/
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//     This implements a simple CORBA client for the
//     corbaname: style IOR parser
//
// = AUTHOR
//     Priyanka Gontla <pgontla@ece.uci.edu>
//
//
// ============================================================================

#include "ior_corbaname_client_i.h"
#include "ace/Argv_Type_Converter.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      IOR_corbaname_Client_i client;

      if (client.init (convert.get_argc(), convert.get_ASCII_argv()) == -1)
        return 1;
      else
        {
          return client.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
    }
  ACE_ENDTRY;
  return 1;
}
