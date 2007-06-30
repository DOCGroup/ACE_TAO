// $Id$

#include "MessengerC.h"
#include "tao/CodecFactory/CodecFactory.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    // Destringify ior
    CORBA::Object_var obj = orb->string_to_object( "file://Messenger.ior" );

    // Narrow
    Messenger_var messenger = Messenger::_narrow( obj.in() );
    if(CORBA::is_nil( messenger.in() ) ) {
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil reference\n"),
                          1);
      }

    // Obtain a reference to the CodecFactory.
    CORBA::Object_var obj2 =
    	orb->resolve_initial_references ("CodecFactory");

    IOP::CodecFactory_var codec_factory =
      IOP::CodecFactory::_narrow (obj2.in ());

    // Set up a structure that contains information necessary to
    // create a GIOP 1.2 CDR encapsulation Codec.
    IOP::Encoding encoding;
    encoding.format = IOP::ENCODING_CDR_ENCAPS;
    encoding.major_version = 1;
    encoding.minor_version = 2;

    // Obtain the CDR encapsulation Codec.
    IOP::Codec_var codec =
       codec_factory->create_codec (encoding);

    CORBA::String_var user_name = CORBA::string_dup ("Ru");

    CORBA::Any user_name_as_any;
    user_name_as_any <<= user_name.in ();

    CORBA::OctetSeq_var client_user_name = codec->encode (user_name_as_any);

    messenger->send_message (client_user_name.in ());

    messenger->shutdown ();

    orb->destroy ();
  }
  catch( const CORBA::SystemException &ex ) {
    ex._tao_print_exception ("Caught CORBA exception: ");
    return 1;
  }
  catch( ... ) {
    return 1;
  }

  return 0;
}
