// -*- C++ -*-

/**
 *  @file   XML_Codecs.h
 *
 *  $Id$
 *
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 *
 *  XML_Codecs is a generic wrapper for various encoding and decoding
 *  mechanisms used in ACEXML. Currently it includes support for handling
 *  Base64 content transfer-encoding of ACEXML_Chars.
 *
 */

#ifndef _ACEXML_XML_CODECS_H
#define _ACEXML_XML_CODECS_H

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"
#include "ace/Codecs.h"

/**
 * @class ACEXML_Base64
 *
 * @brief Encode/Decode a stream of ACEXML_Chars according to Base64 encoding.
 *
 * This class provides methods to encode or decode a stream of ACEXML_Chars
 * to/from Base64 encoding. It doesn't convert the input stream to a
 * canonical form before encoding.
 *
 */
class ACEXML_Export ACEXML_Base64 : public ACE_Base64
{
public:

  //@{

  /**
   * Encodes a stream of octets to Base64 data
   *
   * @param input Binary data in ACEXML_Char stream.
   * @param output_len Length of the encoded Base64 ACEXML_Char stream.
   * @return Encoded Base64 data in ACEXML_Char stream or NULL if input data
   *         cannot be encoded.
   */

  static ACEXML_Char* encode (const ACEXML_Char* input,
                              size_t* output_len);
  /**
   * Decodes a stream of Base64 to octets data
   *
   * @param input Encoded Base64 data in ACEXML_Char stream.
   * @param output_len Length of the binary ACEXML_Char stream.
   * @return Binary data in ACEXML_Char stream or NULL if input data cannot
   *         be encoded.
   */
  static ACEXML_Char* decode (const ACEXML_Char* input,
                              size_t* output_len);

  //@}
};


#include /**/ "ace/post.h"

#endif /* _ACEXML_XML_CODECS_H */
