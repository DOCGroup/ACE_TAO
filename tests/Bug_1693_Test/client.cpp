// -*- C++ -*-
// $Id$

#include "tao/CodecFactory/CodecFactory.h"
#include "testC.h"
#include "ace/Log_Msg.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "my_orb");

      // Obtain a reference to the CodecFactory.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("CodecFactory");

      IOP::CodecFactory_var codec_factory =
        IOP::CodecFactory::_narrow (obj.in ());

      // ----------------------------------------------------------

      // Set up a structure that contains information necessary to
      // create a GIOP 1.1 CDR encapsulation Codec.
      IOP::Encoding encoding;
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 1;

      // Obtain the CDR encapsulation Codec.
      IOP::Codec_var codec =
        codec_factory->create_codec (encoding);

      // ----------------------------------------------------------
      {
        CORBA::OctetSeq_var encoded_data;
        CORBA::Any_var decoded_data;

        Foo::type_ulong l = 9192631;

        CORBA::Any tmp;

        tmp <<= l;

        encoded_data =
          codec->encode_value (tmp);

        decoded_data =
          codec->decode_value (encoded_data.in (),
                               Foo::_tc_type_ulong);

        Foo::type_ulong check = 0;

        if (!(decoded_data >>= check))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Unable to extract typedefed decoded "
                             "data from Any\n"),
                            -1);

        if (check != l)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Extracted value not equal "
                             "to the encoded value\n"),
                            -1);

      }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Test passed\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Bug_1693_Test test:");
      return -1;
    }

  return 0;
}
