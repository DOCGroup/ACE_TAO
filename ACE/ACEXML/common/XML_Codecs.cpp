// -*- C++ -*-  $Id$

#include "ace/Auto_Ptr.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"
#include "ACEXML/common/XML_Codecs.h"



ACEXML_Char*
ACEXML_Base64::encode (const ACEXML_Char* input,
                       size_t* output_len)
{
  if (!input)
    return 0;
  size_t len = ACE_OS::strlen (input);

  ACE_Byte* buf = 0;
  ACE_NEW_RETURN (buf,
                  ACE_Byte[len],
                  0);
  ACE_Auto_Basic_Array_Ptr<ACE_Byte> cleanup_buf (buf);

  for (size_t i = 0; i < len; ++i)
    buf[i] = (ACE_Byte)input[i];
  buf[len] = 0;

  size_t encode_len = 0;
  ACE_Byte* encodedBuf = ACE_Base64::encode (buf, len, &encode_len);

  if (!encodedBuf)
    return 0;

  ACEXML_Char* result = 0;
  ACE_NEW_RETURN (result,
                  ACEXML_Char[encode_len+1],
                  0);

  for (size_t j = 0; j < encode_len; ++j)
    result[j] = (ACEXML_Char)encodedBuf[j];
  result[encode_len] = 0;

  *output_len = encode_len;
  delete[] encodedBuf;
  return result;
}


ACEXML_Char*
ACEXML_Base64::decode (const ACEXML_Char* input,
                       size_t* output_len)
{
  if (!input)
    return 0;

  size_t len = ACE_OS::strlen (input);

  ACE_Byte* buf = 0;

  ACE_NEW_RETURN (buf,
                  ACE_Byte[len],
                  0);

  ACE_Auto_Basic_Array_Ptr<ACE_Byte> cleanup (buf);

  for (size_t i = 0; i < len; ++i)
    buf[i] = (ACE_Byte)input[i];

  buf[len] = 0;

  size_t decode_len = 0;

  ACE_Byte* decodedBuf = ACE_Base64::decode (buf, &decode_len);

  if (!decodedBuf)
    return 0;

  ACEXML_Char* result = 0;

  ACE_NEW_RETURN (result,
                  ACEXML_Char[decode_len+1],
                  0);

  for (size_t j = 0; j < decode_len; ++j)
    result[j] = (ACEXML_Char)decodedBuf[j];

  result[decode_len] = 0;

  *output_len = decode_len;
  delete[] decodedBuf;

  return result;
}

