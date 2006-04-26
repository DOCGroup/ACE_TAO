//
// $Id$
//
#include "tao/ORB.h"
#include "ace/Argv_Type_Converter.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (convert.get_argc(), convert.get_ASCII_argv(), "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "SERVER (%P): Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
