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

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"

/**
 * @class ACEXML_Transcoder
 *
 * @brief ACEXML_Transcoder
 *
 * Wrapper class for performing transcoding among different UNICODE
 * encoding.
 */
class ACEXML_Export ACEXML_Transcoder
{
public:
  /*
   * Status of the conversion function.
   */
  enum
  {
    ACEXML_SUCCESS = 0,
    ACEXML_DESTINATION_TOO_SHORT = -1,
    ACEXML_END_OF_SOURCE = -2,
    ACEXML_INVALID_ARGS = -3,
    ACEXML_IS_SURROGATE = -4,
    ACEXML_NON_UNICODE = -5
  } ACEXML_STATUS;


  // The following functions translate a unicode characters
  // into different encoding.  Return number of characters put into
  // destination or consumed from src if success without
  // error, otherwise, return corresponding error code.
  /*
   * Convert a UTF-16 character into a string in UTF-8 encoding.
   *
   * @return number of characters the function uses to store the
   *         converted string if it succeeds or one of the error STATUS
   *         otherwise.
   */
  static int utf162utf8 (ACEXML_UTF16 src,
                         ACEXML_UTF8 *dst,
                         size_t len);

  /*
   * Convert a UCS-4 character into a string in UTF-8 encoding.
   *
   * @return number of characters the function uses to store the
   *         converted string if it succeeds or one of the error STATUS
   *         otherwise.
   */
  static int ucs42utf8 (ACEXML_UCS4 src,
                        ACEXML_UTF8 *dst,
                        size_t len);

  /*
   * Convert a UCS-4 character into a string in UTF-16 encoding.
   *
   * @return number of characters the function uses to store the
   *         converted string if it succeeds or one of the error STATUS
   *         otherwise.
   */
  static int ucs42utf16 (ACEXML_UCS4 src,
                         ACEXML_UTF16 *dst,
                         size_t len);

  /*
   * Convert a UTF-16 surrogate character pair into a string in UTF-8 encoding.
   *
   * @return number of characters the function uses to store the
   *         converted string if it succeeds or one of the error STATUS
   *         otherwise.
   */
  static int surrogate2utf8 (ACEXML_UTF16 high,
                             ACEXML_UTF16 low,
                             ACEXML_UTF8 *dst,
                             size_t len);

  /*
   * Convert a UTF-16 surrogate character pair into a UCS-4 character.
   *
   * @return SUCCESS if it succeeds or one of the error STATUS
   *         otherwise.
   */
  static int surrogate2ucs4 (ACEXML_UTF16 high,
                             ACEXML_UTF16 low,
                             ACEXML_UCS4 &dst);

  /*
   * Convert the first UNICODE character in a UTF-8 character string
   * into a UCS-4 character.
   *
   * @return number of characters the function consumed from the
   *         UTF-8 string if it succeeds or one of the error STATUS
   *         otherwise.
   */
  static int utf82ucs4 (const ACEXML_UTF8 *src,
                        size_t len,
                        ACEXML_UCS4 &dst);

  /*
   * Convert the first UNICODE character in a UTF-16 character string
   * into a UCS-4 character.
   *
   * @return number of characters the function consumed from the
   *         UTF-16 string if it succeeds or one of the error STATUS
   *         otherwise.
   */
  static int utf162ucs4 (const ACEXML_UTF16 *src,
                        size_t len,
                        ACEXML_UCS4 &dst);

//    static int utf82utf16 (const ACEXML_UTF8 *src,
//                           size_t len,
//                           ACEXML_UTF16 &dst);
  // This function does not handle surrogates.

  // = The following functions are non-inlined:

  /*
   * Convert a UTF-8 string into a UTF-16 string.
   *
   * @param len The length of @a dst string.
   *
   * @return number of characters the function consumed from the
   *         UTF-8 string if it succeeds or one of the error STATUS
   *         otherwise.
   */
  static int utf8s2utf16s (const ACEXML_UTF8 *src,
                           ACEXML_UTF16 *dst,
                           size_t len);

  /*
   * Convert a UTF-16 string into a UTF-8 string.
   *
   * @param len The length of @a dst string.
   *
   * @return number of characters the function uses in
   *         UTF-8 string if it succeeds or one of the error STATUS
   *         otherwise.
   */
  static int utf16s2utf8s (const ACEXML_UTF16 *src,
                           ACEXML_UTF8 *dst,
                           size_t len);
};

#include /**/ "ace/post.h"

#endif /* _ACEXML_TRANSCODE_H_ */
