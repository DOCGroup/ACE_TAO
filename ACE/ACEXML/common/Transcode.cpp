#include "ACEXML/common/Transcode.h"
#include "ace/Truncate.h"

#include <cstring>

int
ACEXML_Transcoder::utf162utf8 (ACEXML_UTF16 src,
                               ACEXML_UTF8 *dst,
                               size_t len)
{
  // Check for valid argument first...

  if (dst == nullptr)
  {
    return ACEXML_INVALID_ARGS;
  }

  if (src < 0x80)
    {
      if (len < 1)
      {
        return ACEXML_DESTINATION_TOO_SHORT;
      }

      *dst = static_cast<ACEXML_UTF8> (src);
      return 1;
    }
  else if (src < 0x800)
    {
      if (len < 2)
      {
        return ACEXML_DESTINATION_TOO_SHORT;
      }

      *dst = static_cast<ACEXML_UTF8> (0xc0 | (src / 0x40));
      *(dst+1) = static_cast<ACEXML_UTF8> (0x80 | (src % 0x40));
      return 2;
    }
  else
    {
      if (len < 3)
      {
        return ACEXML_DESTINATION_TOO_SHORT;
      }

      // Surrogates (0xD800 - 0xDFFF) are not valid unicode values
      if (src >= 0xD800 && src < 0xE000)
      {
        return ACEXML_IS_SURROGATE;
      }

      *dst = static_cast<ACEXML_UTF8> (0xe0 | (src / 0x1000));
      *(dst+1) = static_cast<ACEXML_UTF8> (0x80 | (src % 0x1000 / 0x40));
      *(dst+2) = static_cast<ACEXML_UTF8> (0x80 | (src % 0x40));
      return 3;
    }
}

int
ACEXML_Transcoder::ucs42utf8 (ACEXML_UCS4 src,
                              ACEXML_UTF8 *dst,
                              size_t len)
{
  if (src < 0x10000)
    {
      int const retv = ACEXML_Transcoder::utf162utf8
                 (static_cast<ACEXML_UTF16> (src),
                  dst, len);
      return (retv == ACEXML_IS_SURROGATE ? ACEXML_NON_UNICODE : retv);
    }
  else if (src >= 0x10000 && src < 0x110000)
    {
      if (len < 4)
      {
        return ACEXML_DESTINATION_TOO_SHORT;
      }

      if (dst == nullptr)
      {
        return ACEXML_INVALID_ARGS;
      }

      *dst = static_cast<ACEXML_UTF8> (0xf0 | (src / 0x40000));
      *(dst+1) = static_cast<ACEXML_UTF8> (0x80 | (src % 0x40000 / 0x1000));
      *(dst+2) = static_cast<ACEXML_UTF8> (0x80 | (src % 0x1000 / 0x40));
      *(dst+3) = static_cast<ACEXML_UTF8> (0x80 | (src % 0x40));
      return 4;
    }
  return ACEXML_NON_UNICODE;
}

int
ACEXML_Transcoder::ucs42utf16 (ACEXML_UCS4 src,
                               ACEXML_UTF16 *dst,
                               size_t len)
{
  if (dst == nullptr)
  {
    return ACEXML_INVALID_ARGS;
  }

  if (src < 0x10000)
    {
      if (len < 1)
      {
        return ACEXML_DESTINATION_TOO_SHORT;
      }

      if (src >= 0xD800 && src < 0xE000)
      {
        return ACEXML_NON_UNICODE;     // Surrogates are not valid unicode value
      }

      *dst = static_cast<ACEXML_UTF16> (src);
      return 1;
    }
  else if (src >= 0x10000 && src < 0x110000)
    // Scalar values are encoded into surrogates
    {
      if (len < 2)
      {
        return ACEXML_DESTINATION_TOO_SHORT;
      }

      ACEXML_UCS4 const value = src - 0x10000;
      *dst = static_cast<ACEXML_UTF16> (0xD800 | (value / 0x400));
      *(dst+1) = static_cast<ACEXML_UTF16> (0xDC00 | (value % 0x400));
      return 2;
    }

  return ACEXML_NON_UNICODE;
}

int
ACEXML_Transcoder::surrogate2utf8 (ACEXML_UTF16 high,
                                   ACEXML_UTF16 low,
                                   ACEXML_UTF8 *dst,
                                   size_t len)
{
  if (dst == nullptr ||
      high < 0xD800 || high >= 0xDC00 ||
      low < 0xDC00 || low >= 0xE000)
  {
    return ACEXML_INVALID_ARGS;
  }

  ACEXML_UCS4 const src = (high - 0xD800) * 0x400 + (low - 0xDC00) + 0x10000;
  return ACEXML_Transcoder::ucs42utf8 (src, dst, len);
}

