// $Id$

#include "Server_Peer.h"
#include "Clock_Ticks.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_time.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");
ACE_RANDR_TYPE seed;
int payload_size = 4;

int
parse_args (int argc, ACE_TCHAR *argv[]);

class Sleeper : public ACE_Event_Handler
{
public:
  Sleeper(CORBA::ORB_ptr orb);

  virtual int handle_timeout (ACE_Time_Value const & current_time,
                              void const * arg);

private:
  CORBA::ORB_var orb_;
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

      policy_current->set_policy_overrides (policies,
                                            CORBA::ADD_OVERRIDE);

      policies[0]->destroy ();

      seed = (ACE_RANDR_TYPE) ACE_OS::gethrtime ();

      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG, "SEED = %u\n", seed));

      Server_Peer *impl;
      ACE_NEW_RETURN (impl,
                      Server_Peer (seed, orb.in (), payload_size),
                      1);
      PortableServer::ServantBase_var owner_transfer(impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (impl);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      Test::Peer_var peer =
        Test::Peer::_narrow (object_act.in ());

      CORBA::String_var ior =
        orb->object_to_string (peer.in ());

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      Sleeper sleeper (orb.in ());

      ACE_Time_Value interval(0, 500000);
      ACE_Reactor * reactor = orb->orb_core()->reactor();
      reactor->schedule_timer(&sleeper, 0, interval, interval);

      // ACE_Time_Value run_time(600, 0);
      // orb->run (run_time);
      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:s:b:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 's':
        seed = ACE_OS::atoi(get_opts.opt_arg ());
        break;

      case 'b':
        payload_size = ACE_OS::atoi(get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-s <seed> "
                           "-b <payload_size> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

Sleeper::Sleeper(CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Sleeper::handle_timeout (ACE_Time_Value const & ,
                         void const *)
{
  // ACE_DEBUG((LM_DEBUG, "(%P|%t) - Sleeper::handle_timeout()\n"));
  ACE_Time_Value clk_tck (0, Clock_Ticks::get_usecs_per_tick ());
  this->orb_->perform_work(clk_tck);

  return 0;
}
