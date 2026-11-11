/* -*- C++ -*-  */

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"

#include "MessengerC.h"
#include "orbsvcs/SecurityC.h"

// Policy Example 1
// ================
//
// Example of a client that downgrades
// from message protection to no message
// protection and upgrades from no
// peer authentication to authentication
// of targets, i.e., authentication of
// servers.
//
// The server's service configuration file
// for this example is
//
// # server.conf
// dynamic SSLIOP_Factory Service_Object *
//   TAO_SSLIOP:_make_TAO_SSLIOP_Protocol_Factory()
//   "-SSLNoProtection
//   -SSLAuthenticate SERVER_AND_CLIENT
//   -SSLPrivateKey PEM:server_key.pem
//   -SSLCertificate PEM:server_cert.pem"
//
// static Resource_Factory "-ORBProtocolFactory SSLIOP_Factory"
//
// The clients service configuration file
// for this example is:
//
// # client.conf
// dynamic SSLIOP_Factory Service_Object *
//   TAO_SSLIOP:_make_TAO_SSLIOP_Protocol_Factory()
//   "-SSLAuthenticate NONE
//   -SSLPrivateKey PEM:client_key.pem
//   -SSLCertificate PEM:client_cert.pem"
//
// static Resource_Factory "-ORBProtocolFactory SSLIOP_Factory"
//
// Policy Example 2
// ================
//
// Example of client upgrading from
// no message protection and no
// no authentication to message
// protection and authentication
// of targets, i.e., authentication
// of servers.
//
// The server's service configuration file for this example is
//
// # server.conf
// dynamic SSLIOP_Factory Service_Object *
//   TAO_SSLIOP:_make_TAO_SSLIOP_Protocol_Factory()
//   "-SSLAuthenticate SERVER_AND_CLIENT
//   -SSLPrivateKey PEM:serverkey.pem
//   -SSLCertificate PEM:servercert.pem"
//
// static Resource_Factory "-ORBProtocolFactory SSLIOP_Factory"
//
// The client's service configuration file
// for this example is:
//
// # client.conf
// dynamic SSLIOP_Factory Service_Object *
//   TAO_SSLIOP:_make_TAO_SSLIOP_Protocol_Factory()
//   "-SSLNoProtection
//   -SSLAuthenticate NONE
//   -SSLPrivateKey PEM:clientkey.pem
//   -SSLCertificate PEM:clientcert.pem"
//
// static Resource_Factory "-ORBProtocolFactory SSLIOP_Factory"
//

struct ClientOptions
{
  ACE_TCHAR const *ior { ACE_TEXT ("file://Messenger.ior") };
  int policy {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], ClientOptions &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("e:k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'e':
        options.policy = ACE_OS::atoi (get_opts.optarg);
        if (options.policy < 1 || 2 < options.policy)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage:  %s "
                             "-e [12] "
                             "-k <ior>"
                             "\n",
                             argv[0]),
                            -1);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-e [12] "
                           "-k <ior>"
                           "\n",
                           argv[0]),
                          -1);
      }
  if (options.policy == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Usage:  %s -e [12] -k <ior>\n",
                       argv[0]),
                      -1);
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ClientOptions options {};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (options.ior);
      Security::QOP const qop = options.policy == 1
        ? Security::SecQOPNoProtection
        : Security::SecQOPIntegrityAndConfidentiality;
      CORBA::Any protection {};
      protection <<= qop;

      Security::EstablishTrust establish_trust {};
      establish_trust.trust_in_client = false;
      establish_trust.trust_in_target = true;
      CORBA::Any trust {};
      trust <<= establish_trust;

      CORBA::Policy_var const protection_policy =
        orb->create_policy (Security::SecQOPPolicy, protection);
      CORBA::Policy_var const trust_policy =
        orb->create_policy (Security::SecEstablishTrustPolicy, trust);
      CORBA::PolicyList policy_list {};
      policy_list.length (2);
      policy_list[0] = CORBA::Policy::_duplicate (protection_policy.in ());
      policy_list[1] = CORBA::Policy::_duplicate (trust_policy.in ());

      CORBA::Object_var const object_with_policy =
        object->_set_policy_overrides (policy_list, CORBA::SET_OVERRIDE);
      Messenger_var const messenger = Messenger::_narrow (object_with_policy.in ());
      CORBA::String_var message = CORBA::string_dup ("Implementing security policy now!");
      messenger->send_message ("Chief of Security", "New Directive", message.inout ());
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client: main block");
      return 1;
    }

  return 0;
}
