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
#include "ace/Argv_Type_Converter.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try
    {
      ACE_Argv_Type_Converter atc (argc, argv);
      IOR_corbaloc_Client_i client;

      int init_result;
      init_result = client.init (atc.get_argc (), atc.get_ASCII_argv ());

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
