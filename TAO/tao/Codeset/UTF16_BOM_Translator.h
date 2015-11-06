// -*- C++ -*-

//=============================================================================
/**
 *  @file  UTF16_BOM_Translator.h
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef UTF16_BOM_TRANSLATOR_H
#define UTF16_BOM_TRANSLATOR_H
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
 * @class TAO_UTF16_BOM_Translator
 *
 * @brief Codeset translation specialization - Manages Byte Order Marker
 *
 * This class performs the codeset translation:
 *   - Native:        UTF16 (i.e. Unicode)
 *   - Stream:        UTF16 with Byte Order Marker
 */
class TAO_Codeset_Export TAO_UTF16_BOM_Translator
  : public ACE_WChar_Codeset_Translator
{
public:
  /// Constructor
  /// @param forceBE: true forces all wchar, warray, and wstrings to big-endian byte order
  TAO_UTF16_BOM_Translator (bool forceBE);

  /// Virtual destruction
  virtual ~TAO_UTF16_BOM_Translator (void);

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
  virtual ACE_CDR::ULong ncs () {return 0x00010109;}
  virtual ACE_CDR::ULong tcs () {return 0x00010109;}

private:
  ACE_CDR::Boolean read_wchar_array_i (ACE_InputCDR &,
                                       ACE_CDR::WChar *,
                                       ACE_CDR::ULong&,
                                       int adjust_len = 0);

  ACE_CDR::Boolean write_wchar_array_i (ACE_OutputCDR &,
                                        const ACE_CDR::WChar *,
                                        ACE_CDR::ULong);

  ACE_CDR::Boolean write_swapped_wchar_array_i (ACE_OutputCDR & cdr,
                                            const ACE_CDR::WChar *x,
                                            ACE_CDR::ULong length);


  ACE_CDR::Boolean write_wchar_i (ACE_OutputCDR &,
                                  ACE_CDR::WChar ,
                                  bool allow_BOM);

private:
  /// if this flag is true, force wchar's to big endian order
  bool forceBE_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* UTF16_BOM_TRANSLATOR_H */
