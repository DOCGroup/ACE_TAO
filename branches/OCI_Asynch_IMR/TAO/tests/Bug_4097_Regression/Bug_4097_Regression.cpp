// $Id$

#include "Bug_4097_RegressionS.h"

#include "tao/corba.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/Object_T.h"
#include "tao/ORB_Core.h"
#include "tao/CodecFactory/CodecFactory.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "tao/AnyTypeCode/Any.h"

namespace {
bool
checkResult (const CORBA::Any &any, const MultiLabelUnion &mlu)
{
  bool error = false;
  const MultiLabelUnion *extracted = 0;
  if (any >>= extracted)
    {
      if (extracted->_d () == mlu._d ())
        {
          ACE_DEBUG ((LM_DEBUG,  ". Extracted descriminant is correct (%d)\n", extracted->_d ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,  ". Extracted descriminant is WRONG (%d)\n", extracted->_d ()));
          error = true;
        }
      if (extracted->mlu_char () == mlu.mlu_char ())
        {
          ACE_DEBUG ((LM_DEBUG,  ". Extracted value is correct (%c)\n", extracted->mlu_char ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,  ". Extracted value is WRONG (%c)\n", extracted->mlu_char ()));
          error = true;
        }
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,  ". DID NOT Extract from any (Test Failure)\n"));
      error = true;
    }

  return error;
}
} // End of unnamed namespace

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG, "Starting\n"));
  CORBA::ORB_var orb (CORBA::ORB_init (argc, argv));

  CORBA::Object_var obj (orb->resolve_initial_references ("CodecFactory"));
  IOP::CodecFactory_var codecFactory (IOP::CodecFactory::_narrow (obj.in ()));

  IOP::Encoding cdr_encoding;
  cdr_encoding.format = IOP::ENCODING_CDR_ENCAPS;
  cdr_encoding.major_version = 1;
  cdr_encoding.minor_version = 2;
  IOP::Codec_var codec (codecFactory->create_codec (cdr_encoding));

  // Test starts here with first case

  ACE_DEBUG ((LM_DEBUG,  "Creating union using default descriminant of mlu_char type\n"));
  MultiLabelUnion mlu;
  mlu.mlu_char ('x');
  CORBA::Any any;
  any <<= mlu;
  bool error = checkResult (any, mlu);

  ACE_DEBUG ((LM_DEBUG,  "Encode->Decode any with union using default descriminant of mlu_char type\n"));
  CORBA::OctetSeq_var messageInCDR (codec->encode (any));
  ACE_DEBUG ((LM_DEBUG,  ". Size of encoding is %d\n", messageInCDR->length ()));
  CORBA::Any_var decoded = codec->decode (messageInCDR);
  if (!decoded.ptr ())
    {
      ACE_DEBUG ((LM_DEBUG,  "No decoding\n"));
      error = true;
    }
  else if (checkResult (decoded.in (), mlu))
    {
      error = true;
    }

  ACE_DEBUG ((LM_DEBUG,  "Creating union using case 0\n"));
  mlu.mlu_char ('y');
  mlu._d (static_cast<CORBA::Long> (0)); // Note first case label for type.
  any <<= mlu;
  if (checkResult (any, mlu))
    {
      error = true;
    }

  ACE_DEBUG ((LM_DEBUG,  "Encode->Decode any with union using case 0\n"));
  messageInCDR = codec->encode (any);
  ACE_DEBUG ((LM_DEBUG,  ". Size of encoding is %d\n", messageInCDR->length ()));
  decoded = codec->decode (messageInCDR);
  if (!decoded.ptr ())
    {
      ACE_DEBUG ((LM_DEBUG,  "No decoding\n"));
      error = true;
    }
  else if (checkResult (decoded.in (), mlu))
    {
      error = true;
    }

  ACE_DEBUG ((LM_DEBUG,  "Creating union using case 1\n"));
  mlu.mlu_char ('z');
  mlu._d (static_cast<CORBA::Long> (1)); // Note second case label for same type as before
  any <<= mlu;
  if (checkResult (any, mlu))
    {
      error = true;
    }

  ACE_DEBUG ((LM_DEBUG,  "Encode->Decode any with union using case 1\n"));
  messageInCDR = codec->encode (any);
  ACE_DEBUG ((LM_DEBUG,  ". Size of encoding is %d\n", messageInCDR->length ()));
  decoded = codec->decode (messageInCDR);
  if (!decoded.ptr ())
    {
      ACE_DEBUG ((LM_DEBUG,  "No decoding\n"));
      error = true;
    }
  else if (checkResult (decoded.in (), mlu))
    {
      error = true;
    }

  if (error)
    {
      ACE_DEBUG ((LM_DEBUG, "Test FAILED.\n"));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG, "Test SUCCEEDED.\n"));
  return 0;
}
