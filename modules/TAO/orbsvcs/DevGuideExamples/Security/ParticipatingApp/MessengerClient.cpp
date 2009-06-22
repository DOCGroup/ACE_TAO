/* -*- C++ -*- $Id$ */

#include "orbsvcs/SecurityC.h"
#include "MessengerC.h"

#if 0
The servers service configuration file
for this example is:
---------------------------------------
# server.conf
dynamic SSLIOP_Factory Service_Object *
  TAO_SSLIOP:_make_TAO_SSLIOP_Protocol_Factory()
  "-SSLNoProtection \
   -SSLAuthenticate SERVER_AND_CLIENT \
   -SSLPrivateKey PEM:serverkey.pem \
   -SSLCertificate PEM:servercert.pem"

static Resource_Factory "-ORBProtocolFactory SSLIOP_Factory"
---------------------------------------

The clients service configuration file
for this example is:
---------------------------------------
# client.conf
dynamic SSLIOP_Factory Service_Object *
  TAO_SSLIOP:_make_TAO_SSLIOP_Protocol_Factory()
  "-SSLNoProtection  \
   -SSLAuthenticate SERVER \
   -SSLPrivateKey PEM:clientkey.pem  \
   -SSLCertificate PEM:clientcert.pem"

static Resource_Factory "-ORBProtocolFactory SSLIOP_Factory"
---------------------------------------
#endif

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {

    CORBA::ORB_var orb =
      CORBA::ORB_init( argc, argv );

    CORBA::Object_var obj =
      orb->string_to_object( "file://Messenger.ior" );

    Messenger_var messenger =
      Messenger::_narrow( obj.in() );

    CORBA::String_var message =
      CORBA::string_dup( "Terminating messenger service!" );

    messenger->send_message( "Chief of Security",
                             "New Directive",
                             message.inout() );

    messenger->shutdown("Chief of Security");

    Security::QOP qop =
      Security::SecQOPIntegrityAndConfidentiality;

    CORBA::Any want_protection;
    want_protection <<= qop;

    CORBA::Policy_var policy =
      orb->create_policy (Security::SecQOPPolicy,
                          want_protection);

    Security::EstablishTrust establish_trust;
    establish_trust.trust_in_client = 0;
    establish_trust.trust_in_target = 1;

    CORBA::Any want_trust;
    want_trust <<= establish_trust;

    CORBA::Policy_var policy2 =
      orb->create_policy (Security::SecEstablishTrustPolicy,
                          want_trust);


    CORBA::PolicyList policy_list (2);
    policy_list.length (1);
    policy_list[0] =
      CORBA::Policy::_duplicate (policy.in ());
    policy_list.length (2);
    policy_list[1] =
      CORBA::Policy::_duplicate (policy2.in ());


    CORBA::Object_var object =
      obj->_set_policy_overrides (policy_list,
                                  CORBA::SET_OVERRIDE);

    Messenger_var messenger2 =
      Messenger::_narrow( object.in() );

    message =
      CORBA::string_dup( "Terminating messenger service!" );

    messenger2->send_message( "Chief of Security",
                             "New Directive",
                             message.inout() );

    messenger2->shutdown("Chief of Security");

    orb->destroy();
  }
  catch(const CORBA::Exception& ex)
  {
    ex._tao_print_exception("Client: main block");
    return 1;
  }

  return 0;
}
