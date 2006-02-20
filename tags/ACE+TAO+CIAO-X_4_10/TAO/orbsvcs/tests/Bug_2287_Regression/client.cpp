// $Id$

#include "TestS.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "Hello.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  CORBA::Boolean result = 0;
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Test failed - Not regression - Unexpected Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      // Check this isn't generating transients for any other reason
      hello->ping (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;


      ACE_TRY
        {
          hello->throw_location_forward (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_ERROR, "REGRESSION - Test has failed !!!\n"));
          result = 1;
        }
      ACE_CATCH (CORBA::TRANSIENT, my_ex)
        {
          ACE_UNUSED_ARG (my_ex);
          ACE_DEBUG ((LM_DEBUG, "Client catches a TRANSIENT, as expected. No problem !\n"));
        }
      ACE_ENDTRY;

      hello->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test failed (Not regression) because unexpected exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  if (result)
    {
      ACE_DEBUG ((LM_ERROR, "Error: REGRESSION identified!!!\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Test passed !!!\n"));
    }
  return result;
}
