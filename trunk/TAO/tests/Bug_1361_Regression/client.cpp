// $Id$

#include "Echo.h"
#include "ORB_Task.h"
#include "tao/Utils/Servant_Var.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_signal.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int serverthreads = 4;

int
parse_args (int argc, ACE_TCHAR *argv[]);

class Client_Timer : public ACE_Event_Handler
{
public:
  /// Constructor
  Client_Timer (ACE_Reactor * reactor)
    : ACE_Event_Handler (reactor)
  {
    this->reference_counting_policy ().value (
      ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
  }

  void activate (void)
  {
    ACE_Time_Value tv (150, 0);
    this->reactor()->schedule_timer (this, 0, tv, tv);
  }

  /// Thread entry point
  int handle_timeout (ACE_Time_Value const & , void const *)
  {
    this->reactor ()->cancel_timer (this);
    // kill the application
    ACE::terminate_process (ACE_OS::getpid ());
    return 0;
  }
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      TAO::Utils::Servant_Var<Echo> impl;
      {
        Echo * tmp;
        // ACE_NEW_RETURN is the worst possible way to handle
        // exceptions (think: what if the constructor allocates memory
        // and fails?), but I'm not in the mood to fight for a more
        // reasonable way to handle allocation errors in ACE.
        ACE_NEW_RETURN (tmp,
                        Echo(orb.in(), 1000 / serverthreads),
                        1);
        impl = tmp;
      }

      PortableServer::ObjectId_var id =
        root_poa->activate_object (impl.in ());

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      Test::Echo_var echo =
        Test::Echo::_narrow (object_act.in ());

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Echo_Caller_var server =
        Test::Echo_Caller::_narrow(tmp.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Echo_Caller reference <%s>\n",
                             ior),
                            1);
        }

      poa_manager->activate ();

      ORB_Task worker (orb.in());
      worker.activate (THR_NEW_LWP | THR_JOINABLE,
                       serverthreads);

      try
        {
          for(int i = serverthreads; i; --i)
            {
              server->start_task(echo.in());
            }
        }
      catch (...)
        {
        }

      Client_Timer * task = new Client_Timer (orb->orb_core()->reactor());
      task->activate ();
      task->remove_reference ();

      orb->run ();

      worker.wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) client - event loop finished\n"));

      // Actually the code here should never be reached.
      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 't':
        serverthreads = ACE_OS::atoi(get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-t threads "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}
