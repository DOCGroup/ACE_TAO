// -*- C++ -*-

//=============================================================================
/**
 *  @file    Wchar_IBM1047_ISO8859_Translator.h
 *
 *  $Id$
 *
 *  Declares the arrays required to convert between IBM1047 (aka EBCDIC)  and ISO8859 (aka
 *  Latin/1).
 *
 *
 *  @author Jim Rogers (jrogers@viasoft.com)
 */
//=============================================================================

#ifndef WCHAR_IBM1047_ISO8859_TRANSLATOR_H
#define WCHAR_IBM1047_ISO8859_TRANSLATOR_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CDR_Stream.h"
#include "IBM1047_ISO8859_export.h"

extern IBM1047_ISO8859_Export ACE_CDR::WChar Wchar_to_IBM1047[257];
extern IBM1047_ISO8859_Export ACE_CDR::WChar Wchar_from_IBM1047[257];

// ****************************************************************

/**
 * @class ACE_Char_IBM1047_ISO8859
 *
 * @brief Codeset translation specialization.
 *
 * This class performs the codeset translation:
 *   - Native:        IBM_1047 (i.e. EBCDIC)
 *   - Stream:        ISO-8859 (i.e. Latin/1)
 */
class IBM1047_ISO8859_Export WIBM1047_ISO8859 : public ACE_WChar_Codeset_Translator
{
public:
  /// A do nothing constructor.
  WIBM1047_ISO8859 (void);

  /// Virtual destruction
  virtual ~WIBM1047_ISO8859 (void);

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
  virtual ACE_CDR::ULong ncs () {return 0x10020417;}
  virtual ACE_CDR::ULong tcs () {return 0x00010001;}
};

#include "ace/post.h"
#endif /* WCHAR_IBM1047_ISO8859_TRANSLATOR */

