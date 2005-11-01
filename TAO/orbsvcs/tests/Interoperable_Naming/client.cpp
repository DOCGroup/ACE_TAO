// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/Interoperable_Naming/
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//     This implements a simple CORBA client for the
//     CosNaming::NamingContextExt example
//
// = AUTHORS
//     Priyanka Gontla <pgontla@ece.uci.edu>
//
//
// ============================================================================

#include "ncontextext_client_i.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  int result = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      NContextExt_Client_i client;

      if (client.init (convert.get_argc(), convert.get_ASCII_argv()) != 0)
        return 1;

      result = client.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
      return 1;
    }
  ACE_ENDTRY;

  return result;
}
