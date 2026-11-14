#include "TestC.h"
#include "tao/CodecFactory/CodecFactory.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const object =
        orb->resolve_initial_references ("CodecFactory");

      IOP::CodecFactory_var const codec_factory =
        IOP::CodecFactory::_narrow (object.in ());
      IOP::Encoding encoding {IOP::ENCODING_CDR_ENCAPS, 1, 2};

      IOP::Codec_var const codec = codec_factory->create_codec (encoding);

      CORBA::Any any_o;
      Value_var value (new OBV_Value);
      Value *value_to_insert = value._retn ();
      // The non-copying Any insertion takes ownership of value_to_insert.
      any_o <<= &value_to_insert;

      CORBA::OctetSeq_var const bin = codec->encode (any_o);
      CORBA::Any_var const any_n = codec->decode (bin.in ());

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught a CORBA exception\n");
      return 1;
    }
  catch (...)
    {
      return 1;
    }

  return 0;
}
