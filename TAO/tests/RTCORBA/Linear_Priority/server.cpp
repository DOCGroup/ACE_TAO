#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"
#include "testS.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"
#include "../common_args.cpp"

#include <memory>

class test_i :
  public POA_test
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);
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
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
test_i::method ()
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::method\n"));
}

void
test_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::shutdown\n"));

  this->orb_->shutdown (false);
}

PortableServer::POA_ptr
test_i::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

struct Server_Options
{
  CORBA::ULong stacksize {};
  CORBA::ULong static_threads { 2 };
  CORBA::ULong dynamic_threads {};
  CORBA::Boolean allow_request_buffering {};
  CORBA::ULong max_buffered_requests {};
  CORBA::ULong max_request_buffer_size {};
  CORBA::Boolean allow_borrowing {};
  ACE_TCHAR const *ior { ACE_TEXT ("ior") };
  ACE_TCHAR const *bands_file { ACE_TEXT ("bands") };
  ACE_TCHAR const *lanes_file { ACE_TEXT ("lanes") };
  int debug { 1 };
};

static int
parse_args (int argc, ACE_TCHAR **argv, Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:b:d:l:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior = get_opts.opt_arg ();
        break;

      case 'b':
        options.bands_file = get_opts.opt_arg ();
        break;

      case 'd':
        options.debug = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'l':
        options.lanes_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-b <bands_file> "
                           "-d <debug> "
                           "-l <lanes_file> "
                           "-o <ior file> "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

static void
write_iors_to_file (CORBA::Object_ptr object,
                    CORBA::ORB_ptr orb,
                    ACE_TCHAR const *filename)
{
  std::unique_ptr<FILE, int (*) (FILE *)> file (
    ACE_OS::fopen (filename, ACE_TEXT ("w")), ACE_OS::fclose);
  ACE_ASSERT (file != nullptr);

  CORBA::String_var const ior =
    orb->object_to_string (object);

  int const result =
    ACE_OS::fprintf (file.get (),
                     "%s",
                     ior.in ());

  ACE_ASSERT (result >= 0 &&
              static_cast<size_t> (result) == ACE_OS::strlen (ior.in ()));
  ACE_UNUSED_ARG (result);
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
  Server_Options const options_;
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

      CORBA::PolicyList policies;

      int result =
        get_priority_bands ("server",
                            this->options_.bands_file,
                            rt_orb.in (),
                            policies,
                            this->options_.debug);
      if (result != 0)
        return result;

      result =
        get_priority_lanes ("server",
                            this->options_.lanes_file,
                            rt_orb.in (),
                            this->options_.stacksize,
                            this->options_.static_threads,
                            this->options_.dynamic_threads,
                            this->options_.allow_request_buffering,
                            this->options_.max_buffered_requests,
                            this->options_.max_request_buffer_size,
                            this->options_.allow_borrowing,
                            policies,
                            this->options_.debug);
      if (result != 0)
        return result;

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
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies);

      test_i *servant {};
      ACE_NEW_THROW_EX (servant,
                        test_i (this->orb_.in (),
                                poa.in ()),
                        CORBA::NO_MEMORY ());

      PortableServer::ServantBase_var safe_servant (servant);

      PortableServer::ObjectId_var const id =
        poa->activate_object (servant);

      CORBA::Object_var const object_act = poa->id_to_reference (id.in ());

      test_var const test =
        test::_narrow (object_act.in ());

      write_iors_to_file (test.in (),
                          this->orb_.in (),
                          this->options_.ior);

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
