// $Id$

#include "TestC.h"
#include "tao/CodecFactory/CodecFactory.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
       CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
       CORBA::Object_var object =
               orb->resolve_initial_references("CodecFactory");

       IOP::CodecFactory_var codec_factory =
               IOP::CodecFactory::_narrow(object.in ());
       IOP::Encoding e = {IOP::ENCODING_CDR_ENCAPS,1,2};

       IOP::Codec_var codec = codec_factory->create_codec(e);

       CORBA::Any any_o;
       Value* value = new OBV_Value;
       any_o <<= &value;

       CORBA::OctetSeq_var bin = codec->encode(any_o);
       CORBA::Any_var any_n = codec->decode(bin.in ());
    }
  catch (const CORBA::Exception& ex)
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
