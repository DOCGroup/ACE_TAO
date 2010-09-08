// $Id$

#include "ace/Get_Opt.h"
#include "ace/Stats.h"
#include "ace/Task.h"
#include "ace/Sample_History.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "testS.h"
#include "tests/RTCORBA/common_args.cpp"
#include "tests/RTCORBA/check_supported_priorities.cpp"

class test_i :
  public POA_test
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);

  void method (CORBA::ULong work,
               CORBA::ULong prime_number);

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (void);
  //FUZZ: enable check_for_lack_ACE_OS

  PortableServer::POA_ptr _default_POA (void);

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
test_i::method (CORBA::ULong work,
                CORBA::ULong prime_number)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "test_i::method: %hd units of work\n",
                work));

  for (; work != 0; work--)
    ACE::is_prime (prime_number,
                   2,
                   prime_number / 2);
}

PortableServer::POA_ptr
test_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

static const ACE_TCHAR *ior_output_file = ACE_TEXT("ior");
static CORBA::ULong static_threads = 1;
static CORBA::ULong dynamic_threads = 0;
static CORBA::ULong number_of_lanes = 0;
static RTCORBA::Priority default_thread_priority = 0;
static RTCORBA::Priority pool_priority = ACE_INT16_MIN;

static const ACE_TCHAR *bands_file = ACE_TEXT("empty-file");
static const ACE_TCHAR *lanes_file = ACE_TEXT("empty-file");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv,
                        "b:f:hl:n:o:s:" // server options
                        "c:e:g:hi:j:k:m:p:q:r:t:u:v:w:x:y:z:" // client options
                        );
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'b':
        bands_file = get_opts.opt_arg ();
        break;

      case 'f':
        pool_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'l':
        lanes_file = get_opts.opt_arg ();
        break;

      case 'n':
        number_of_lanes = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 's':
        static_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'c':
      case 'e':
      case 'g':
      case 'i':
      case 'j':
      case 'k':
      case 'm':
      case 'p':
      case 'q':
      case 'r':
      case 't':
      case 'u':
      case 'v':
      case 'w':
      case 'x':
      case 'y':
      case 'z':
        // client options: ignored.
        break;

      case 'h':
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-b <bands file> (defaults to %s)\n"
                           "\t-f <pool priority> (defaults to %d)\n"
                           "\t-h <help: shows options menu>\n"
                           "\t-l <lanes file> (defaults to %s)\n"
                           "\t-n <number of lanes> (defaults to %d)\n"
                           "\t-o <ior file> (defaults to %s)\n"
                           "\t-s <static threads> (defaults to %d)\n"
                           "\n",
                           argv [0],
                           bands_file,
                           default_thread_priority,
                           lanes_file,
                           number_of_lanes,
                           ior_output_file,
                           static_threads),
                          -1);
      }

  return 0;
}

int
write_ior_to_file (const ACE_TCHAR *ior_file,
                   CORBA::ORB_ptr orb,
                   CORBA::Object_ptr object)
{
  CORBA::String_var ior =
    orb->object_to_string (object);

  FILE *output_file =
    ACE_OS::fopen (ior_file,
                   "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing IOR: %s",
                       ior_file),
                      -1);

  ACE_OS::fprintf (output_file,
                   "%s",
                   ior.in ());

  ACE_OS::fclose (output_file);

  return 0;
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
  try
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ());

      default_thread_priority =
        current->the_priority ();

      int result = 0;
      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;

      CORBA::PolicyList policies;

      CORBA::Boolean allow_borrowing = 0;
      if (number_of_lanes != 0)
        {
          get_auto_priority_lanes_and_bands (number_of_lanes,
                                             rt_orb.in (),
                                             stacksize,
                                             static_threads,
                                             dynamic_threads,
                                             allow_request_buffering,
                                             max_buffered_requests,
                                             max_request_buffer_size,
                                             allow_borrowing,
                                             policies,
                                             1);
        }
      else if (ACE_OS::strcmp (lanes_file, ACE_TEXT("empty-file")) != 0)
        {
          result =
            get_priority_lanes ("server",
                                lanes_file,
                                rt_orb.in (),
                                stacksize,
                                static_threads,
                                dynamic_threads,
                                allow_request_buffering,
                                max_buffered_requests,
                                max_request_buffer_size,
                                allow_borrowing,
                                policies,
                                1);

          if (result != 0)
            return result;

          result =
            get_priority_bands ("server",
                                bands_file,
                                rt_orb.in (),
                                policies,
                                1);

          if (result != 0)
            return result;
        }
      else
        {
          if (pool_priority == ACE_INT16_MIN)
            pool_priority =
              default_thread_priority;

          RTCORBA::ThreadpoolId threadpool_id =
            rt_orb->create_threadpool (stacksize,
                                       static_threads,
                                       dynamic_threads,
                                       pool_priority,
                                       allow_request_buffering,
                                       max_buffered_requests,
                                       max_request_buffer_size);

          policies.length (policies.length () + 1);
          policies[policies.length () - 1] =
            rt_orb->create_threadpool_policy (threadpool_id);

          if (ACE_OS::strcmp (bands_file, ACE_TEXT("empty-file")) != 0)
            {
              result =
                get_priority_bands ("server",
                                    bands_file,
                                    rt_orb.in (),
                                    policies,
                                    1);

              if (result != 0)
                return result;
            }
        }

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        root_poa->create_implicit_activation_policy
        (PortableServer::IMPLICIT_ACTIVATION);

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              default_thread_priority);

      PortableServer::POA_var poa =
        root_poa->create_POA ("RT POA",
                              poa_manager.in (),
                              policies);

      test_i *servant =
        new test_i (this->orb_.in (),
                    poa.in ());

      PortableServer::ServantBase_var safe_servant (servant);
      ACE_UNUSED_ARG (safe_servant);

      test_var test =
        servant->_this ();

      result =
        write_ior_to_file (ior_output_file,
                           this->orb_.in (),
                           test.in ());

      if (result != 0)
        return result;

      poa_manager->activate ();

      this->orb_->run ();

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      check_supported_priorities (orb.in ());

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
      ACE_ASSERT (result != -1);
      ACE_UNUSED_ARG (result);

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
