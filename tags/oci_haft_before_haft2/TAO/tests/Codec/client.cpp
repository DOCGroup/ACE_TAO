// -*- C++ -*-

#include "tao/IOPC.h"
#include "testC.h"

ACE_RCSID (Codec,
	   client,
	   "$Id$")

int
verify_data (Foo::Bar *original, Foo::Bar *extracted)
{
  if (!original || !extracted)
    return -1;

  ACE_DEBUG ((LM_DEBUG,
	      "Original\n"
	      "--------\n"
	      "%d\n"
	      "%d\n"
//		"%Lu\n"
	      "%s\n\n"
	      "Extracted\n"
	      "---------\n"
	      "%d\n"
	      "%d\n"
//		"%Lu\n"
	      "%s\n\n",
	      original->A,
	      original->B,
//		original->C,
	      original->D.in (),
	      extracted->A,
	      extracted->B,
//		extracted->C,
	      extracted->D.in ()));

  if (original->A != extracted->A
      || original->B != extracted->B
      || original->C != extracted->C
      || ACE_OS_String::strcmp (original->D, extracted->D) != 0)
    return -1;

  return 0;
}

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
      ACE_TRY_CHECK;

      // Obtain a reference to the CodecFactory.
      CORBA::Object_var obj =
	orb->resolve_initial_references ("CodecFactory"
					 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      IOP::CodecFactory_var codec_factory =
	IOP::CodecFactory::_narrow (obj.in ()
                                    ACE_ENV_ARG_PARAMETER);
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
	codec_factory->create_codec (encoding
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ----------------------------------------------------------

      // Test values to be placed in the test structure.
      const CORBA::Long A = 1010;
      const CORBA::Long B = -3427;
      const CORBA::ULongLong C = ACE_UINT64_LITERAL (2001);
      const char D[] = "I'm Batman.";

      // Create the structure to be encoded.
      Foo::Bar value;
      value.A = A;
      value.B = B;
      value.C = C;
      value.D = CORBA::string_dup (D);

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
      encoded_data = codec->encode (data
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if ((ACE_reinterpret_cast (ptrdiff_t, encoded_data->get_buffer ())
	     % ACE_CDR::MAX_ALIGNMENT) == 0)
	    ACE_DEBUG ((LM_WARNING,
                        "\n"
                        "WARNING: Data to be decoded is already aligned "
                        "on MAX_ALIGNMENT.\n\n"));

      // Extract the data from the octet sequence.
      decoded_data = codec->decode (encoded_data.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!(decoded_data.in() >>= extracted_value))
	ACE_ERROR_RETURN ((LM_ERROR,
			   "ERROR: Unable to extract decoded data "
                           "from Any\n"),
			  -1);

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
      encoded_data = codec->encode_value (data
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if ((ACE_reinterpret_cast (ptrdiff_t, encoded_data->get_buffer ())
	     % ACE_CDR::MAX_ALIGNMENT) == 0)
	    ACE_DEBUG ((LM_WARNING,
                        "\n"
                        "WARNING: Data to be decoded is already aligned "
                        "on MAX_ALIGNMENT.\n\n"));

      // Extract the data from the octet sequence.
      decoded_data = codec->decode_value (encoded_data.in (),
					  Foo::_tc_Bar
					  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!(decoded_data.in() >>= extracted_value))
	ACE_ERROR_RETURN ((LM_ERROR,
			   "ERROR: Unable to extract decoded data "
                           "from Any\n"),
			  -1);

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
