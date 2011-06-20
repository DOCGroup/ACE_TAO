// -*- C++ -*-  $Id$

#include "ACEXML/common/Transcode.h"
#include "ace/OS_NS_string.h"
#include "ace/Truncate.h"

int
ACEXML_Transcoder::utf162utf8 (ACEXML_UTF16 src,
                               ACEXML_UTF8 *dst,
                               size_t len)
{
  // Check for valid argument first...

  if (dst == 0)
    return ACEXML_INVALID_ARGS;

  if (src < 0x80)
    {
      if (len < 1)
        return ACEXML_DESTINATION_TOO_SHORT;

      *dst = static_cast<ACEXML_UTF8> (src);
      return 1;
    }
  else if (src < 0x800)
    {
      if (len < 2)
        return ACEXML_DESTINATION_TOO_SHORT;

      *dst = 0xc0 | (static_cast<ACEXML_UTF8> (src) / 0x40);
      *(dst+1) = 0x80 | (static_cast<ACEXML_UTF8> (src) % 0x40);
      return 2;
    }
  else
    {
      if (len < 3)
        return ACEXML_DESTINATION_TOO_SHORT;

      // Surrogates (0xD800 - 0xDFFF) are not valid unicode values
      if (src >= 0xD800 && src < 0xE000)
        return ACEXML_IS_SURROGATE;

      *dst = 0xe0 | (static_cast<ACEXML_UTF8> (src) / 0x1000);
      *(dst+1) = 0x80 | ((static_cast<ACEXML_UTF8> (src) % 0x1000) / 0x40);
      *(dst+2) = 0x80 | (static_cast<ACEXML_UTF8> (src) % 0x40);
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
      int retv = ACEXML_Transcoder::utf162utf8
                 (static_cast<ACEXML_UTF16> (src),
                  dst, len);
      return (retv == ACEXML_IS_SURROGATE ? ACEXML_NON_UNICODE : retv);
    }
  else if (src >= 0x100000 && src < 0x110000)
    {
      if (len < 4)
        return ACEXML_DESTINATION_TOO_SHORT;

      if (dst == 0)
        return ACEXML_INVALID_ARGS;

      *dst = 0xf0 | (static_cast<ACEXML_UTF8> (src / 0x40000));
      *(dst+1) = 0x80 | ((static_cast<ACEXML_UTF8> (src % 0x40000)) / 0x1000);
      *(dst+2) = 0x80 | ((static_cast<ACEXML_UTF8> (src % 0x1000)) / 0x40);
      *(dst+3) = 0x80 | (static_cast<ACEXML_UTF8> (src % 0x40));
      return 4;
    }
  return ACEXML_NON_UNICODE;
}


