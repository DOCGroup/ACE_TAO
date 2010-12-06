// -*- C++ -*-

//=============================================================================
/**
 *  @file    UTF8_Latin1_Translator.h
 *
 *  $Id$
 *
 *
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef UTF8_LATIN1_TRANSLATOR_H
#define UTF8_LATIN1_TRANSLATOR_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Codeset/codeset_export.h"
#include "tao/Versioned_Namespace.h"
#include "ace/CDR_Stream.h"

// ****************************************************************

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class UTF8_Latin1_Translator
 *
 * @brief Codeset translation specialization - Manages Byte Order Marker
 *
 * This class performs the codeset translation:
 *   - Native:        ISO-8859-1 (i.e. Latin 1)
 *   - Stream:        UTF-8 (8 bit unicode mapping)
 *
 * When writing chars, it is possible to raise a DATA_CONVERSION exception
 * because some valid Latin codepoints map to 2-byte utf-8 codepoints.
 *
 * When reading strings, it is possible to raise a DATA_CONVERSION exception
 * because the string may include utf-8 unicode codepoints > 0x00FF.
 */
class TAO_Codeset_Export TAO_UTF8_Latin1_Translator
  : public ACE_Char_Codeset_Translator
{
public:
  /// constructor
  TAO_UTF8_Latin1_Translator ();

  /// Virtual destruction
  virtual ~TAO_UTF8_Latin1_Translator (void);

  // = Documented in $ACE_ROOT/ace/CDR_Stream.h
  virtual ACE_CDR::Boolean read_char (ACE_InputCDR &,
                                      ACE_CDR::Char &);
  virtual ACE_CDR::Boolean read_string (ACE_InputCDR &,
                                        ACE_CDR::Char *&);
  virtual ACE_CDR::Boolean read_char_array (ACE_InputCDR &,
                                            ACE_CDR::Char *,
                                            ACE_CDR::ULong);
  virtual ACE_CDR::Boolean write_char (ACE_OutputCDR &,
                                       ACE_CDR::Char);
  virtual ACE_CDR::Boolean write_string (ACE_OutputCDR &,
                                         ACE_CDR::ULong,
                                         const ACE_CDR::Char *);
  virtual ACE_CDR::Boolean write_char_array (ACE_OutputCDR &,
                                             const ACE_CDR::Char *,
                                             ACE_CDR::ULong);
  virtual ACE_CDR::ULong ncs () {return 0x00010001U;}
  virtual ACE_CDR::ULong tcs () {return 0x05010001U;}

private:
  ACE_CDR::ULong read_char_i (ACE_InputCDR &,
                                ACE_CDR::Char &);

  ACE_CDR::Boolean write_char_i (ACE_OutputCDR &,
                                 ACE_CDR::Char);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* UTF8_LATIN1_TRANSLATOR_H */
