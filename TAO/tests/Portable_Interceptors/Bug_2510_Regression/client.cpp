#include "ace/Get_Opt.h"
#include "ace/Synch.h"
#include "ace/Thread.h"
#include "testC.h"
#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/SString.h"

struct Options
{
  ACE_TString ior { ACE_TEXT ("file://test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("f:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          options.ior = get_opts.opt_arg ();
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:  %s "
                             "-f <ior> "
                             "\n",
                             argv [0]),
                            -1);
        }
    }
  return 0;
}

static ACE_THR_FUNC_RETURN
run_test (void *data)
{
  Test_Interceptors::Visual_ptr const server =
    static_cast<Test_Interceptors::Visual_ptr> (data);

  try
    {
      server->normal (10);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception thrown in run_test()\n");
    }

  return {};
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};
      Options options {};

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);
      PortableInterceptor::ORBInitializer_var const initializer (temp_initializer);

      PortableInterceptor::register_orb_initializer (initializer.in ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (options.ior.c_str ());

      Test_Interceptors::Visual_var const server =
        Test_Interceptors::Visual::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             options.ior.c_str ()),
                            1);
        }

      ACE_hthread_t thread_handle {};
      if (ACE_Thread::spawn (run_test,
                             static_cast<void *> (server.in ()),
                             THR_NEW_LWP | THR_JOINABLE,
                             0,
                             &thread_handle) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot start thread"), 1);

      ACE_Thread::join (thread_handle);

      server->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception in client:");
      return 1;
    }

  return 0;
}
