// $Id$


// Portions of this file are:
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved

// CDR:         Encode/Decode basic machine data types
//
// Implementation of OMG "Common Data Representation" (CDR) ... there
// are one routine each for byte/halfword/word/doubleword put/get,
// which adjust to establish "natural" alignment (the bulk of the
// code) and then put or get with byteswapping as needed.
//
// The implementation knows that native data formats are conformant
// with OMG-IDL's (and hence CDR's) size requirements, and relies on
// the fact that (for example) CORBA_Long is always four bytes long
// even if the environment's "int" is a different size.
//
//      char, octet                       8 bits (1 byte)
//      short, unsigned short            16 bits (2 bytes)
//      long, unsigned long, float       32 bits (4 bytes)
//      double, (unsigned) long long     64 bits (8 bytes)
//      long double                     128 bits (16 bytes)
//
// Moreover, this "knows" that the native 'char' represents ISO
// Latin/1 characters (an ASCII superset addressing Western European
// characters) and that "double" and "float" comply with the IEEE
// standards. (The "long double" may not be a native data type,
// though.)
//
// THREADING NOTE: "CDR" is a data structure which must be protected
// by external critical sections.

#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/ORB_Core.h"
#include "tao/Environment.h"

#if !defined (__ACE_INLINE__)
# include "tao/CDR.i"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID(tao, CDR, "$Id$")



#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_CDR_Timeprobe_Description[] =
{
  "OutputCDR::ctor[1] - enter",
  "OutputCDR::ctor[1] - leave",
  "OutputCDR::ctor[2] - enter",
  "OutputCDR::ctor[2] - leave",
  "OutputCDR::ctor[3] - enter",
  "OutputCDR::ctor[3] - leave"
};

enum
{
  TAO_OUTPUT_CDR_CTOR1_ENTER = 2000,
  TAO_OUTPUT_CDR_CTOR1_LEAVE,
  TAO_OUTPUT_CDR_CTOR2_ENTER,
  TAO_OUTPUT_CDR_CTOR2_LEAVE,
  TAO_OUTPUT_CDR_CTOR3_ENTER,
  TAO_OUTPUT_CDR_CTOR3_LEAVE
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_CDR_Timeprobe_Description,
                                  TAO_OUTPUT_CDR_CTOR1_ENTER);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_OutputCDR::TAO_OutputCDR (size_t size,
                              int byte_order,
                              ACE_Allocator *buffer_allocator,
                              ACE_Allocator *data_block_allocator,
                              ACE_Allocator* message_block_allocator,
                              size_t memcpy_tradeoff,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version,
                              ACE_Char_Codeset_Translator *char_translator,
                              ACE_WChar_Codeset_Translator *wchar_translator)
  :  ACE_OutputCDR (size,
                    byte_order,
                    buffer_allocator,
                    data_block_allocator,
                    message_block_allocator,
                    memcpy_tradeoff,
                    major_version,
                    minor_version)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR1_ENTER);
  this->char_translator_ = char_translator;
  this->wchar_translator_ = wchar_translator;
}

TAO_OutputCDR::TAO_OutputCDR (char *data,
                              size_t size,
                              int byte_order,
                              ACE_Allocator *buffer_allocator,
                              ACE_Allocator *data_block_allocator,
                              ACE_Allocator* message_block_allocator,
                              size_t memcpy_tradeoff,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version,
                              ACE_Char_Codeset_Translator *char_translator,
                              ACE_WChar_Codeset_Translator *wchar_translator)
  :  ACE_OutputCDR (data,
                    size,
                    byte_order,
                    buffer_allocator,
                    data_block_allocator,
                    message_block_allocator,
                    memcpy_tradeoff,
                    major_version,
                    minor_version)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR2_ENTER);
  this->char_translator_ = char_translator;
  this->wchar_translator_ = wchar_translator;
}

TAO_OutputCDR::TAO_OutputCDR (ACE_Message_Block *data,
                              int byte_order,
                              size_t memcpy_tradeoff,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version,
                              ACE_Char_Codeset_Translator *char_translator,
                              ACE_WChar_Codeset_Translator *wchar_translator)
  :  ACE_OutputCDR (data,
                    byte_order,
                    memcpy_tradeoff,
                    major_version,
                    minor_version)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR3_ENTER);
  this->char_translator_ = char_translator;
  this->wchar_translator_ = wchar_translator;
}

// ****************************************************************

TAO_InputCDR::TAO_InputCDR (const TAO_OutputCDR& rhs,
                            ACE_Allocator* buffer_allocator,
                            ACE_Allocator* data_block_allocator,
                            ACE_Allocator* message_block_allocator,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (rhs,
                  buffer_allocator
                  ? buffer_allocator
                  : (orb_core ? orb_core->output_cdr_buffer_allocator () : 0),
                  data_block_allocator
                  ? data_block_allocator
                  : (orb_core ? orb_core->output_cdr_dblock_allocator () :
                     0),
                  message_block_allocator
                  ? message_block_allocator
                  : (orb_core ?
                     orb_core->output_cdr_msgblock_allocator () : 0)),
  orb_core_ (orb_core)
{
  this->init_translators ();
}

void
TAO_InputCDR::init_translators (void)
{
  if (this->orb_core_ != 0)
    {
      this->char_translator_ = this->orb_core_->from_iso8859 ();
      this->wchar_translator_ = this->orb_core_->from_unicode ();
    }
}
