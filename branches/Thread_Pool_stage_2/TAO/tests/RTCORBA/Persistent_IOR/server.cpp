// $Id$

#include "ace/Get_Opt.h"
#include "testS.h"
#include "tao/RTPortableServer/RTPortableServer.h"

class test_i :
  public POA_test,
  public PortableServer::RefCountServantBase
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);

  void method (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (CORBA_Environment &ACE_TRY_ENV);

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
test_i::method (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::method\n"));
}

void
test_i::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::shutdown\n"));

  this->orb_->shutdown (0,
                        ACE_TRY_ENV);
  ACE_CHECK;
}

PortableServer::POA_ptr
test_i::_default_POA (CORBA_Environment &)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

static const char *persistent_ior = "persistent_ior";
static const char *transient_ior = "transient_ior";
static int debug = 0;

static CORBA::ULong stacksize = 0;
static CORBA::ULong static_threads = 2;
static CORBA::ULong dynamic_threads = 2;
static RTCORBA::Priority default_thread_priority =
RTCORBA::Priority (ACE_DEFAULT_THREAD_PRIORITY);
static CORBA::Boolean allow_request_buffering = 0;
static CORBA::ULong max_buffered_requests = 0;
static CORBA::ULong max_request_buffer_size = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "p:t:d:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        persistent_ior = get_opts.optarg;
        break;

      case 't':
        transient_ior = get_opts.optarg;
        break;

      case 'd':
        debug = ACE_OS::atoi (get_opts.optarg);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-p <persistent ior file> "
                           "-t <transient ior file> "
                           "-d <debug> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

static void
write_iors_to_file (CORBA::Object_ptr object,
                    CORBA::ORB_ptr orb,
                    const char *filename,
                    CORBA::Environment &ACE_TRY_ENV)
{
  FILE *file =
    ACE_OS::fopen (filename, "w");
  ACE_ASSERT (file != 0);

  CORBA::String_var ior =
    orb->object_to_string (object,
                           ACE_TRY_ENV);
  ACE_CHECK;

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "%s\n",
                ior.in ()));

  u_int result = 0;
  result =
    ACE_OS::fprintf (file,
                     "%s",
                     ior.in ());

  ACE_ASSERT (result == ACE_OS::strlen (ior.in ()));

  ACE_OS::fclose (file);
}

void
create_poa_and_servant_with_persistent_policy (CORBA::ORB_ptr orb,
                                               PortableServer::POA_ptr root_poa,
                                               PortableServer::POAManager_ptr poa_manager,
                                               CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::PolicyList policies;

  CORBA::Policy_var implicit_activation_policy =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION,
                                                 ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Policy_var lifespan_policy =
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT,
                                      ACE_TRY_ENV);
  ACE_CHECK;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    implicit_activation_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    lifespan_policy;

  PortableServer::POA_var poa =
    root_poa->create_POA ("persistent_child",
                          poa_manager,
                          policies,
                          ACE_TRY_ENV);
  ACE_CHECK;

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (orb,
                            poa.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant (servant);

  test_var test =
    servant->_this (ACE_TRY_ENV);
  ACE_CHECK;

  write_iors_to_file (test.in (),
                      orb,
                      persistent_ior,
                      ACE_TRY_ENV);
  ACE_CHECK;
}

void
create_poa_and_servant_with_tp_policy (CORBA::ORB_ptr orb,
                                       RTCORBA::RTORB_ptr rt_orb,
                                       PortableServer::POA_ptr root_poa,
                                       PortableServer::POAManager_ptr poa_manager,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  RTCORBA::ThreadpoolId threadpool_id =
    rt_orb->create_threadpool (stacksize,
                               static_threads,
                               dynamic_threads,
                               default_thread_priority,
                               allow_request_buffering,
                               max_buffered_requests,
                               max_request_buffer_size,
                               ACE_TRY_ENV);
  ACE_TRY_CHECK;

  CORBA::Policy_var threadpool_policy =
    rt_orb->create_threadpool_policy (threadpool_id,
                                      ACE_TRY_ENV);
  ACE_TRY_CHECK;

  CORBA::Policy_var implicit_activation_policy =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION,
                                                 ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::PolicyList policies;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    implicit_activation_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    threadpool_policy;

  PortableServer::POA_var poa =
    root_poa->create_POA ("tp_child",
                          poa_manager,
                          policies,
                          ACE_TRY_ENV);
  ACE_CHECK;

  test_i *servant = 0;
  ACE_NEW_THROW_EX (servant,
                    test_i (orb,
                            poa.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant (servant);

  test_var test =
    servant->_this (ACE_TRY_ENV);
  ACE_CHECK;

  write_iors_to_file (test.in (),
                      orb,
                      transient_ior,
                      ACE_TRY_ENV);
  ACE_CHECK;
}

void
create_poa_with_tp_and_persistent_policy (RTCORBA::RTORB_ptr rt_orb,
                                          PortableServer::POA_ptr root_poa,
                                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::PolicyList policies;

      RTCORBA::ThreadpoolId threadpool_id =
        rt_orb->create_threadpool (stacksize,
                                   static_threads,
                                   dynamic_threads,
                                   default_thread_priority,
                                   allow_request_buffering,
                                   max_buffered_requests,
                                   max_request_buffer_size,
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Policy_var threadpool_policy =
        rt_orb->create_threadpool_policy (threadpool_id,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Policy_var lifespan_policy =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT,
                                          ACE_TRY_ENV);
      ACE_CHECK;

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        lifespan_policy;

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        threadpool_policy;

      PortableServer::POA_var invalid_poa =
        root_poa->create_POA ("invalid",
                              PortableServer::POAManager::_nil (),
                              policies,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // This next line of code should not run because an exception
      // should have been raised.
      ACE_DEBUG ((LM_DEBUG, "ERROR: no exception caught\n"));
    }
  ACE_CATCH (PortableServer::POA::InvalidPolicy, ex)
    {
      // Expected exception.
      ACE_DEBUG ((LM_DEBUG,
                  "InvalidPolicy exception is caught as expected.\n"));
    }
  ACE_CATCHANY
    {
      // Unexpected exception.
      ACE_DEBUG ((LM_DEBUG, "ERROR: unexpected exception caught\n"));
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

int
main (int argc, char **argv)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in (),
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("RootPOA",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      create_poa_and_servant_with_tp_policy (orb.in (),
                                             rt_orb.in (),
                                             root_poa.in  (),
                                             poa_manager.in (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      create_poa_and_servant_with_persistent_policy (orb.in (),
                                                     root_poa.in  (),
                                                     poa_manager.in (),
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      create_poa_with_tp_and_persistent_policy (rt_orb.in (),
                                                root_poa.in  (),
                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
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
