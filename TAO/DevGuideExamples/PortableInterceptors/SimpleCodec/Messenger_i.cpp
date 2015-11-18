#include "Messenger_i.h"
#include "ace/OS_NS_string.h"
#include "tao/CodecFactory/CodecFactory.h"
#include <iostream>

// Implementation skeleton constructor
Messenger_i::Messenger_i (void)
{
}

// Implementation skeleton destructor
Messenger_i::~Messenger_i (void)
{
}

void Messenger_i::send_message (const CORBA::OctetSeq & user_name)
{
  try
    {
      int argc = 0;
      char **argv = 0;

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain a reference to the CodecFactory.
      CORBA::Object_var obj =
      orb->resolve_initial_references ("CodecFactory");

      IOP::CodecFactory_var codec_factory;

      if(CORBA::is_nil(obj.in()))
        {
          std::cerr <<  "Error: codec_factory" << std::endl;
          ACE_OS::exit(1);
        }
      else
        {
          codec_factory = IOP::CodecFactory::_narrow (obj.in ());
          std::cout << "Server got codec factory" << std::endl;
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

      CORBA::Any uid_as_any;
      uid_as_any = *(codec->decode(user_name));

      CORBA::Long uid;
      if (uid_as_any >>= uid) {
        std::cout << "UID: " << uid << std::endl;
      } else {
        std::cerr << "Could not extract UID from any." << std::endl;
      }

    }
  catch(...)
    {
      std::cerr << "exception received" << std::endl;
      ACE_OS::exit(1);
    }
}
