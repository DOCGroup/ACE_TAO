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
#include /**/ "ace/pre.h"

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
 *   - Native:        IBM_1047 (i.e. EBCDIC)
 *   - Stream:        ISO-8859 (i.e. Latin/1)
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
  ACE_CDR::Boolean read_wchar_array_i (ACE_InputCDR &,
                                       ACE_CDR::WChar *,
                                       ACE_CDR::ULong&,
                                       int adjust_len = 0);

  ACE_CDR::Boolean write_wchar_i (ACE_OutputCDR &,
                                  ACE_CDR::WChar ,
                                  int use_BOM,
                                  int encode_len);

  /**
    Due to surrogate pair substution the transmitted length of a wstring can
    differ from the logical strength length.  This version of write_wchar_array
    accepts both lengths and uses them as necessary.
   */
  ACE_CDR::Boolean write_measured_wchar_array (
      ACE_OutputCDR & cdr,
      const ACE_CDR::WChar *x,
      ACE_CDR::ULong length,
      ACE_CDR::ULong transmission_length);

};

#include /**/ "ace/post.h"
#endif /* WCHAR_UCS4_UTF16_TRANSLATOR */
