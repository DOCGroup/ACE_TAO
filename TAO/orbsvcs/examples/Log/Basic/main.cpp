#include "TLS_Client.h"

ACE_RCSID (Basic,
           main,
           "$Id$")


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TLS_Client client; // Telecom Log Service Client

  ACE_TRY_NEW_ENV
    {
      client.init (argc, argv
                   ACE_ENV_ARG_PARAMETER); //Init the Client
      ACE_TRY_CHECK;

      client.run_tests (ACE_ENV_SINGLE_ARG_PARAMETER); //Init the Client

      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "TLS_Client user error: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "TLS_Client system error: ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
