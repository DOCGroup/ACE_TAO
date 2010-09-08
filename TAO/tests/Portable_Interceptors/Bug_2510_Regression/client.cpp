// $Id$

#include "ace/Get_Opt.h"
#include "ace/Synch.h"
#include "ace/Thread.h"
#include "testC.h"
#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("ef:"));
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

    try
    {
        server->normal (10);
    }
    catch (const CORBA::Exception& ex)
    {
        ex._tao_print_exception ("Exception thrown in run_test()\n");
    }

    return (ACE_THR_FUNC_RETURN)0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
    try
    {
        PortableInterceptor::ORBInitializer_ptr temp_initializer;

        ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
        PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

        PortableInterceptor::register_orb_initializer (initializer.in ());

        CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

        if (parse_args (argc, argv) != 0)
          return 1;

        CORBA::Object_var object =
        orb->string_to_object (ior);

        Test_Interceptors::Visual_var server =
        Test_Interceptors::Visual::_narrow (object.in ());

        if (CORBA::is_nil (server.in ()))
        {
            ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

        ACE_hthread_t threadHandle;
        if ( ACE_Thread::spawn(run_test,
                               static_cast<void*>(server.in()),
                               THR_NEW_LWP | THR_JOINABLE ,
                               0,
                               & threadHandle) == -1 )
        {
           ACE_ERROR_RETURN ((LM_ERROR,"Cannot start thread"), 1);
        }

        ACE_Thread::join (threadHandle);

        server->shutdown ();

        orb->destroy ();
    }

    catch (const CORBA::Exception& ex)
    {
        ex._tao_print_exception ("Caught exception in client:");
        return 1;
    }


    return 0;
}
