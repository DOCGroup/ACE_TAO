#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "../check_supported_priorities.cpp"
#include "../common_args.cpp"
#include "testS.h"

#include <cstddef>
#include <memory>

struct Server_Options
{
  CORBA::ULong stacksize {};
  CORBA::ULong static_threads { 2 };
  CORBA::ULong dynamic_threads {};
  bool allow_request_buffering {};
  CORBA::ULong max_buffered_requests {};
  CORBA::ULong max_request_buffer_size {};
  bool allow_borrowing {};
  int debug { 1 };
  int ior_file_count { 1 };
  ACE_TCHAR const *ior_file_base { ACE_TEXT ("ior") };
  ACE_TCHAR const *bands_file { ACE_TEXT ("empty_file") };
  ACE_TCHAR const *lanes_file { ACE_TEXT ("empty_file") };
};

class test_i :
  public POA_test
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          int debug);
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  void method () override;

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override;
  //FUZZ: enable check_for_lack_ACE_OS

  PortableServer::POA_ptr _default_POA () override;

private:
  CORBA::ORB_var const orb_;
  PortableServer::POA_var const poa_;
  int const debug_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                int debug)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    debug_ (debug)
{
}

void
test_i::method ()
{
  // Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources * const tss =
    this->orb_->orb_core ()->get_tss_resources ();

  /// Get the lane attribute in TSS.
  // Both pointers are borrowed from the ORB's thread-specific resources.
  TAO_Thread_Lane * const lane = static_cast<TAO_Thread_Lane *> (tss->lane_);

  if (this->debug_)
    {
      if (lane)
        ACE_DEBUG ((LM_DEBUG,
                    "test_i::method request in thread %t (pool id = %d; lane id = %d)\n",
                    lane->pool ().id (),
                    lane->id ()));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "test_i::method request in thread %t (default thread pool)\n"));
    }
}

void
test_i::shutdown ()
{
  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                "test_i::shutdown\n"));

  this->orb_->shutdown (false);
}

PortableServer::POA_ptr
test_i::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

static int
parse_args (int argc, ACE_TCHAR **argv, Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:d:l:o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'b':
        options.bands_file = get_opts.opt_arg ();
        break;

      case 'd':
        options.debug = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'l':
        options.lanes_file = get_opts.opt_arg ();
        break;

      case 'o':
        options.ior_file_base = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s usage:\n"
                           "\t-b <bands file> (defaults to %s)\n"
                           "\t-b <lanes file> (defaults to %s)\n"
                           "\t-o <ior file base> (defaults to %s)\n"
                           "\n",
                           argv[0],
                           options.bands_file,
                           options.lanes_file,
                           options.ior_file_base),
                          -1);
      }

  return 0;
}

static void
write_iors_to_file (CORBA::Object_ptr object,
                    CORBA::ORB_ptr orb,
                    Server_Options &options)
{
  ACE_TCHAR filename[BUFSIZ] {};
  ACE_OS::sprintf (filename,
                   ACE_TEXT ("%s_%d"),
                   options.ior_file_base,
                   options.ior_file_count);
  ++options.ior_file_count;

  std::unique_ptr<FILE, int (*) (FILE *)> file (
    ACE_OS::fopen (filename, ACE_TEXT ("w")),
    ACE_OS::fclose);
  ACE_ASSERT (file != nullptr);

  CORBA::String_var const ior =
    orb->object_to_string (object);

  int const result = ACE_OS::fprintf (file.get (), "%s", ior.in ());

  ACE_ASSERT (result >= 0 && static_cast<size_t> (result) == ACE_OS::strlen (ior.in ()));
  ACE_UNUSED_ARG (result);
}

