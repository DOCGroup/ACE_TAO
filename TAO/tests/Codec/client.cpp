// -*- C++ -*-
#include "tao/CodecFactory/CodecFactory.h"
#include "tao/Codeset/Codeset.h"
#include "testC.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"

#include <cstring>
#include <memory>

using WString = std::unique_ptr<CORBA::WChar, decltype (&CORBA::wstring_free)>;

WString
gen_wstring (CORBA::ULong const max_length)
{
  CORBA::ULong const len = max_length;
  WString buf (CORBA::wstring_alloc (len), &CORBA::wstring_free);
  CORBA::ULong i {};
  CORBA::WChar const limit =
    ACE_OutputCDR::wchar_maxbytes() == 1 ? ACE_OCTET_MAX : ACE_WCHAR_MAX;
  while (i < len)
    {
      CORBA::WChar const wc = ACE_OS::rand () % limit;
      if (wc)
        {
          buf.get ()[i] = wc;
          i++;
        }
    }

  buf.get ()[i] = 0;
  return buf;
}

int
verify_data (Foo::Bar const *const original, Foo::Bar const *const extracted)
{
  if (!original || !extracted)
    return -1;

  ACE_DEBUG ((LM_DEBUG,
              "Original\n"
              "--------\n"
              "%d\n"
              "%d\n"
//              "%Lu\n"
              "%C\n\n"
              "Extracted\n"
              "---------\n"
              "%d\n"
              "%d\n"
//              "%Lu\n"
              "%C\n\n",
              original->A,
              original->B,
//              original->C,
              original->D.in (),
              extracted->A,
              extracted->B,
//              extracted->C,
              extracted->D.in ()));

  if (original->A != extracted->A
      || original->B != extracted->B
      || original->C != extracted->C
      || (std::strcmp (original->D, extracted->D) != 0)
      || (ACE_OS::strcmp (original->E, extracted->E) != 0))
    return -1;

  return 0;
}

int
test_codec (IOP::Codec_ptr const codec)
{
  // ----------------------------------------------------------

  // Test values to be placed in the test structure.
  constexpr CORBA::Long A = 1010;
  constexpr CORBA::Long B = -3427;
  constexpr CORBA::ULongLong C = ACE_UINT64_LITERAL (2001);
  CORBA::Char const D[] = "I'm Batman.";
  WString const E = gen_wstring (25);

  // Create the structure to be encoded.
  Foo::Bar value {};
  value.A = A;
  value.B = B;
  value.C = C;
  value.D = CORBA::string_dup (D);
  value.E = CORBA::wstring_dup (E.get ());

  CORBA::Any data {};
  data <<= value;

  // ----------------------------------------------------------

  CORBA::OctetSeq_var encoded_data {};
  CORBA::Any_var decoded_data {};
  Foo::Bar const *extracted_value {};

  // Encode the structure into an octet sequence using the CDR
  // enscapsulation Codec.

  ACE_DEBUG ((LM_DEBUG,
              "Testing CDR encapsulation Codec encode()/decode()\n"
              "=================================================\n"));

  // Start out with the encode() method, i.e. the one that
  // includes the TypeCode in the CDR encapsulation.
  encoded_data = codec->encode (data);

  if ((reinterpret_cast<ptrdiff_t> (encoded_data->get_buffer ())
          % ACE_CDR::MAX_ALIGNMENT) == 0)
    ACE_DEBUG ((LM_DEBUG,
                "\nData for decoding are already aligned "
                "on MAX_ALIGNMENT.\n\n"));
  // Extract the data from the octet sequence.
  decoded_data = codec->decode (encoded_data.in ());

  if (!(decoded_data.in () >>= extracted_value))
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
  encoded_data = codec->encode_value (data);

  if ((reinterpret_cast<ptrdiff_t> (encoded_data->get_buffer ())
          % ACE_CDR::MAX_ALIGNMENT) == 0)
    ACE_DEBUG ((LM_WARNING,
                "\n"
                "WARNING: Data to be decoded is already aligned "
                "on MAX_ALIGNMENT.\n\n"));

  // Extract the data from the octet sequence.
  decoded_data = codec->decode_value (encoded_data.in (),
                                      Foo::_tc_Bar);

  if (!(decoded_data.in () >>= extracted_value))
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

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int retval {};
  try
    {
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv, "my_orb")};

      // Obtain a reference to the CodecFactory.
      CORBA::Object_var const obj {orb->resolve_initial_references ("CodecFactory")};

      IOP::CodecFactory_var const codec_factory {IOP::CodecFactory::_narrow (obj.in ())};

      // Set up a structure that contains information necessary to
      // create a GIOP 1.1 CDR encapsulation Codec.
      IOP::Encoding_1_2 encoding_1_2 {};
      encoding_1_2.format = IOP::ENCODING_CDR_ENCAPS;
      encoding_1_2.major_version = 1;
      encoding_1_2.minor_version = 2;
      encoding_1_2.char_codeset = 0x00010001U;
      encoding_1_2.wchar_codeset = 0x00010109U;

      // Obtain the CDR encapsulation Codec.
      IOP::Codec_var const codec_1_2 {
        codec_factory->create_codec_with_codesets (encoding_1_2)};

      retval = test_codec (codec_1_2.in ());

      // ----------------------------------------------------------

      // Set up a structure that contains information necessary to
      // create a GIOP 1.1 CDR encapsulation Codec.
      IOP::Encoding encoding {};
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 1;

      // Obtain the CDR encapsulation Codec.
      IOP::Codec_var const codec {codec_factory->create_codec (encoding)};

      retval = test_codec (codec.in ());
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Codec test:");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, "Codec test passed.\n"));

  return retval;
}
