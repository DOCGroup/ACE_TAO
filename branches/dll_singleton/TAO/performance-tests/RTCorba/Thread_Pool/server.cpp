// $Id$

#include "ace/Get_Opt.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "testS.h"
#include "tests/RTCORBA/Linear_Priority/readers.cpp"
#include "fudge_priorities.cpp"

ACE_RCSID(Thread_Pools, server, "$Id$")

class test_i :
  public POA_test,
  public PortableServer::RefCountServantBase
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);

  void method (CORBA::ULong work,
               CORBA::ULong prime_number
               ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
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
test_i::method (CORBA::ULong work,
                CORBA::ULong prime_number
                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
test_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

static const char *ior_output_file = "ior";
static CORBA::ULong static_threads = 1;
static CORBA::ULong dynamic_threads = 0;
static CORBA::ULong number_of_lanes = 0;
static RTCORBA::Priority default_thread_priority = 0;
static RTCORBA::Priority pool_priority = ACE_INT16_MIN;

static const char *bands_file = "empty-file";
static const char *lanes_file = "empty-file";

int
parse_args (int argc, char *argv[])
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
write_ior_to_file (const char *ior_file,
                   CORBA::ORB_ptr orb,
                   CORBA::Object_ptr object
                   ACE_ENV_ARG_DECL)
{
  CORBA::String_var ior =
    orb->object_to_string (object
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

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

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      fudge_priorities (orb.in ());

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("RTORB"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("RTCurrent"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      default_thread_priority =
        current->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

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
                                             policies
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if (ACE_OS::strcmp (lanes_file, "empty-file") != 0)
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
                                policies
                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (result != 0)
            return result;

          result =
            get_priority_bands ("server",
                                bands_file,
                                rt_orb.in (),
                                policies
                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

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
                                       max_request_buffer_size
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          policies.length (policies.length () + 1);
          policies[policies.length () - 1] =
            rt_orb->create_threadpool_policy (threadpool_id
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (ACE_OS::strcmp (bands_file, "empty-file") != 0)
            {
              result =
                get_priority_bands ("server",
                                    bands_file,
                                    rt_orb.in (),
                                    policies
                                    ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              if (result != 0)
                return result;
            }
        }

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        root_poa->create_implicit_activation_policy
        (PortableServer::IMPLICIT_ACTIVATION
         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              default_thread_priority
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa =
        root_poa->create_POA ("RT POA",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_i *servant =
        new test_i (orb.in (),
                    poa.in ());

      PortableServer::ServantBase_var safe_servant (servant);
      ACE_UNUSED_ARG (safe_servant);

      test_var test =
        servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      result =
        write_ior_to_file (ior_output_file,
                           orb.in (),
                           test.in ()
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result != 0)
        return result;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
