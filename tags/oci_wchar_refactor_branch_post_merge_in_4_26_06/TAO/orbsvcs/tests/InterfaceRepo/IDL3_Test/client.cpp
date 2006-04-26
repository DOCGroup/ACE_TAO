// -*- C++ -*-
// $Id$

#include "idl3_client.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID (Application_Test, 
           client, 
           "$Id$")

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  IDL3_Client client;

  ACE_TRY_NEW_ENV
    {
      if (client.init (convert.get_argc(), convert.get_ASCII_argv()
                       ACE_ENV_ARG_PARAMETER)
           == -1)
        {
          return 1;
        }
      else
        {
          int status = client.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (status == -1)
            {
              return 1;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