int
ACEXML_Transcoder::surrogate2ucs4 (ACEXML_UTF16 high,
                                   ACEXML_UTF16 low,
                                   ACEXML_UCS4 &dst)
{
  if (high < 0xD800 || high >= 0xDC00 ||
      low < 0xDC00 || low >= 0xE000)
  {
    return ACEXML_INVALID_ARGS;
  }

  dst = (high - 0xD800) * 0x400 + (low - 0xDC00) + 0x10000;
  return ACEXML_SUCCESS;
}

int
ACEXML_Transcoder::utf82ucs4 (ACEXML_UTF8 const *the_src,
                              size_t len,
                              ACEXML_UCS4 &dst)
{
  if (the_src == nullptr)
    {
      return ACEXML_INVALID_ARGS;
    }

  if (len == 0)
    {
      return ACEXML_END_OF_SOURCE;
    }

  unsigned char const *src = reinterpret_cast<unsigned char const *> (the_src);
  size_t count {};
  ACEXML_UCS4 value {};
  ACEXML_UCS4 minimum {};

  if (src[0] < 0x80)
    {
      count = 1;
      value = src[0];
    }
  else if ((src[0] & 0xe0) == 0xc0)
    {
      count = 2;
      value = src[0] & 0x1f;
      minimum = 0x80;
    }
  else if ((src[0] & 0xf0) == 0xe0)
    {
      count = 3;
      value = src[0] & 0x0f;
      minimum = 0x800;
    }
  else if ((src[0] & 0xf8) == 0xf0)
    {
      count = 4;
      value = src[0] & 0x07;
      minimum = 0x10000;
    }
  else
    {
      return ACEXML_NON_UNICODE;
    }

  for (size_t i = 1; i < count; ++i)
    {
      if (i >= len)
        {
          return ACEXML_END_OF_SOURCE;
        }
      if ((src[i] & 0xc0) != 0x80)
        {
          return ACEXML_NON_UNICODE;
        }
      value = (value << 6) | (src[i] & 0x3f);
    }

  if (value < minimum || value >= 0x110000 ||
      (value >= 0xd800 && value < 0xe000))
    {
      return ACEXML_NON_UNICODE;
    }

  dst = value;
  return ACE_Utils::truncate_cast<int> (count);
}

int
ACEXML_Transcoder::utf162ucs4 (const ACEXML_UTF16 *src,
                               size_t len,
                               ACEXML_UCS4 &dst)
{
  if (src == nullptr)
    {
      return ACEXML_INVALID_ARGS;
    }

  if (len == 0)
    {
      return ACEXML_END_OF_SOURCE;
    }

  if (*src >= 0xD800 && *src < 0xDC00)
    {
      if (len < 2)
        {
          return ACEXML_END_OF_SOURCE;
        }

      int const result = ACEXML_Transcoder::surrogate2ucs4 (*src, *(src+1), dst);
      return result == ACEXML_SUCCESS ? 2 : result;
    }
  if (*src >= 0xDC00 && *src < 0xE000)
    {
      return ACEXML_NON_UNICODE;
    }

  dst = *src;
  return 1;
}

int
ACEXML_Transcoder::utf8s2utf16s (const ACEXML_UTF8 *src,
                                 ACEXML_UTF16 *dst,
                                 size_t len)
{
  if (src == nullptr || dst == nullptr)
    {
      return ACEXML_INVALID_ARGS;
    }

  size_t src_len = std::strlen (src) + 1;

  size_t total_len = 0;
  ACEXML_UCS4 temp;

  while (src_len > 0)
    {
      int forward = ACEXML_Transcoder::utf82ucs4 (src,
                                                  src_len,
                                                  temp);
      if (forward <= 0)
      {
        return forward;
      }

      src += forward;
      src_len -= forward;

      forward = ACEXML_Transcoder::ucs42utf16 (temp,
                                               dst,
                                               len);
      if (forward <= 0)
      {
        return forward;
      }

      total_len += forward;
      dst += forward;
      len -= forward;
    }

  return ACE_Utils::truncate_cast<int> (total_len);
}

int
ACEXML_Transcoder::utf16s2utf8s (const ACEXML_UTF16 *src,
                                 ACEXML_UTF8 *dst,
                                 size_t len)
{
  if (src == nullptr || dst == nullptr)
  {
    return ACEXML_INVALID_ARGS;
  }

  size_t src_len = 1;
  for (const ACEXML_UTF16 *p = src; *p++ != 0; ++src_len)
    {
      ;
    }

  size_t total_len = 0;
  ACEXML_UCS4 temp;

  while (src_len > 0)
    {
      int forward = ACEXML_Transcoder::utf162ucs4 (src,
                                                   src_len,
                                                   temp);
      if (forward <= 0)
      {
        return forward;
      }

      src += forward;
      src_len -= forward;

      forward = ACEXML_Transcoder::ucs42utf8 (temp,
                                              dst,
                                              len);
      if (forward <= 0)
      {
        return forward;
      }

      total_len += forward;
      dst += forward;
      len -= forward;
    }

  return ACE_Utils::truncate_cast<int> (total_len);
}
