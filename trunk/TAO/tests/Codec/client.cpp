// -*- C++ -*-

#include "tao/IOPC.h"
#include "testC.h"

ACE_RCSID (Codec,
           client,
           "$Id$")

int
verify_data (Foo::Bar *original, Foo::Bar *extracted)
{
  ACE_DEBUG ((LM_DEBUG,
              "Original\t\t\tExtracted\n"
              "--------\t\t\t---------\n"
              "%s\t%s\n"
              "%d\t\t\t\t%d\n"
              "%s\t\t\t%s\n\n",
              original->A.in (), extracted->A.in (),
              original->B, extracted->B,
              original->C.in (), extracted->C.in ()));

  if (ACE_OS_String::strcmp (original->A, extracted->A) != 0
      || original->B != extracted->B
      || ACE_OS_String::strcmp (original->C, extracted->C) != 0)
    -1;

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "my_orb", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Obtain a reference to the CodecFactory.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("CodecFactory",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      IOP::CodecFactory_var codec_factory =
        IOP::CodecFactory::_narrow (obj.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ----------------------------------------------------------

      // Set up a structure that contains information necessary to
      // create a GIOP 1.1 CDR encapsulation Codec.
      IOP::Encoding encoding;
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 1;

      // Obtain the CDR encapsulation Codec.
      IOP::Codec_var codec =
        codec_factory->create_codec (encoding, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ----------------------------------------------------------

      // Test values to be placed in the test structure.
      const char A[] = "I need a standing ovation!";
      CORBA::Long B = -3427;
      const char C[] = "I'm Batman.";

      // Create the structure to be encoded.
      Foo::Bar value;
      value.A = CORBA::string_dup (A);
      value.B = B;
      value.C = CORBA::string_dup (C);

      CORBA::Any data;
      data <<= value;

      // ----------------------------------------------------------

      CORBA::OctetSeq_var encoded_data;
      CORBA::Any_var decoded_data;
      Foo::Bar *extracted_value;

      // Encode the structure into an octet sequence using the CDR
      // enscapsulation Codec.

      ACE_DEBUG ((LM_DEBUG,
                  "Testing CDR encapsulation Codec encode()/decode()\n"
                  "=================================================\n"));

      // Start out with the encode() method, i.e. the one that
      // includes the TypeCode in the CDR encapsulation.
      encoded_data = codec->encode (data, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Extract the data from the octet sequence.
      decoded_data = codec->decode (encoded_data, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      decoded_data.in() >>= extracted_value;

      // Verify that the extracted data matches the data that was
      // originally encoded into the octet sequence.
      if (::verify_data (&value, extracted_value) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Data extracted using "
                           "IOP::Codec::decode() does not match "
                           "original data.\n"),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  "Testing CDR encapsulation Codec "
                  "encode_value()/decode_value()\n"
                  "================================"
                  "=============================\n"));

      // Now use the encode_value() method, i.e. the one that does
      // *not* include the TypeCode in the CDR encapsulation.
      encoded_data = codec->encode_value (data, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Extract the data from the octet sequence.
      decoded_data = codec->decode_value (encoded_data,
                                          Foo::_tc_Bar,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      decoded_data.in() >>= extracted_value;

      // Verify that the extracted data matches the data that was
      // originally encoded into the octet sequence.
      if (::verify_data (&value, extracted_value) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Data extracted using "
                           "IOP::Codec::decode_value() does not match "
                           "original data.\n"),
                          -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Codec test:");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "Codec test passed.\n"));

  return 0;
}
