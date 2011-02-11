// $Id$

#include "Client_Peer.h"
#include "Clock_Ticks.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[]);

class Timer : public ACE_Event_Handler
{
public:
  Timer(Test::Peer_ptr local_peer,
        Test::Peer_ptr remote_peer);

  virtual int handle_timeout (ACE_Time_Value const & current_time,
                              void const * arg);

private:
  Test::Peer_var local_peer_;
  Test::Peer_var remote_peer_;
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
      scope_as_any <<= Messaging::SYNC_WITH_SERVER;

      CORBA::PolicyList policies(1); policies.length (1);
      policies[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            scope_as_any);

      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      policies[0]->destroy ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Client_Peer *impl;
      ACE_NEW_RETURN (impl,
                      Client_Peer (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (impl);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      Test::Peer_var local_peer =
        Test::Peer::_narrow (object_act.in ());

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Peer_var peer =
        Test::Peer::_narrow(tmp.in ());

      if (CORBA::is_nil (peer.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Peer reference <%s>\n",
                             ior),
                            1);
        }

      poa_manager->activate ();

      Timer timer(local_peer.in (), peer.in ());

      ACE_Time_Value interval(0, 50 * Clock_Ticks::get_usecs_per_tick ());
      ACE_Reactor * reactor = orb->orb_core()->reactor();
      reactor->schedule_timer(&timer, 0, interval, interval);

      ACE_Time_Value run_time(120, 0);
      orb->run (run_time);

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

Timer::Timer(Test::Peer_ptr local_peer,
             Test::Peer_ptr remote_peer)
  : local_peer_ (Test::Peer::_duplicate (local_peer))
  , remote_peer_ (Test::Peer::_duplicate (remote_peer))
{
}

int
Timer::handle_timeout (ACE_Time_Value const & ,
                       void const *)
{
  Test::Payload data;
  this->remote_peer_->callme (this->local_peer_.in(), 32, data);
  return 0;
}
