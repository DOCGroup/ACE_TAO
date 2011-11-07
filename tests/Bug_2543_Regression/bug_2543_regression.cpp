// $Id$

#include "TestC.h"
#include "tao/CodecFactory/CodecFactory.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
       CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

       orb->register_value_factory(
               Base::_tao_obv_static_repository_id(),
               new Base_init);
       orb->register_value_factory(
               Value::_tao_obv_static_repository_id(),
               new Value_init);

       CORBA::Object_var object =
               orb->resolve_initial_references("CodecFactory");

       IOP::CodecFactory_var codec_factory =
               IOP::CodecFactory::_narrow(object.in ());
       IOP::Encoding e = {IOP::ENCODING_CDR_ENCAPS,1,2};

       IOP::Codec_var codec = codec_factory->create_codec(e);

       CORBA::Any any_o;
       BaseSeq values;
       values.length(1);
       values[0] = new OBV_Value(1,"An instance of Value");
       any_o <<= values;
       any_o.type(_tc_BaseSeq);

       CORBA::OctetSeq_var bin = codec->encode(any_o);
       CORBA::Any_var any_n = codec->decode(bin.in ());

       const BaseSeq * out = 0;
       if (!(any_n.in () >>= out))
         {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Error: extract out of any failed\n"),
                               1);
         }
       if (ACE_OS::strcmp( (*out)[0]->_tao_obv_repository_id(),
                          Value::_tao_obv_static_repository_id()) != 0)
         {
            ACE_ERROR_RETURN ((LM_ERROR, "Error: wrong type!\n"), 1);
         }

         orb->destroy();
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
