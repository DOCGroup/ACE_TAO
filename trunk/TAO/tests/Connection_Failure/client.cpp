// $Id$

#include "testC.h"
#include "tao/SystemException.h"

#include "ace/Log_Msg.h"

ACE_RCSID (Connection_Failure,
           client,
           "$Id$")

const char *ior =
  "corbaloc:iiop:192.3.47/10007/RandomObject";

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var tmp =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // First connection happens here..
      test::_narrow (tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::TRANSIENT, transient_exception)
    {
      // This exception is correct.
      ACE_DEBUG ((LM_DEBUG,
                  "_narrow() to non-existent object failed correctly.  Test succeeded.\n"));
      return 0;
    }
  ACE_ENDTRY;

  // Should not reach here.
  ACE_ERROR_RETURN ((LM_ERROR,
                     "_narrow() succeeded to non-existent object when it should not have...\n"),
                     -1);
}
