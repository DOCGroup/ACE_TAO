// -*- C++ -*-  $Id$

ACEXML_INLINE int
ACEXML_Transcoder::utf162utf8 (ACEXML_UTF16 src,
                               ACEXML_UTF8 *dst,
                               size_t len)
{
  // Check for valid argument first...

  if (dst == 0)
    return INVALID_ARGS;

  if (src < 0x80)
    {
      if (len < 1)
        return DESTINATION_TOO_SHORT;

      *dst = ACE_static_cast (ACEXML_UTF8, src);
      return 1;
    }
  else if (src < 0x800)
    {
      if (len < 2)
        return DESTINATION_TOO_SHORT;

      *dst = 0xc0 | (src / 0x40);
      *(dst+1) = 0x80 | (src % 0x40);
      return 2;
    }
  else
    {
      if (len < 3)
        return DESTINATION_TOO_SHORT;

      // Surrogates (0xD800 - 0xDFFF) are not valid unicode values
      if (src >= 0xD800 && src < 0xE000)
        return IS_SURROGATE;

      *dst = 0xe0 | (src / 0x1000);
      *(dst+1) = 0x80 | ((src % 0x1000) / 0x40);
      *(dst+2) = 0x80 | (src % 0x40);
      return 3;
    }
  ACE_NOTREACHED (return NON_UNICODE;)
}

ACEXML_INLINE int
ACEXML_Transcoder::ucs42utf8 (ACEXML_UCS4 src,
                              ACEXML_UTF8 *dst,
                              size_t len)
{
  if (src < 0x10000)
    {
      int retv = ACEXML_Transcoder::utf162utf8
        (ACE_static_cast (ACEXML_UTF16, src),
         dst, len);
      return (retv == IS_SURROGATE ? NON_UNICODE : retv);
    }
  else if (src >= 0x100000 && src < 0x110000)
    {
      if (len < 4)
        return DESTINATION_TOO_SHORT;

      if (dst == 0)
        return INVALID_ARGS;

      *dst = 0xf0 | (src / 0x40000);
      *(dst+1) = 0x80 | ((src % 0x40000) / 0x1000);
      *(dst+2) = 0x80 | ((src % 0x1000) / 0x40);
      *(dst+3) = 0x80 | (src % 0x40);
      return 4;
    }
  return NON_UNICODE;
}


ACEXML_INLINE int
ACEXML_Transcoder::ucs42utf16 (ACEXML_UCS4 src,
                               ACEXML_UTF16 *dst,
                               size_t len)
{
  if (dst == 0)
    return INVALID_ARGS;

  if (src < 0x10000)
    {
      if (len < 1)
        return DESTINATION_TOO_SHORT;

      if (src >= 0xD800 && src < 0xE000)
        return NON_UNICODE;     // Surrogates are not valid unicode value

      *dst = ACE_static_cast (ACEXML_UTF16, src);
      return 1;
    }
  else if (src >= 0x100000 && src < 0x110000)
    // Scalar values are encoded into surrogates
    {
      if (len < 2)
        return DESTINATION_TOO_SHORT;

      *dst = 0xD800 | (src / 0x400);
      *(dst+1) = 0xDC00 | (src % 0x400);
      return 2;
    }

  return NON_UNICODE;
}

ACEXML_INLINE int
ACEXML_Transcoder::surrogate2utf8 (ACEXML_UTF16 high,
                                   ACEXML_UTF16 low,
                                   ACEXML_UTF8 *dst,
                                   size_t len)
{
  if (len < 3)
    return DESTINATION_TOO_SHORT;

  if (dst == 0 ||
      (high >= 0xD800 && high < 0xDC00) ||
      (low >= 0xDC00 && low < 0xE000))
    return INVALID_ARGS;

  ACEXML_UCS4 src = (high - 0xD800) * 0x400 + (low - 0xDC00) + 0x10000;
  *dst = 0xD800 | (src / 0x400);
  *(dst+1) = 0xDC00 | (src % 0x400);
  return 2;
}

ACEXML_INLINE int
ACEXML_Transcoder::surrogate2ucs4 (ACEXML_UTF16 high,
                                   ACEXML_UTF16 low,
                                   ACEXML_UCS4 &dst)
{
  if ((high >= 0xD800 && high < 0xDC00) ||
      (low >= 0xDC00 && low < 0xE000))
    return INVALID_ARGS;

  dst = (high - 0xD800) * 0x400 + (low - 0xDC00) + 0x10000;
  return SUCCESS;
}

ACEXML_INLINE int
ACEXML_Transcoder::utf82ucs4 (const ACEXML_UTF8 *the_src,
                              size_t len,
                              ACEXML_UCS4 &dst)
{
  if (the_src == 0)
    return INVALID_ARGS;

  const unsigned char *src = ACE_reinterpret_cast (const unsigned char *,
                                                   the_src);

  size_t forward = 1;

  if (forward > len)
    return END_OF_SOURCE;

  if (ACE_static_cast (unsigned char, *src) < 0x80)
    dst = *src;
  else if ((*src & 0xE0) == 0xC0)
    {
      dst = (*(src++) & 0x1f) * 0x40;
      if (++forward > len)
        return END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return NON_UNICODE;     // Error transcoding unicode scalar
      dst += *src & 0x3f;
    }
  else if ((*src & 0xF0) == 0xE0)
    {
      dst = (*src++ & 0x0f) * 0x40;
      if (++forward > len)
        return END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return NON_UNICODE;
      dst = (dst + (*src++ & 0x3f)) * 0x40;
      if (++forward > len)
        return END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return NON_UNICODE;
      dst += *src & 0x3f;
    }
  else if ((*src & 0xF8) == 0xF0)
    {
      dst = (*src++ & 0x0f) * 0x40;
      if (++forward > len)
        return END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return NON_UNICODE;
      dst = (dst + (*src++ & 0x3f)) * 0x40;
      if (++forward > len)
        return END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return NON_UNICODE;
      dst = (dst + (*src++ & 0x3f)) * 0x40;
      if (++forward > len)
        return END_OF_SOURCE;
      if ((*src & 0xC0) != 0x80)
        return NON_UNICODE;
      dst += *src & 0x3f;
    }
  else
    return NON_UNICODE;

  return forward;
}

ACEXML_INLINE int
ACEXML_Transcoder::utf162ucs4 (const ACEXML_UTF16 *src,
                               size_t len,
                               ACEXML_UCS4 &dst)
{
  if (src == 0)
    return INVALID_ARGS;

  size_t forward = 1;
  if (*src >= 0xDC00 && *src < 0xE000)
    {
      if (len < 2)
        return END_OF_SOURCE;
      return ACEXML_Transcoder::surrogate2ucs4 (*src,
                                                *(src+1),
                                                dst);
    }
  else
    {
      if (len < 1)
        return END_OF_SOURCE;
      dst = *src;
    }

  return forward;
}
