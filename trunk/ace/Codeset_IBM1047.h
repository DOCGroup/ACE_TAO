// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Codeset_IBM1047.cpp
//
// = DESCRIPTION
//    Declares the arrays required to convert between ISO8859 (aka
//    Latin/1) and IBM1047 (aka EBCDIC).
//
// = AUTHOR
//   Jim Rogers (jrogers@viasoft.com)
//
// ============================================================================

#ifndef ACE_CODESET_IMB1047_H
#define ACE_CODESET_IMB1047_H
#include "ace/pre.h"

#include "ace/inc_user_config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(ACE_MVS)

#include "ace/CDR_Stream.h"

extern ACE_Export char ACE_to_IBM1047[257];
extern ACE_Export char ACE_from_IBM1047[257];

// ****************************************************************

class ACE_Export ACE_IBM1047_ISO8859 : public ACE_Char_Codeset_Translator
{
  // = TITLE
  //     Codeset translation specialization.
  //
  // = DESCRIPTION
  //     This class performs the codeset translation:
  //
  //     Native:        IBM_1047 (i.e. EBCDIC)
  //     Stream:        ISO-8859 (i.e. Latin/1)
  //
public:
  ACE_IBM1047_ISO8859 (void);
  // A do nothing constructor.

  virtual ~ACE_IBM1047_ISO8859 (void);
  // Virtual destruction

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
};

class ACE_Export ACE_ISO8859_IBM1047 : public ACE_Char_Codeset_Translator
{
  // = TITLE
  //     Codeset translation specialization.
  //
  // = DESCRIPTION
  //     This class performs the codeset translation:
  //
  //     Native:        ISO-8859 (i.e. Latin/1)
  //     Stream:        IBM-1047 (i.e. EBCDIC)
  //
public:
  ACE_ISO8859_IBM1047 (void);
  // A do nothing constructor.

  virtual ~ACE_ISO8859_IBM1047 (void);
  // Virtual destruction

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
};

#endif /* ACE_MVS */

#include "ace/post.h"
#endif /* ACE_CODESET_IMB1047_H */
