#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB_Constants.h"
#include "tao/Policy_CurrentC.h"
#include "tao/debug.h"
#include "testS.h"
#include <memory>
#include <cstring>

struct Options
{
  ACE_TCHAR const *ior_file {ACE_TEXT ("distributor.ior")};
  ACE_TCHAR const *ior {ACE_TEXT ("file://receiver.ior")};
};

static Options options;
static int const number_of_connection_attempts {20};

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'f':
        options.ior_file = get_opts.opt_arg ();
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s\n"
                           "\t-f <ior_file> (defaults to %s)\n"
                           "\t-k <ior> (defaults to %s)\n"
                           "\n",
                           argv[0],
                           options.ior_file,
                           options.ior),
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

  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;
  ~test_i () override = default;

  void start_test (CORBA::Long session_id,
                   char const *protocol,
                   CORBA::ULong invocation_rate,
                   CORBA::ULong message_size,
                   CORBA::ULong iterations) override;

  void end_test () override;

  void oneway_sync () override;

  void twoway_sync () override;

  void oneway_method (CORBA::Long session_id,
                      CORBA::ULong iteration,
                      ::test::octets const &payload) override;

  void twoway_method (CORBA::Long &session_id,
                      CORBA::ULong &iteration,
                      ::test::octets &payload) override;

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override;
  //FUZZ: enable check_for_lack_ACE_OS

  PortableServer::POA_ptr _default_POA () override;

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

void
test_i::start_test (CORBA::Long session_id,
                    char const *protocol,
                    CORBA::ULong invocation_rate,
                    CORBA::ULong message_size,
                    CORBA::ULong iterations)
{
  RTCORBA::ProtocolList protocols;
  protocols.length (1);
  protocols[0].transport_protocol_properties =
    RTCORBA::ProtocolProperties::_nil ();
  protocols[0].orb_protocol_properties =
    RTCORBA::ProtocolProperties::_nil ();

  if (std::strcmp (protocol, "DIOP") == 0)
    {
      if (TAO_debug_level) ACE_DEBUG ((LM_DEBUG, "test protocol is DIOP\n"));
      protocols[0].protocol_type = TAO_TAG_DIOP_PROFILE;
    }
  else if (std::strcmp (protocol, "SCIOP") == 0)
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
                                               CORBA::SET_OVERRIDE);

  // Since the network maybe unavailable temporarily, make sure to try
  // for a few times before giving up.
  for (int j = 0;;)
    {
    test_protocol_setup:

      try
        {
          // Send a message to ensure that the connection is setup.
          this->receiver_->oneway_sync ();

          goto test_protocol_success;
        }
      catch (CORBA::TRANSIENT const&)
        {
          ++j;

          if (j < number_of_connection_attempts)
            {
              ACE_OS::sleep (1);
              goto test_protocol_setup;
            }
        }

      ACE_ERROR ((LM_ERROR,
                  "Cannot setup test protocol\n"));

      throw CORBA::TRANSIENT (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
    }

 test_protocol_success:

  // Use IIOP for setting up the test since the test protocol maybe
  // unreliable.
  this->policy_manager_->set_policy_overrides (this->base_protocol_policy_,
                                               CORBA::SET_OVERRIDE);

  // Since the network maybe unavailable temporarily, make sure to try
  // for a few times before giving up.
  for (int k = 0;;)
    {
    base_protocol_setup:

      try
        {
          // Let the server know what to expect..
          this->receiver_->start_test (session_id,
                                       protocol,
                                       invocation_rate,
                                       message_size,
                                       iterations);

          goto base_protocol_success;
        }
      catch (CORBA::TRANSIENT const&)
        {
          ACE_OS::sleep (1);

          if (k < number_of_connection_attempts)
            {
              ACE_OS::sleep (1);
              goto base_protocol_setup;
            }
        }

      ACE_ERROR ((LM_ERROR,
                  "Cannot setup base protocol\n"));

      throw CORBA::TRANSIENT (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
    }

 base_protocol_success:

  // Select the test protocol for these invocation.
  this->policy_manager_->set_policy_overrides (this->test_protocol_policy_,
                                               CORBA::SET_OVERRIDE);
}

void
test_i::end_test ()
{
  // Use IIOP to indicate end of test to server.
  this->policy_manager_->set_policy_overrides (this->base_protocol_policy_,
                                               CORBA::SET_OVERRIDE);

  this->receiver_->end_test ();
}

void
test_i::oneway_sync ()
{
  this->receiver_->oneway_sync ();
}

void
test_i::twoway_sync ()
{
  this->receiver_->twoway_sync ();
}

void
test_i::oneway_method (CORBA::Long session_id,
                       CORBA::ULong iteration,
                       ::test::octets const &payload)
{
  this->receiver_->oneway_method (session_id,
                                  iteration,
                                  payload);
}

void
test_i::twoway_method (CORBA::Long &session_id,
                       CORBA::ULong &iteration,
                       ::test::octets &payload)
{
  this->receiver_->twoway_method (session_id,
                                  iteration,
                                  payload);
}

PortableServer::POA_ptr
test_i::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::shutdown ()
{
  this->receiver_->shutdown ();

  this->orb_->shutdown (false);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rtorb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        orb->resolve_initial_references ("ORBPolicyManager");

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (object.in ());

      int const parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      object =
        orb->string_to_object (options.ior);

      test_var receiver =
        test::_narrow (object.in ());

      test_i *servant {};
      ACE_NEW_RETURN (servant,
                      test_i (orb.in (),
                              root_poa.in (),
                              rtorb.in (),
                              policy_manager.in (),
                              receiver.in ()),
                      -1);
      PortableServer::ServantBase_var safe_servant (servant);
      ACE_UNUSED_ARG (safe_servant);

      test_var test =
        servant->_this ();

      CORBA::String_var ior =
        orb->object_to_string (test.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_file, "w"), ACE_OS::fclose);
        ACE_ASSERT (output_file != nullptr);

        u_int const result =
          ACE_OS::fprintf (output_file.get (),
                           "%s",
                           ior.in ());
        ACE_ASSERT (result == ACE_OS::strlen (ior.in ()));
        ACE_UNUSED_ARG (result);
      }

      poa_manager->activate ();

      orb->run ();
      ACE_OS::sleep(1);
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
