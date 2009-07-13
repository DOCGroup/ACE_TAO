// -*- C++ -*-

//=============================================================================
/**
 *  @file    UTF16_UCS2_Translator.h
 *
 *  $Id$
 *
 *  @author Iliyan Jeliazkov <jeliazkov_i@ociweb.com>
 *  @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#ifndef UTF16_UCS2_TRANSLATOR_H
#define UTF16_UCS2_TRANSLATOR_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CDR_Stream.h"
#include "ace/Codeset_Symbols.h"
#include "tao/CONV_FRAMEC.h"
#include "UTF16_UCS2_export.h"

// ****************************************************************

/**
 * @class UTF16_UCS2_Translator
 *
 * @brief Codeset translation specialization
 *
 * This class performs the codeset translation:
 *   - Native:        UTF16 (i.e. Unicode)
 *   - Stream:        UCS2
 */
class UTF16_UCS2_Export UTF16_UCS2_Translator : public ACE_WChar_Codeset_Translator
{
public:
  /// constructor
  /// @param tcs: the target codeset id to use (and report)
  UTF16_UCS2_Translator (CONV_FRAME::CodeSetId tcs = ACE_CODESET_ID_ISO_UCS_2_LEVEL_1);

  /// Virtual destruction
  virtual ~UTF16_UCS2_Translator (void);

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
  virtual ACE_CDR::ULong tcs () {return this->tcs_;}

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
                                  ACE_CDR::WChar);

private:
  CONV_FRAME::CodeSetId tcs_;


};

#include /**/ "ace/post.h"
#endif /* UTF16_UCS2_TRANSLATOR_H */
