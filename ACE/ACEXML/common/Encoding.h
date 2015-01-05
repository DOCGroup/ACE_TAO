// -*- C++ -*-

//=============================================================================
/**
 *  @file    Encoding.h
 *
 * This file provides utility functions to determine the encoding of a file
 * or a byte stream automatically.
 *
 *  $Id$
 *
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_ENCODING_H
#define _ACEXML_ENCODING_H

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"

/**
 * @class ACEXML_Encoding Encoding.h
 *
 * @brief ACEXML_Encoding
 *
 * Wrapper class for determining the encoding of a file or a byte stream.
 */
class ACEXML_Export ACEXML_Encoding
{
public:
  enum {
    UCS4BE,
    UCS4LE,
    UCS4_2143,
    UCS4_3412,
    UTF16BE,
    UTF16LE,
    UTF8,
    OTHER
  } ENCODING;

  static const ACEXML_Char* encoding_names_[8];

  static const ACEXML_UTF8 byte_order_mark_[][4];

  static const ACEXML_UTF8 magic_values_[][4];

  static const ACEXML_Char* get_encoding (const char* input);

};

#include /**/ "ace/post.h"

#endif /* _ACEXML_ENCODING_H */