void
vanilla_poa (CORBA::ORB_ptr orb,
             PortableServer::POA_ptr root_poa,
             PortableServer::POAManager_ptr poa_manager,
             Server_Options &options)
{
  CORBA::PolicyList policies;

  CORBA::Policy_var const implicit_activation_policy =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    implicit_activation_policy;

  PortableServer::POA_var const poa =
    root_poa->create_POA ("child",
                          poa_manager,
                          policies);

  test_i *servant {};
  // ServantBase_var assumes ownership immediately below.
  ACE_NEW_THROW_EX (servant,
                    test_i (orb,
                            poa.in (),
                            options.debug),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var safe_servant (servant);

  PortableServer::ObjectId_var const id_act =
    poa->activate_object (servant);

  CORBA::Object_var const object = poa->id_to_reference (id_act.in ());

  test_var const test =
    test::_narrow (object.in ());

  write_iors_to_file (test.in (),
                      orb,
                      options);
}

void
rt_poa (CORBA::ORB_ptr orb,
        RTCORBA::RTORB_ptr rt_orb,
        PortableServer::POA_ptr root_poa,
        PortableServer::POAManager_ptr poa_manager,
        Server_Options &options)
{
  CORBA::PolicyList policies;

  int result =
    get_priority_bands ("server",
                        options.bands_file,
                        rt_orb,
                        policies,
                        options.debug);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error in parsing bands data file: %s\n",
                  options.bands_file));
      return;
    }

  result =
    get_priority_lanes ("server",
                        options.lanes_file,
                        rt_orb,
                        options.stacksize,
                        options.static_threads,
                        options.dynamic_threads,
                        options.allow_request_buffering,
                        options.max_buffered_requests,
                        options.max_request_buffer_size,
                        options.allow_borrowing,
                        policies,
                        options.debug);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error in parsing lanes data file: %s\n",
                  options.lanes_file));
      return;
    }

  CORBA::Policy_var const priority_model_policy =
    rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                          0);

  // Implicit_activation policy.
  CORBA::Policy_var const implicit_activation_policy =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    priority_model_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    implicit_activation_policy;

  PortableServer::POA_var const poa =
    root_poa->create_POA ("rt_poa",
                          poa_manager,
                          policies);

  test_i *servant {};
  // ServantBase_var assumes ownership immediately below.
  ACE_NEW_THROW_EX (servant,
                    test_i (orb,
                            poa.in (),
                            options.debug),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var safe_servant (servant);

  PortableServer::ObjectId_var const id_act =
    poa->activate_object (servant);

  CORBA::Object_var const object = poa->id_to_reference (id_act.in ());

  test_var const test =
    test::_narrow (object.in ());

  write_iors_to_file (test.in (),
                      orb,
                      options);
}

class Task : public ACE_Task_Base
{
public:
  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb,
        Server_Options const &options);
  Task (Task const &) = delete;
  Task (Task &&) = delete;
  Task &operator= (Task const &) = delete;
  Task &operator= (Task &&) = delete;

  int svc () override;

private:
  CORBA::ORB_var const orb_;
  Server_Options options_;
};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb,
            Server_Options const &options)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb)),
    options_ (options)
{
}

int
Task::svc ()
{
  try
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var const rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      vanilla_poa (this->orb_.in (),
                   root_poa.in (),
                   poa_manager.in (),
                   this->options_);

      rt_poa (this->orb_.in (),
              rt_orb.in (),
              root_poa.in (),
              poa_manager.in (),
              this->options_);

      poa_manager->activate ();

      this->orb_->run ();

      this->orb_->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;

      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      int const parse_args_result = parse_args (argc, argv, options);
      if (parse_args_result != 0)
        return parse_args_result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      if (!check_supported_priorities (orb.in ()))
        return 2;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in (),
                 options);

      // Task activation flags.
      long const flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      int const activate_result = task.activate (flags);
      if (activate_result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %C\n"
                                 "because the user does not have the appropriate privileges, terminating program....\n"
                                 "Check svc.conf options and/or run as root\n",
                                 sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                                2);
            }
          else
            // Unexpected error.
            ACE_ASSERT (0);
        }

      // Wait for task to exit.
      int const wait_result = thread_manager.wait ();
      ACE_ASSERT (wait_result != -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
