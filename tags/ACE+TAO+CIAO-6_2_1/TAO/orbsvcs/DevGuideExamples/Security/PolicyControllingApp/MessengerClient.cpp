/* -*- C++ -*- $Id$ */

#include "ace/Get_Opt.h"

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

const ACE_TCHAR *ior = ACE_TEXT("file://Messenger.ior");

int which = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("e:k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'e':
        which = ACE_OS::atoi(get_opts.optarg);
    if(which < 1 || 2 < which)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Usage:  %s "
                               "-e [12] "
                               "-k <ior>"
                               "\n",
                               argv [0]),
                              -1);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-e [12] "
                           "-k <ior>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {

    CORBA::ORB_var orb =
      CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
      return 1;
    else if(which < 1 || 2 < which)
      return 1;

    CORBA::Object_var obj =
      orb->string_to_object( ior );

    Security::QOP            qop;
    CORBA::Any               protection;
    Security::EstablishTrust establish_trust;
    CORBA::Any               trust;
    CORBA::PolicyList        policy_list (2);

    if (which == 1)
    {
      qop = Security::SecQOPNoProtection;
      //qop = Security::SecQOPIntegrity;

      establish_trust.trust_in_client = 0;
      establish_trust.trust_in_target = 1;
    }
    else
    {
      qop = Security::SecQOPIntegrityAndConfidentiality;

      establish_trust.trust_in_client = 0;
      establish_trust.trust_in_target = 1;
    }

    protection <<= qop;
    trust      <<= establish_trust;

    CORBA::Policy_var policy =
    orb->create_policy (Security::SecQOPPolicy, protection);

    CORBA::Policy_var policy2 =
    orb->create_policy (Security::SecEstablishTrustPolicy, trust);

    policy_list.length (1);
    policy_list[0] = CORBA::Policy::_duplicate (policy.in ());
    policy_list.length (2);
    policy_list[1] = CORBA::Policy::_duplicate (policy2.in ());

    CORBA::Object_var object =
    obj->_set_policy_overrides (policy_list,
                                  CORBA::SET_OVERRIDE);

    Messenger_var messenger =
      Messenger::_narrow( object.in() );

    CORBA::String_var message =
      CORBA::string_dup( "Implementing security policy now!" );

    messenger->send_message( "Chief of Security",
                             "New Directive",
                             message.inout() );
  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception("Client: main block");
    return 1;
  }

  return 0;
}
