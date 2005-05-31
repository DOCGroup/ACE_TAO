// -*- C++ -*-

//=============================================================================
/**
 *  @file    ParserInternals.h
 *
 *  $Id$
 *
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_PARSER_INTERNALS_H_
#define _ACEXML_PARSER_INTERNALS_H_

#include /**/ "ace/pre.h"
#include "ACEXML/parser/parser/Parser_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"

/**
 * @class ACEXML_ParserInt ParserInternals.h "ACEXML/parser/parser/ParserInternals.h"
 *
 * @brief A class to hide some of the internal implementation details of
 *        the parser.
 *
 */
class ACEXML_PARSER_Export ACEXML_ParserInt
{

public:
#if defined (ACE_USES_WCHAR)

  /**
   * Check if a character @a c is a BaseChar. This method checks for the
   * complete set of characters allowed when WCHAR support is enabled.
   *
   * @retval 1 if @a c is a valid XML Character, 0 otherwise.
   */
  static int isBasechar_i (const ACEXML_Char c);

  /**
   * Check if a character @a c is a Ideographic. This method checks for the
   * complete set of characters allowed when WCHAR support is enabled.
   *
   * @retval 1 if @a c is a valid XML Character, 0 otherwise.
   */
  static int isIdeographic_i (const ACEXML_Char c);

  /**
   * Check if a character @a c is a CombiningChar. This method checks for the
   * complete set of characters allowed when WCHAR support is enabled.
   *
   * @retval 1 if @a c is a valid XML Character, 0 otherwise.
   */
  static int isCombiningchar_i (const ACEXML_Char c);

  /**
   * Check if a character @a c is a Digit. This method checks for the
   * complete set of characters allowed when WCHAR support is enabled.
   *
   * @retval 1 if @a c is a valid XML Character, 0 otherwise.
   */
  static int isDigit_i (const ACEXML_Char c);

  /**
   * Check if a character @a c is an Extender. This method checks for the
   * complete set of characters allowed when WCHAR support is enabled.
   *
   * @retval 1 if @a c is a valid XML Character, 0 otherwise.
   */
  static int isExtender_i (const ACEXML_Char c);

#endif /* ACE_USES_WCHAR */

  static const ACEXML_Char base_char_table_[256];

  static const ACEXML_Char* msg_[];

  static const ACEXML_Char* predef_ent_[];

  static const ACEXML_Char* predef_val_[];

  // Enum describing the position in a document when a reference occurs.
  enum ReferenceState {
    IN_CONTENT,
    IN_ATT_VALUE,
    AS_ATT_VALUE,
    IN_ENTITY_VALUE,
    IN_INT_DTD,
    IN_EXT_DTD,
    IN_NOTATION,
    INVALID = -1
  };

};

#include /**/ "ace/post.h"

#endif /* _ACEXML_PARSER_INTERNALS_H_ */
