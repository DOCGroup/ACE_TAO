// $Id$

#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "../check_supported_priorities.cpp"
#include "../common_args.cpp"
#include "testS.h"

static CORBA::ULong stacksize = 0;
static CORBA::ULong static_threads = 2;
static CORBA::ULong dynamic_threads = 0;
static CORBA::Boolean allow_request_buffering = 0;
static CORBA::ULong max_buffered_requests = 0;
static CORBA::ULong max_request_buffer_size = 0;
static CORBA::Boolean allow_borrowing = 0;

static int debug = 1;
static int ior_file_count = 1;
static const char *ior_file_base = "ior";
static const char *bands_file = "empty_file";
static const char *lanes_file = "empty_file";

class test_i :
  public POA_test,
  public PortableServer::RefCountServantBase
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);

  void method (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);

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
test_i::method (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources *tss =
    this->orb_->orb_core ()->get_tss_resources ();

  /// Get the lane attribute in TSS.
    TAO_Thread_Lane *lane =
      (TAO_Thread_Lane *) tss->lane_;

    if (debug)
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
test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "test_i::shutdown\n"));

  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::POA_ptr
test_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "b:d:l:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'b':
        bands_file = get_opts.opt_arg ();
        break;

      case 'd':
        debug = ::atoi (get_opts.opt_arg ());
        break;

      case 'l':
        lanes_file = get_opts.opt_arg ();
        break;

      case 'o':
        ior_file_base = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s usage: \n"
                           "\t-b <bands file> (defaults to %s)\n"
                           "\t-b <lanes file> (defaults to %s)\n"
                           "\t-o <ior file base> (defaults to %s)\n"
                           "\n",
                           argv[0],
                           bands_file,
                           lanes_file,
                           ior_file_base),
                          -1);
      }

  return 0;
}

static void
write_iors_to_file (CORBA::Object_ptr object,
                    CORBA::ORB_ptr orb
                    ACE_ENV_ARG_DECL)
{
  char filename[BUFSIZ];
  sprintf (filename,
           "%s_%d",
           ior_file_base,
           ior_file_count);
  ior_file_count++;

  FILE *file =
    ACE_OS::fopen (filename, "w");
  ACE_ASSERT (file != 0);

  CORBA::String_var ior =
    orb->object_to_string (object
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  u_int result = 0;
  result =
    ACE_OS::fprintf (file,
                     "%s",
                     ior.in ());

  ACE_ASSERT (result == ACE_OS::strlen (ior.in ()));
  ACE_UNUSED_ARG (result);

  ACE_OS::fclose (file);
}

void
vanilla_poa (CORBA::ORB_ptr orb,
             PortableServer::POA_ptr root_poa,
             PortableServer::POAManager_ptr poa_manager
             ACE_ENV_ARG_DECL)
{
  CORBA::PolicyList policies;

  CORBA::Policy_var implicit_activation_policy =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    implicit_activation_policy;

  PortableServer::POA_var poa =
    root_poa->create_POA ("child",
                          poa_manager,
                          policies
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (orb,
                            poa.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant (servant);

  test_var test =
    servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  write_iors_to_file (test.in (),
                      orb
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
rt_poa (CORBA::ORB_ptr orb,
        RTCORBA::RTORB_ptr rt_orb,
        PortableServer::POA_ptr root_poa,
        PortableServer::POAManager_ptr poa_manager
        ACE_ENV_ARG_DECL)
{
  CORBA::PolicyList policies;

  int result =
    get_priority_bands ("server",
                        bands_file,
                        rt_orb,
                        policies,
                        debug
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error in parsing bands data file: %s\n",
                  bands_file));
      return;
    }

  result =
    get_priority_lanes ("server",
                        lanes_file,
                        rt_orb,
                        stacksize,
                        static_threads,
                        dynamic_threads,
                        allow_request_buffering,
                        max_buffered_requests,
                        max_request_buffer_size,
                        allow_borrowing,
                        policies,
                        debug
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error in parsing lanes data file: %s\n",
                  lanes_file));
      return;
    }

  CORBA::Policy_var priority_model_policy =
    rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                          0
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Implicit_activation policy.
  CORBA::Policy_var implicit_activation_policy =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    priority_model_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    implicit_activation_policy;

  PortableServer::POA_var poa =
    root_poa->create_POA ("rt_poa",
                          poa_manager,
                          policies
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (orb,
                            poa.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant (servant);

  test_var test =
    servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  write_iors_to_file (test.in (),
                      orb
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

class Task : public ACE_Task_Base
{
public:

  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb);

  int svc (void);

  CORBA::ORB_var orb_;

};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        this->orb_->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      vanilla_poa (this->orb_.in (),
                   root_poa.in (),
                   poa_manager.in ()
                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      rt_poa (this->orb_.in (),
              rt_orb.in (),
              root_poa.in (),
              poa_manager.in ()
              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
main (int argc, char **argv)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      check_supported_priorities (orb.in());

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      result =
        task.activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %s\n"
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
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
