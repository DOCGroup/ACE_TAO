#include "Server_Peer.h"
#include "Clock_Ticks.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdio.h"
#include <memory>
#include <string>

struct Options
{
  std::basic_string<ACE_TCHAR> ior_output_file {ACE_TEXT ("test.ior")};
  unsigned int seed {};
  int payload_size {4};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:s:b:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case 's':
        options.seed = static_cast<unsigned int> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;

      case 'b':
        options.payload_size = ACE_OS::atoi (get_opts.opt_arg ());
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
  return 0;
}

class Sleeper : public ACE_Event_Handler
{
public:
  explicit Sleeper (CORBA::ORB_ptr orb);
  Sleeper (Sleeper const &) = delete;
  Sleeper (Sleeper &&) = delete;
  Sleeper &operator= (Sleeper const &) = delete;
  Sleeper &operator= (Sleeper &&) = delete;

  int handle_timeout (ACE_Time_Value const &current_time,
                      void const *arg) override;

private:
  CORBA::ORB_var orb_ {};
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  options.seed = static_cast<unsigned int> (ACE_OS::gethrtime ());
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();
      CORBA::Object_var const object =
        orb->resolve_initial_references ("PolicyCurrent");
      CORBA::PolicyCurrent_var const policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }

      CORBA::Any scope_as_any;
      scope_as_any <<= Messaging::SYNC_WITH_SERVER;
      CORBA::PolicyList policies (1);
      policies.length (1);
      policies[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            scope_as_any);
      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);
      policies[0]->destroy ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG, "SEED = %u\n", options.seed));
      std::unique_ptr<Server_Peer> servant =
        std::make_unique<Server_Peer> (options.seed,
                                       orb.in (),
                                       static_cast<CORBA::ULong> (options.payload_size));
      Server_Peer *const impl = servant.get ();
      PortableServer::ServantBase_var const owner_transfer (servant.release ());
      PortableServer::ObjectId_var const id = root_poa->activate_object (impl);
      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());
      Test::Peer_var const peer = Test::Peer::_narrow (object_act.in ());
      CORBA::String_var const ior = orb->object_to_string (peer.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();
      Sleeper sleeper (orb.in ());
      ACE_Time_Value const interval (0, 500000);
      ACE_Reactor *const reactor = orb->orb_core ()->reactor ();
      reactor->schedule_timer (&sleeper, 0, interval, interval);
      orb->run ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
  return 0;
}

Sleeper::Sleeper (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Sleeper::handle_timeout (ACE_Time_Value const &,
                         void const *)
{
  ACE_Time_Value clk_tck (0, Clock_Ticks::get_usecs_per_tick ());
  this->orb_->perform_work (clk_tck);
  return 0;
}
