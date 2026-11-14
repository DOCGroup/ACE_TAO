// -*- C++ -*-

#include "ace/OS_Errno.h"
#include "ace/OS_NS_string.h"
#include "ACEXML/common/XML_Codecs.h"
#include <memory>
#include <new>

ACEXML_Char*
ACEXML_Base64::encode (const ACEXML_Char* input,
                       size_t* output_len)
{
  if (input == nullptr)
    return nullptr;
  size_t len = ACE_OS::strlen (input);

  std::unique_ptr<ACE_Byte[]> buf {new (std::nothrow) ACE_Byte[len + 1]};
  if (!buf)
    {
      errno = ENOMEM;
      return nullptr;
    }

  for (size_t i = 0; i < len; ++i)
    buf[i] = static_cast<ACE_Byte> (input[i]);
  buf[len] = 0;

  size_t encode_len = 0;
  std::unique_ptr<ACE_Byte[]> encoded_buf (
    ACE_Base64::encode (buf.get (), len, &encode_len));

  if (encoded_buf == nullptr)
    return nullptr;

  ACEXML_Char *result {};
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

  std::unique_ptr<ACE_Byte[]> buf {new (std::nothrow) ACE_Byte[len + 1]};
  if (!buf)
    {
      errno = ENOMEM;
      return nullptr;
    }

  for (size_t i = 0; i < len; ++i)
    buf[i] = static_cast<ACE_Byte> (input[i]);

  buf[len] = 0;

  size_t decode_len = 0;

  std::unique_ptr<ACE_Byte[]> decoded_buf (
    ACE_Base64::decode (buf.get (), &decode_len));

  if (decoded_buf == nullptr)
    return nullptr;

  ACEXML_Char *result {};

  ACE_NEW_RETURN (result,
                  ACEXML_Char[decode_len+1],
                  nullptr);

  for (size_t j = 0; j < decode_len; ++j)
    result[j] = (ACEXML_Char)decoded_buf[j];

  result[decode_len] = 0;

  *output_len = decode_len;

  return result;
}

