#include "Messenger_i.h"
#include "tao/CodecFactory/CodecFactory.h"

Messenger_i::Messenger_i (CORBA::ORB_ptr const orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Messenger_i::send_message (CORBA::OctetSeq const &user_name)
{
  try
    {
      // Obtain a reference to the CodecFactory.
      CORBA::Object_var const obj =
        orb_->resolve_initial_references ("CodecFactory");

      IOP::CodecFactory_var const codec_factory =
        IOP::CodecFactory::_narrow (obj.in ());

      // Set up a structure that contains information necessary to
      // create a GIOP 1.2 CDR encapsulation Codec.
      IOP::Encoding encoding;
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 2;

      // Obtain the CDR encapsulation Codec.
      IOP::Codec_var const codec =
        codec_factory->create_codec (encoding);

      CORBA::Any_var user_name_as_any {};
      user_name_as_any = codec->decode (user_name);

      char const *name {};
      user_name_as_any >>= name;
      ACE_DEBUG ((LM_DEBUG, "User's name: %C\n", name));
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception ("ERROR");
    }
  catch (CORBA::Exception const &e)
    {
      e._tao_print_exception ("ERROR");
    }
}

void
Messenger_i::shutdown ()
{
  this->orb_->shutdown (false);
}
