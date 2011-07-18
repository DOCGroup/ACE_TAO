// $Id$

#include "MessengerC.h"
#include "ClientInitializer.h"
#include "tao/ORBInitializer_Registry.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include <iostream>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      temp_initializer = new ClientInitializer;

      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

      // Destringify ior
      CORBA::Object_var obj = orb->string_to_object( "file://Messenger.ior" );
      if( CORBA::is_nil( obj.in() ) ) {
        std::cerr << "Nil Messenger reference" << std::endl;
        return 1;
      }

      // Narrow
      Messenger_var messenger = Messenger::_narrow( obj.in() );
      if( CORBA::is_nil( messenger.in() ) ) {
        std::cerr << "Argument is not a Messenger reference" << std::endl;
        return 1;
      }

      // Obtain a reference to the CodecFactory.
      CORBA::Object_var obj2 =
      orb->resolve_initial_references ("CodecFactory");

      IOP::CodecFactory_var codec_factory;

      if(CORBA::is_nil(obj2.in()))
        {
          std::cerr <<  "Error: codec_factory" << std::endl;
          ACE_OS::exit(1);
        }
      else
        {
          codec_factory = IOP::CodecFactory::_narrow (obj2.in ());
          std::cout << "got codec factory" << std::endl;
        }

      // Set up a structure that contains information necessary to
      // create a GIOP 1.2 CDR encapsulation Codec.
      IOP::Encoding encoding;
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 2;

      // Obtain the CDR encapsulation Codec.
      IOP::Codec_var codec =
        codec_factory->create_codec (encoding);

      CORBA::Long uid = 64321;

      CORBA::Any uid_as_any;

      uid_as_any <<= uid;
      CORBA::OctetSeq client_uid = *codec->encode (uid_as_any);
      messenger->send_message( client_uid );
    }

  catch(const CORBA::Exception& ex)
    {
      std::cerr << "Exception in MessengerClient: " << ex << std::endl;
      return 1;
    }

  std::cout << "Message was sent" << std::endl;
  return 0;
}
