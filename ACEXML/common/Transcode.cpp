// -*- C++ -*-  $Id$

#include "common/Transcode.h"

#if !defined (__ACEXML_INLINE__)
# include "common/Transcode.i"
#endif /* __ACEXML_INLINE__ */

int
ACEXML_Transcoder::utf8s2utf16s (const ACEXML_UTF8 *src,
                                ACEXML_UTF16 *dst,
                                size_t len)
{
  if (src == 0 || dst == 0)
    return INVALID_ARGS;

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

  return total_len;
}

int
ACEXML_Transcoder::utf16s2utf8s (const ACEXML_UTF16 *src,
                                ACEXML_UTF8 *dst,
                                size_t len)
{
  if (src == 0 || dst == 0)
    return INVALID_ARGS;

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

  return total_len;
}
