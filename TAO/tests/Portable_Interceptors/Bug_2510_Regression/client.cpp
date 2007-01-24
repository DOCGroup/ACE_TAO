// $Id$

#include "ace/Get_Opt.h"
#include "ace/Synch.h"
#include "testC.h"
#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

ACE_RCSID(Bug_2510_Regression, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "ef:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'e':
      break;
      case 'f':
        ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-v "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

static ACE_THR_FUNC_RETURN run_test(void* pData)
{
    Test_Interceptors::Visual_ptr server = static_cast<Test_Interceptors::Visual_ptr>(pData);

    ACE_TRY
    {
        server->normal (10 ACE_ENV_ARG_PARAMETER);
    }
    ACE_CATCHANY
    {
        ACE_PRINT_EXCEPTION (ex, "Exception thrown in run_test()\n");
    }
    ACE_ENDTRY;

    return (ACE_THR_FUNC_RETURN)0;
}

int
main (int argc, char *argv[])
{
    ACE_TRY_NEW_ENV
    {
        PortableInterceptor::ORBInitializer_ptr temp_initializer;

        ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
        PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

        PortableInterceptor::register_orb_initializer (initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);

        CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

        if (parse_args (argc, argv) != 0)
        return 1;

        CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);

        Test_Interceptors::Visual_var server =
        Test_Interceptors::Visual::_narrow (object.in () ACE_ENV_ARG_PARAMETER);

        if (CORBA::is_nil (server.in ()))
        {
            ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

        ACE_hthread_t threadHandle;
        if ( ACE_Thread::spawn(	run_test,
                                static_cast<void*>(server.in()),
						        THR_NEW_LWP | THR_JOINABLE ,
                                0,
                                & threadHandle
                               ) == -1 )
        {
           ACE_ERROR_RETURN ((LM_ERROR,"Cannot start thread"), 1);
        }

        ACE_Thread::join (threadHandle);

        server->shutdown ();
    }

    ACE_CATCHANY
    {
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception in client:");
        return 1;
    }

    ACE_ENDTRY;

    return 0;
}
