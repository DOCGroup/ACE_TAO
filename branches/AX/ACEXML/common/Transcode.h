// -*- C++ -*-

//=============================================================================
/**
 *  @file    Transcode.h
 *
 * This file declares functions to convert char string among different
 * unicode encoding (utf8, utf16, utf32)
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_TRANSCODE_H_
#define _ACEXML_TRANSCODE_H_
#include "common/XML_Types.h"

/**
 * @class ACEXML_Transcoder Transcode.h "common/Transcode.h"
 *
 * @brief ACEXML_Transcoder
 *
 * Wrapper class for performing transcoding among different UNICODE
 * encoding.
 */
class ACEXML_Export ACEXML_Transcoder
{
public:
  enum
  {
    SUCCESS = 0,
    DESTINATION_TOO_SHORT = -1,
    END_OF_SOURCE = -2,
    INVALID_ARGS = -3,
    IS_SURROGATE = -4,
    NON_UNICODE = -5
  };


  // The following functions translate a unicode characters
  // into different encoding.  Return number of characters put into
  // destination or consumed from src if success without
  // error, otherwise, return corresponding error code.
  static int utf162utf8 (ACEXML_UTF16 src,
                         ACEXML_UTF8 *dst,
                         size_t len);

  static int ucs42utf8 (ACEXML_UCS4 src,
                        ACEXML_UTF8 *dst,
                        size_t len);

  static int ucs42utf16 (ACEXML_UCS4 src,
                         ACEXML_UTF16 *dst,
                         size_t len);

  static int surrogate2utf8 (ACEXML_UTF16 high,
                             ACEXML_UTF16 low,
                             ACEXML_UTF8 *dst,
                             size_t len);

  static int surrogate2ucs4 (ACEXML_UTF16 high,
                             ACEXML_UTF16 low,
                             ACEXML_UCS4 &dst);

  static int utf82ucs4 (const ACEXML_UTF8 *src,
                        size_t len,
                        ACEXML_UCS4 &dst);

  static int utf162ucs4 (const ACEXML_UTF16 *src,
                        size_t len,
                        ACEXML_UCS4 &dst);

//    static int utf82utf16 (const ACEXML_UTF8 *src,
//                           size_t len,
//                           ACEXML_UTF16 &dst);
  // This function does not handle surrogates.

  // The following functions are non-inlined:
  static int utf8s2utf16s (const ACEXML_UTF8 *src,
                           ACEXML_UTF16 *dst,
                           size_t len);

  static int utf16s2utf8s (const ACEXML_UTF16 *src,
                           ACEXML_UTF8 *dst,
                           size_t len);
};

#if defined (__ACEXML_INLINE__)
# include "common/Transcode.i"
#endif /* __ACEXML_INLINE__ */
#endif /* _ACEXML_TRANSCODE_H_ */
