// -*- C++ -*-

#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"
#include "ACEXML/common/XML_Codecs.h"
#include <memory>

ACEXML_Char*
ACEXML_Base64::encode (const ACEXML_Char* input,
                       size_t* output_len)
{
  if (input == nullptr)
    return nullptr;
  size_t len = ACE_OS::strlen (input);

  ACE_Byte* buf = nullptr;
  ACE_NEW_RETURN (buf,
                  ACE_Byte[len],
                  nullptr);
  std::unique_ptr<ACE_Byte[]> cleanup_buf (buf);

  for (size_t i = 0; i < len; ++i)
    buf[i] = (ACE_Byte)input[i];
  buf[len] = 0;

  size_t encode_len = 0;
  std::unique_ptr<ACE_Byte[]> encoded_buf (
    ACE_Base64::encode (buf, len, &encode_len));

  if (encoded_buf == nullptr)
    return nullptr;

  ACEXML_Char* result = nullptr;
  ACE_NEW_RETURN (result,
                  ACEXML_Char[encode_len+1],
                  nullptr);

  for (size_t j = 0; j < encode_len; ++j)
    result[j] = (ACEXML_Char)encoded_buf[j];
  result[encode_len] = 0;

  *output_len = encode_len;
  return result;
}


ACEXML_Char*
ACEXML_Base64::decode (const ACEXML_Char* input,
                       size_t* output_len)
{
  if (input == nullptr)
    return nullptr;

  size_t len = ACE_OS::strlen (input);

  ACE_Byte* buf = nullptr;

  ACE_NEW_RETURN (buf,
                  ACE_Byte[len],
                  nullptr);

  std::unique_ptr<ACE_Byte[]> cleanup (buf);

  for (size_t i = 0; i < len; ++i)
    buf[i] = (ACE_Byte)input[i];

  buf[len] = 0;

  size_t decode_len = 0;

  std::unique_ptr<ACE_Byte[]> decoded_buf (
    ACE_Base64::decode (buf, &decode_len));

  if (decoded_buf == nullptr)
    return nullptr;

  ACEXML_Char* result = nullptr;

  ACE_NEW_RETURN (result,
                  ACEXML_Char[decode_len+1],
                  nullptr);

  for (size_t j = 0; j < decode_len; ++j)
    result[j] = (ACEXML_Char)decoded_buf[j];

  result[decode_len] = 0;

  *output_len = decode_len;

  return result;
}

