// -*- C++ -*-

#include "tao/CodecFactory/CodecFactory.h"
#include "testC.h"
#include "ace/Log_Msg.h"

ACE_RCSID (Codec,
           client,
           "$Id$")

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "my_orb"
                         ACE_ENV_ARG_PARAMETER);

      // Obtain a reference to the CodecFactory.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("CodecFactory"
                                         ACE_ENV_ARG_PARAMETER);

      IOP::CodecFactory_var codec_factory =
        IOP::CodecFactory::_narrow (obj.in ()
                                    ACE_ENV_ARG_PARAMETER);

      // ----------------------------------------------------------

      // Set up a structure that contains information necessary to
      // create a GIOP 1.1 CDR encapsulation Codec.
      IOP::Encoding encoding;
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 1;

      // Obtain the CDR encapsulation Codec.
      IOP::Codec_var codec =
        codec_factory->create_codec (encoding
                                     ACE_ENV_ARG_PARAMETER);

      // ----------------------------------------------------------
      {
        CORBA::OctetSeq_var encoded_data;
        CORBA::Any_var decoded_data;

        Foo::type_ulong l = 9192631;

        CORBA::Any tmp;

        tmp <<= l;

        encoded_data =
          codec->encode_value (tmp
                               ACE_ENV_ARG_PARAMETER);

        decoded_data =
          codec->decode_value (encoded_data.in (),
                               Foo::_tc_type_ulong
                               ACE_ENV_ARG_PARAMETER);

        Foo::type_ulong check = 0;

        if (!(decoded_data >>= check))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Unable to extract typedefed decoded "
                             "data from Any\n"),
                            -1);

        if (check != l)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Extracted value not equal "
                             "to the encoded value \n"),
                            -1);

      }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Test passed \n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Bug_1693_Test test:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
