#include "TestC.h"
#include "tao/CodecFactory/CodecFactory.h"
#include <cstring>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::ValueFactory_var const base_factory (new Base_init);
      CORBA::ValueFactory_var const previous_base_factory = orb->register_value_factory (
        Base::_tao_obv_static_repository_id (),
        base_factory.in ());
      CORBA::ValueFactory_var const value_factory (new Value_init);
      CORBA::ValueFactory_var const previous_value_factory = orb->register_value_factory (
        Value::_tao_obv_static_repository_id (),
        value_factory.in ());

      CORBA::Object_var const object =
        orb->resolve_initial_references ("CodecFactory");

      IOP::CodecFactory_var const codec_factory =
        IOP::CodecFactory::_narrow (object.in ());
      IOP::Encoding const encoding {IOP::ENCODING_CDR_ENCAPS, 1, 2};

      IOP::Codec_var const codec = codec_factory->create_codec (encoding);

      CORBA::Any any_o;
      BaseSeq values;
      values.length (1);
      values[0] = new OBV_Value (1, "An instance of Value");
      any_o <<= values;
      any_o.type (_tc_BaseSeq);

      CORBA::OctetSeq_var const bin = codec->encode (any_o);
      CORBA::Any_var const any_n = codec->decode (bin.in ());

      // This pointer refers into any_n and remains valid while the Any is alive.
      BaseSeq const *out {};
      if (!(any_n.in () >>= out))
        ACE_ERROR_RETURN ((LM_ERROR, "Error: extract out of any failed\n"), 1);

      if (std::strcmp ((*out)[0]->_tao_obv_repository_id (),
                       Value::_tao_obv_static_repository_id ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Error: wrong type!\n"), 1);

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
