// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB_Constants.h"
#include "tao/debug.h"
#include "testS.h"

static const char *ior_file = "distributor.ior";
static const char *ior = "file://receiver.ior";
static int number_of_connection_attempts = 20;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'f':
        ior_file = get_opts.opt_arg ();
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s\n"
                           "\t-f <ior_file> (defaults to %s)\n"
                           "\t-k <ior> (defaults to %s)\n"
                           "\n",
                           argv[0],
                           ior_file,
                           ior),
                          -1);
      }

  return 0;
}

class test_i :
  public POA_test
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          RTCORBA::RTORB_ptr rtorb,
          CORBA::PolicyManager_ptr policy_manager,
          test_ptr receiver);

  ~test_i (void);

  void start_test (CORBA::Long session_id,
                   const char *protocol,
                   CORBA::ULong invocation_rate,
                   CORBA::ULong message_size,
                   CORBA::ULong iterations
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void end_test (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void oneway_sync (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void twoway_sync (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void oneway_method (CORBA::Long session_id,
                      CORBA::ULong iteration,
                      const ::test::octets &payload
                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void twoway_method (CORBA::Long &session_id,
                      CORBA::ULong &iteration,
                      ::test::octets &payload
                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  RTCORBA::RTORB_var rtorb_;
  CORBA::PolicyManager_var policy_manager_;
  test_var receiver_;

  CORBA::PolicyList base_protocol_policy_;
  CORBA::PolicyList test_protocol_policy_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                RTCORBA::RTORB_ptr rtorb,
                CORBA::PolicyManager_ptr policy_manager,
                test_ptr receiver)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    rtorb_ (RTCORBA::RTORB::_duplicate (rtorb)),
    policy_manager_ (CORBA::PolicyManager::_duplicate (policy_manager)),
    receiver_ (test::_duplicate (receiver))
{
  // Base protocol is used for setting up and tearing down the test.
  this->base_protocol_policy_.length (1);

  // Test protocol is the one being tested.
  this->test_protocol_policy_.length (1);

  RTCORBA::ProtocolList protocols;
  protocols.length (1);
  protocols[0].transport_protocol_properties =
    RTCORBA::ProtocolProperties::_nil ();
  protocols[0].orb_protocol_properties =
    RTCORBA::ProtocolProperties::_nil ();

  // IIOP is always used for the base protocol.
  protocols[0].protocol_type = 0;

  // User decides the test protocol.
  this->base_protocol_policy_[0] =
    this->rtorb_->create_client_protocol_policy (protocols);
}

test_i::~test_i (void)
{
}

void
test_i::start_test (CORBA::Long session_id,
                    const char *protocol,
                    CORBA::ULong invocation_rate,
                    CORBA::ULong message_size,
                    CORBA::ULong iterations
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  RTCORBA::ProtocolList protocols;
  protocols.length (1);
  protocols[0].transport_protocol_properties =
    RTCORBA::ProtocolProperties::_nil ();
  protocols[0].orb_protocol_properties =
    RTCORBA::ProtocolProperties::_nil ();

  if (ACE_OS::strcmp (protocol, "DIOP") == 0)
    {
      if (TAO_debug_level) ACE_DEBUG ((LM_DEBUG, "test protocol is DIOP\n"));
      protocols[0].protocol_type = TAO_TAG_DIOP_PROFILE;
    }
  else if (ACE_OS::strcmp (protocol, "SCIOP") == 0)
    {
      if (TAO_debug_level) ACE_DEBUG ((LM_DEBUG, "test protocol is SCIOP\n"));
      protocols[0].protocol_type = TAO_TAG_SCIOP_PROFILE;
    }
  else
    {
      if (TAO_debug_level) ACE_DEBUG ((LM_DEBUG, "test protocol is IIOP\n"));
      protocols[0].protocol_type = 0;
    }

  this->test_protocol_policy_[0] =
    this->rtorb_->create_client_protocol_policy (protocols);

  // Make sure we have a connection to the server using the test
  // protocol.
  this->policy_manager_->set_policy_overrides (this->test_protocol_policy_,
                                               CORBA::SET_OVERRIDE
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Since the network maybe unavailable temporarily, make sure to try
  // for a few times before giving up.
  for (int j = 0;;)
    {

    test_protocol_setup:

      ACE_TRY_EX (B1)
        {
          // Send a message to ensure that the connection is setup.
          this->receiver_->oneway_sync (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (B1);

          goto test_protocol_success;
        }
      ACE_CATCH (CORBA::TRANSIENT, exception)
        {
          ++j;

          if (j < number_of_connection_attempts)
            {
              ACE_OS::sleep (1);
              goto test_protocol_setup;
            }
        }
      ACE_ENDTRY;

      ACE_ERROR ((LM_ERROR,
                  "Cannot setup test protocol\n"));

      ACE_THROW (CORBA::TRANSIENT (CORBA::OMGVMCID | 2,
                                   CORBA::COMPLETED_NO));
    }

 test_protocol_success:

  // Use IIOP for setting up the test since the test protocol maybe
  // unreliable.
  this->policy_manager_->set_policy_overrides (this->base_protocol_policy_,
                                               CORBA::SET_OVERRIDE
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Since the network maybe unavailable temporarily, make sure to try
  // for a few times before giving up.
  for (int k = 0;;)
    {

    base_protocol_setup:

      ACE_TRY_EX (B2)
        {
          // Let the server know what to expect..
          this->receiver_->start_test (session_id,
                                       protocol,
                                       invocation_rate,
                                       message_size,
                                       iterations
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (B2);

          goto base_protocol_success;
        }
      ACE_CATCH (CORBA::TRANSIENT, exception)
        {
          ACE_OS::sleep (1);

          if (k < number_of_connection_attempts)
            {
              ACE_OS::sleep (1);
              goto base_protocol_setup;
            }
        }
      ACE_ENDTRY;

      ACE_ERROR ((LM_ERROR,
                  "Cannot setup base protocol\n"));

      ACE_THROW (CORBA::TRANSIENT (CORBA::OMGVMCID | 2,
                                   CORBA::COMPLETED_NO));
    }

 base_protocol_success:

  // Select the test protocol for these invocation.
  this->policy_manager_->set_policy_overrides (this->test_protocol_policy_,
                                               CORBA::SET_OVERRIDE
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
test_i::end_test (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Use IIOP to indicate end of test to server.
  this->policy_manager_->set_policy_overrides (this->base_protocol_policy_,
                                               CORBA::SET_OVERRIDE
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->receiver_->end_test (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
test_i::oneway_sync (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->receiver_->oneway_sync (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
test_i::twoway_sync (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->receiver_->twoway_sync (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
test_i::oneway_method (CORBA::Long session_id,
                       CORBA::ULong iteration,
                       const ::test::octets &payload
                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->receiver_->oneway_method (session_id,
                                  iteration,
                                  payload
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
test_i::twoway_method (CORBA::Long &session_id,
                       CORBA::ULong &iteration,
                       ::test::octets &payload
                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->receiver_->twoway_method (session_id,
                                  iteration,
                                  payload
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
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
  this->receiver_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
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
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rtorb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("ORBPolicyManager"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (object.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

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

      object =
        orb->string_to_object (ior
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var receiver =
        test::_narrow (object.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_i *servant =
        new test_i (orb.in (),
                    root_poa.in (),
                    rtorb.in (),
                    policy_manager.in (),
                    receiver.in ());
      PortableServer::ServantBase_var safe_servant (servant);
      ACE_UNUSED_ARG (safe_servant);

      test_var test =
        servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (test.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      FILE *output_file =
        ACE_OS::fopen (ior_file, "w");
      ACE_ASSERT (output_file != 0);

      u_int result =
        ACE_OS::fprintf (output_file,
                         "%s",
                         ior.in ());
      ACE_ASSERT (result == ACE_OS::strlen (ior.in ()));
      ACE_UNUSED_ARG (result);

      ACE_OS::fclose (output_file);

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
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