int
ACEXML_Transcoder::ucs42utf16 (ACEXML_UCS4 src,
                               ACEXML_UTF16 *dst,
                               size_t len)
{
  if (dst == 0)
    return ACEXML_INVALID_ARGS;

  if (src < 0x10000)
    {
      if (len < 1)
        return ACEXML_DESTINATION_TOO_SHORT;

      if (src >= 0xD800 && src < 0xE000)
        return ACEXML_NON_UNICODE;     // Surrogates are not valid unicode value

      *dst = static_cast<ACEXML_UTF16> (src);
      return 1;
    }
  else if (src >= 0x100000 && src < 0x110000)
    // Scalar values are encoded into surrogates
    {
      if (len < 2)
        return ACEXML_DESTINATION_TOO_SHORT;

      *dst = 0xD800 | (static_cast<ACEXML_UTF16> (src) / 0x400);
      *(dst+1) = 0xDC00 | (static_cast<ACEXML_UTF16> (src) % 0x400);
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
  if (len < 3)
    return ACEXML_DESTINATION_TOO_SHORT;

  if (dst == 0 ||
      (high >= 0xD800 && high < 0xDC00) ||
      (low >= 0xDC00 && low < 0xE000))
    return ACEXML_INVALID_ARGS;

  ACEXML_UCS4 src = (high - 0xD800) * 0x400 + (low - 0xDC00) + 0x10000;
  *dst = static_cast<ACEXML_UTF8> (0xD800 | (src / 0x400));
  *(dst+1) = static_cast<ACEXML_UTF8> (0xDC00 | (src % 0x400));
  return 2;
}

int
ACEXML_Transcoder::surrogate2ucs4 (ACEXML_UTF16 high,
                                   ACEXML_UTF16 low,
                                   ACEXML_UCS4 &dst)
{
  if ((high >= 0xD800 && high < 0xDC00) ||
      (low >= 0xDC00 && low < 0xE000))
    return ACEXML_INVALID_ARGS;

  dst = (high - 0xD800) * 0x400 + (low - 0xDC00) + 0x10000;
  return ACEXML_SUCCESS;
}

int
ACEXML_Transcoder::utf82ucs4 (const ACEXML_UTF8 *the_src,
                              size_t len,
                              ACEXML_UCS4 &dst)
{
  if (the_src == 0)
    {
      return ACEXML_INVALID_ARGS;
    }

  const unsigned char *src = reinterpret_cast<const unsigned char *> (the_src);

  size_t forward = 1;

  if (forward > len)
    {
      return ACEXML_END_OF_SOURCE;
    }

  if (static_cast<unsigned char> (*src) < 0x80)
    {
      dst = *src;
    }
  else if ((*src & 0xE0) == 0xC0)
    {
      dst = (*(src++) & 0x1f) * 0x40;
      if (++forward > len)
        return ACEXML_END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return ACEXML_NON_UNICODE;     // Error transcoding unicode scalar
      dst += *src & 0x3f;
    }
  else if ((*src & 0xF0) == 0xE0)
    {
      dst = (*src++ & 0x0f) * 0x40;
      if (++forward > len)
        return ACEXML_END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return ACEXML_NON_UNICODE;
      dst = (dst + (*src++ & 0x3f)) * 0x40;
      if (++forward > len)
        return ACEXML_END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return ACEXML_NON_UNICODE;
      dst += *src & 0x3f;
    }
  else if ((*src & 0xF8) == 0xF0)
    {
      dst = (*src++ & 0x0f) * 0x40;
      if (++forward > len)
        return ACEXML_END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return ACEXML_NON_UNICODE;
      dst = (dst + (*src++ & 0x3f)) * 0x40;
      if (++forward > len)
        return ACEXML_END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return ACEXML_NON_UNICODE;
      dst = (dst + (*src++ & 0x3f)) * 0x40;
      if (++forward > len)
        return ACEXML_END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return ACEXML_NON_UNICODE;
      dst += *src & 0x3f;
    }
  else
    {
      return ACEXML_NON_UNICODE;
    }

  return ACE_Utils::truncate_cast<int> (forward);
}

int
ACEXML_Transcoder::utf162ucs4 (const ACEXML_UTF16 *src,
                               size_t len,
                               ACEXML_UCS4 &dst)
{
  if (src == 0)
    {
      return ACEXML_INVALID_ARGS;
    }

  size_t forward = 1;
  if (*src >= 0xDC00 && *src < 0xE000)
    {
      if (len < 2)
        {
          return ACEXML_END_OF_SOURCE;
        }

      return ACEXML_Transcoder::surrogate2ucs4 (*src,
                                                *(src+1),
                                                dst);
    }
  else
    {
      if (len < 1)
        {
          return ACEXML_END_OF_SOURCE;
        }

      dst = *src;
    }

  return ACE_Utils::truncate_cast<int> (forward);
}

int
ACEXML_Transcoder::utf8s2utf16s (const ACEXML_UTF8 *src,
                                 ACEXML_UTF16 *dst,
                                 size_t len)
{
  if (src == 0 || dst == 0)
    {
      return ACEXML_INVALID_ARGS;
    }

  size_t src_len = ACE_OS::strlen (src) + 1;

  size_t total_len = 0;
  int forward;
  ACEXML_UCS4 temp;

  while (src_len > 0)
    {
      if ((forward = ACEXML_Transcoder::utf82ucs4 (src,
                                                   src_len,
                                                   temp)) <= 0)
        return forward;

      src += forward;
      src_len -= forward;

      if ((forward = ACEXML_Transcoder::ucs42utf16 (temp,
                                                    dst,
                                                    len)) <= 0)
        return forward;

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
  if (src == 0 || dst == 0)
    return ACEXML_INVALID_ARGS;

  size_t src_len = 1;
  for (const ACEXML_UTF16 *p = src; *p++ != 0; ++src_len)
    ;

  size_t total_len = 0;
  int forward;
  ACEXML_UCS4 temp;

  while (src_len > 0)
    {
      if ((forward = ACEXML_Transcoder::utf162ucs4 (src,
                                                    src_len,
                                                    temp)) <= 0)
        return forward;

      src += forward;
      src_len -= forward;

      if ((forward = ACEXML_Transcoder::ucs42utf8 (temp,
                                                   dst,
                                                   len)) <= 0)
        return forward;

      total_len += forward;
      dst += forward;
      len -= forward;
    }

  return ACE_Utils::truncate_cast<int> (total_len);
}
