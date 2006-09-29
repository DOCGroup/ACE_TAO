// $Id$

#include "TestC.h"
#include "tao/CodecFactory/CodecFactory.h"

ACE_RCSID (Bug_2542_Regression,
           bug_2542_regression,
           "$Id$")

int main (int argc, char* argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
       CORBA::ORB_var orb = CORBA::ORB_init(argc,argv);
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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught a CORBA exception \n");
      return 1;
    }
  ACE_CATCHALL
    {
      return 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);

  return 0;
}
