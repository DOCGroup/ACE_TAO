// $Id$

#include "TestC.h"

const char *ior = "file://test.ior";

int
main (int argc, char *argv[])
{
  int received_expected_exception = 0;
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (roundtrip.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Test::Roundtrip reference <%s>\n",
                             ior),
                            1);
        }

      Test::Timestamp time = 10;
      roundtrip->test_method (time ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH(Test::ServerOverload, ov)
    {
      ACE_DEBUG ((LM_DEBUG, "Received expected exception\n"));
      received_expected_exception = 1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "");
      return 1;
    }
  ACE_ENDTRY;

  if(!received_expected_exception)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ERROR, expecting a ServerOverload exception!!\n"),
                        1);
    }

  return 0;
}
