// $Id$

#include "ace/Get_Opt.h"
#include "testS.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"
#include "./readers.cpp"

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
  ACE_DEBUG ((LM_DEBUG,
              "test_i::method\n"));
}

void
test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
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

static CORBA::ULong stacksize = 0;
static CORBA::ULong static_threads = 2;
static CORBA::ULong dynamic_threads = 0;
static CORBA::Boolean allow_request_buffering = 0;
static CORBA::ULong max_buffered_requests = 0;
static CORBA::ULong max_request_buffer_size = 0;
static CORBA::Boolean allow_borrowing = 0;

static const char *ior = "ior";

static const char *bands_file = "bands";
static const char *lanes_file = "lanes";

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "b:l:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'b':
        bands_file = get_opts.opt_arg ();
        break;

      case 'l':
        lanes_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-b <bands_file> "
                           "-l <lanes_file> "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

static void
write_iors_to_file (CORBA::Object_ptr object,
                    CORBA::ORB_ptr orb,
                    const char *filename
                    ACE_ENV_ARG_DECL)
{
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

int
main (int argc, char **argv)
{
  // Make sure we can support multiple priorities that are required
  // for this test.
  check_supported_priorities ();

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      object =
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

      CORBA::PolicyList policies;

      result =
        get_priority_bands ("server",
                            bands_file,
                            rt_orb.in (),
                            policies
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result != 0)
        return result;

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

      CORBA::Policy_var priority_model_policy =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              0
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Implicit_activation policy.
      CORBA::Policy_var implicit_activation_policy =
        root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        priority_model_policy;

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        implicit_activation_policy;

      PortableServer::POA_var poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_i *servant = 0;
      ACE_NEW_THROW_EX (servant,
                        test_i (orb.in (),
                                poa.in ()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_servant (servant);

      test_var test =
        servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      write_iors_to_file (test.in (),
                          orb.in (),
                          ior
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
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
