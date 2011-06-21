// $Id$

#include "Echo.h"
#include "ORB_Task.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[]);

class Crasher : public ACE_Event_Handler
{
public:
  Crasher();

  virtual int handle_timeout (ACE_Time_Value const & current_time,
                              void const * arg);
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::Object_var object =
        orb->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }
      CORBA::Any scope_as_any;
      scope_as_any <<= Messaging::SYNC_WITH_TRANSPORT;

      CORBA::PolicyList policies(1); policies.length (1);
      policies[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            scope_as_any);

      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      policies[0]->destroy ();

      if (parse_args (argc, argv) != 0)
        return 1;

      PortableServer::Servant_var<Echo> impl;
      {
        Echo * tmp = 0;
        // ACE_NEW_RETURN is the worst possible way to handle
        // exceptions (think: what if the constructor allocates memory
        // and fails?), but I'm not in the mood to fight for a more
        // reasonable way to handle allocation errors in ACE.
        ACE_NEW_RETURN (tmp,
                        Echo(orb.in()),
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

      Test::Server_var server =
        Test::Server::_narrow(tmp.in ());

      if (CORBA::is_nil (echo.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Server reference <%s>\n",
                             ior),
                            1);
        }

      poa_manager->activate ();

      Crasher crasher;

      // Crash after 15 seconds
      ACE_Time_Value interval(15, 0);
      ACE_Reactor * reactor = orb->orb_core()->reactor();
      reactor->schedule_timer(&crasher, 0, interval, interval);

      ORB_Task task(orb.in());
      task.activate(THR_NEW_LWP | THR_JOINABLE, 4, 1);

      Test::Payload payload(16); payload.length(16);
      for(int i = 0; i != 4; ++i)
        {
          server->start_task(echo.in());
        }

      task.wait();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - event loop finished\n"));

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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
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
  // Indicates successful parsing of the command line
  return 0;
}

Crasher::Crasher()
{
}

int
Crasher::handle_timeout (ACE_Time_Value const & ,
                         void const *)
{
  ACE_OS::abort();

  return 0;
}
