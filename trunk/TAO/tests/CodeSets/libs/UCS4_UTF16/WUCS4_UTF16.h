// -*- C++ -*-

//=============================================================================
/**
 *  @file    WUCS4_UTF16.h
 *
 *  $Id$
 *
 *
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef WUCS4_UTF16_H
#define WUCS4_UTF16_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CDR_Stream.h"
#include "WUCS4_UTF16_export.h"

// ****************************************************************

/**
 * @class ACE_Char_UCS4_UTF16
 *
 * @brief Codeset translation specialization.
 *
 * This class performs the codeset translation:
 *   - Native:        UCS4 (4-byte codepoints, lower 16 match Unicode)
 *   - Stream:        UTF16 (i.e. Unicode)
 *    Defines the actions required to convert between UCS-4, a 4 byte wide char
 *    codeset as the native wchar codeset, and UCS-16, aka unicode, a 2-byte
 *    codeset for the transport wchar codeset.
 *
 *    This translator does not alter any codepoint values, but it does
 *    illustrate how to deal with the unicode byte order marker (BOM) and how
 *    to deal with codepoint size differences.
 *
 *    Note that the BOM is an optional value. The write_*() methods do not
 *    encode this value. When BOM is not encoded, the byte order of the
 *    encapsulation is used.
 */
class UCS4_UTF16_Export WUCS4_UTF16 : public ACE_WChar_Codeset_Translator
{
public:
  /// A do nothing constructor.
  WUCS4_UTF16 (void);

  /// Virtual destruction
  virtual ~WUCS4_UTF16 (void);

  // = Documented in $ACE_ROOT/ace/CDR_Stream.h
  virtual ACE_CDR::Boolean read_wchar (ACE_InputCDR &,
                                      ACE_CDR::WChar &);
  virtual ACE_CDR::Boolean read_wstring (ACE_InputCDR &,
                                        ACE_CDR::WChar *&);
  virtual ACE_CDR::Boolean read_wchar_array (ACE_InputCDR &,
                                            ACE_CDR::WChar *,
                                            ACE_CDR::ULong);
  virtual ACE_CDR::Boolean write_wchar (ACE_OutputCDR &,
                                       ACE_CDR::WChar);
  virtual ACE_CDR::Boolean write_wstring (ACE_OutputCDR &,
                                         ACE_CDR::ULong,
                                         const ACE_CDR::WChar *);
  virtual ACE_CDR::Boolean write_wchar_array (ACE_OutputCDR &,
                                             const ACE_CDR::WChar *,
                                             ACE_CDR::ULong);
  virtual ACE_CDR::ULong ncs () {return 0x00010104;}
  virtual ACE_CDR::ULong tcs () {return 0x00010109;}

private:
  ACE_CDR::Octet tcs_size_;
};

#include "ace/post.h"
#endif /* WCHAR_UCS4_UTF16_TRANSLATOR */
