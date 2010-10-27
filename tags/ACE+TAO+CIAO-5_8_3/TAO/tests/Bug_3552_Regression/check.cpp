// $Id$

#include "tao/CodecFactory/CodecFactory.h"
#include "structC.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = 1; // Failed until changed.

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain the reference to the CodecFactory.
      CORBA::Object_var
        obj = orb->resolve_initial_references ("CodecFactory");
      IOP::CodecFactory_var
        codec_factory = IOP::CodecFactory::_narrow (obj.in ());

      // Obtain the codec from the factory.
      // Set up a structure that contains information necessary to
      // create a GIOP 1.2 CDR encapsulation Codec.
      IOP::Encoding encoding;
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 2;
      IOP::Codec_var
        codec = codec_factory->create_codec (encoding);

      // Create the CDR we want to check
      CORBA::Octet mixedEndianCDR[]= {
        // Starts off BigEndian
        0x00,                                           // BigEndian mark
              0x1a, 0x87, 0x00,                         // 3 bytes padding
        0x00, 0x00, 0x00, 0x0f,                         // Typecode = tk_struct
        0x00, 0x00, 0x00, 0x4c,                         // Length of embedded encapsulation
        // Has an embedded LittleEndian bit
        0x01,                                           // LittleEndian mark
              0x1a, 0x87, 0x00,                         // 3 bytes padding
        0x17, 0x00, 0x00, 0x00,                         // Length of ID string
        0x49, 0x44, 0x4c, 0x3a, 0x54, 0x65, 0x73, 0x74, // "IDL:Test"
        0x2f, 0x74, 0x68, 0x65, 0x53, 0x74, 0x72, 0x75, // "/theStru"
        0x63, 0x74, 0x3a, 0x31, 0x2e, 0x30, 0x00,       // "ct:1.0" + Null
                                                  0x00, // 1 byte Padding
        0x0a, 0x00, 0x00, 0x00,                         // Length of simple name
        0x74, 0x68, 0x65, 0x53, 0x74, 0x72, 0x75, 0x63, // "theStruc"
        0x74, 0x00,                                     // "t" + Null
                    0x00, 0x00,                         // 2 bytes padding
        0x01, 0x00, 0x00, 0x00,                         // Has One member
        0x0a, 0x00, 0x00, 0x00,                         // Length of member name
        0x74, 0x68, 0x65, 0x53, 0x74, 0x72, 0x69, 0x6e, // "theStrin"
        0x67, 0x00,                                     // "g" + Null
                    0x00, 0x00,                         // 2 bytes padding
        0x12, 0x00, 0x00, 0x00,                         // member typecode = tk_string
        0x00, 0x00, 0x00, 0x00,                         // Of Variable length
        // Should now return back to outer scope BigEndian
        0x00, 0x00, 0x00, 0x06,                         // (Length of value)
        0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x00              // "Hello" + Null
      };
      const unsigned int
        mixedEndianCDRLength= sizeof (mixedEndianCDR) / sizeof (mixedEndianCDR[0]);

      // We actually need this to be a sequence of octets (Don't delete them on destruction)
      CORBA::OctetSeq
        OctSeq (mixedEndianCDRLength, mixedEndianCDRLength, mixedEndianCDR, 0);

      // Now try and decode this message
      CORBA::Any_var
        decodedData = codec->decode (OctSeq);
      // And extract the actual struct (still owned by the any)
      Test::theStruct *myStruct;
      if (decodedData.in () >>= myStruct)
        {
          if (!strcmp (myStruct->theString, "Hello"))
            {
              result = 0; // Success!
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                "ERROR: Unexpected string=\"%C\" (should be \"Hello\")\n",
                myStruct->theString.in ()));
            }
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "ERROR: Didn't decode the Struct\n"));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught:");
    }

  if (result)
    ACE_DEBUG ((LM_DEBUG, "Mixed Endian CDR decoding is NOT working\nTest Failed!\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "Mixed Endian CDR decoding is working\nTest Passed!\n"));
  return result;
}
