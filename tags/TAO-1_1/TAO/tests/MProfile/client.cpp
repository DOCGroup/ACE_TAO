// $Id$

#include "tao/IORManipulation.h"
#include "tao/IORS.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "testC.h"

ACE_RCSID(MT_Server, client, "$Id$")

const char *ior = "file://test.ior";
int do_shutdown = 0;

const char *name = "file://amba.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "xk:m:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.optarg;
        break;

      case 'm':
        name = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

void
run_test (Simple_Server_ptr server,
          CORBA::Environment &ACE_TRY_ENV);

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Primary server
      CORBA::Object_var object_primary =
        orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      //Secondary server
      CORBA::Object_var object_secondary =
        orb->string_to_object (name, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get an object reference for the ORBs IORManipultion object!
      CORBA_Object_ptr IORM =
        orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                         0,
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      TAO_IOP::TAO_IOR_Manipulation_ptr iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length(2);
      iors [0] = object_primary;
      iors [1] = object_secondary;

      CORBA_Object_var merged = iorm->merge_iors (iors, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      // Combined IOR stuff
      Simple_Server_var server =
        Simple_Server::_narrow (merged.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      run_test (server.in (), ACE_TRY_ENV);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Caught an exception \n");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

void run_test (Simple_Server_ptr server,
               CORBA::Environment &ACE_TRY_ENV)
{
  for (int loop = 0; loop < 10; loop++)
    {
      ACE_TRY
        {
          // Make a remote call
          server->remote_call (ACE_TRY_ENV);
          ACE_TRY_CHECK;
          
          /*ACE_DEBUG ((LM_DEBUG,
                      "Kill  the primary . . . "));
          ACE_OS::sleep (25);
          ACE_DEBUG ((LM_DEBUG, " hope you did\n")); */
          ACE_DEBUG ((LM_DEBUG,
                      "I am going to shutdown \n"));
          server->shutdown (ACE_TRY_ENV);
	  ACE_TRY_CHECK;
          ACE_OS::sleep (25);
        }
      ACE_CATCH (CORBA::TRANSIENT, t)
        {
          if (t.completed () != CORBA::COMPLETED_NO)
            {
              ACE_PRINT_EXCEPTION (t, "Unexpected kind of TRANSIENT");
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "The completed status %d\n", t.completed ()));
              ACE_DEBUG ((LM_DEBUG,
                          "Automagically re-issuing request on TRANSIENT\n"));
              ACE_OS::sleep (1);
            }
        }
      ACE_CATCH (CORBA::COMM_FAILURE, f)
        {
          ACE_PRINT_EXCEPTION (f, "A (sort of) expected COMM_FAILURE");
          ACE_DEBUG ((LM_DEBUG,
                      "Automagically re-issuing request on COMM_FAILURE\n"));
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Unexpected exception");
          ACE_RETHROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}
