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

#include "tao/corba.h"
#include "ace/Timeprobe.h"

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

// ****************************************************************
// ****************************************************************
// ****************************************************************

TAO_OutputCDR::TAO_OutputCDR (size_t size,
                              int byte_order,
			                        ACE_Allocator *buffer_allocator,
			                        ACE_Allocator *data_block_allocator,
                              size_t memcpy_tradeoff)
  :  ACE_OutputCDR (size,
        byte_order,
        buffer_allocator 
          ? buffer_allocator 
          : TAO_ORB_Core_instance ()->output_cdr_buffer_allocator (),
        data_block_allocator
          ? data_block_allocator
          : TAO_ORB_Core_instance ()->output_cdr_dblock_allocator (),
        memcpy_tradeoff
          ? memcpy_tradeoff
          : TAO_ORB_Core_instance ()->orb_params ()->cdr_memcpy_tradeoff ())
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR1_ENTER);
}

TAO_OutputCDR::TAO_OutputCDR (char *data, 
                              size_t size,
                              int byte_order,
			                        ACE_Allocator *buffer_allocator,
			                        ACE_Allocator *data_block_allocator,
                              size_t memcpy_tradeoff)
  :  ACE_OutputCDR (data,
        size,
        byte_order,
        buffer_allocator 
          ? buffer_allocator 
          : TAO_ORB_Core_instance ()->output_cdr_buffer_allocator (),
        data_block_allocator
          ? data_block_allocator
          : TAO_ORB_Core_instance ()->output_cdr_dblock_allocator (),
        memcpy_tradeoff
          ? memcpy_tradeoff
          : TAO_ORB_Core_instance ()->orb_params ()->cdr_memcpy_tradeoff ())
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR2_ENTER);
}

TAO_OutputCDR::TAO_OutputCDR (ACE_Message_Block *data,
                              int byte_order,
                              size_t memcpy_tradeoff)
  :  ACE_OutputCDR (data,
        byte_order,
        memcpy_tradeoff
          ? memcpy_tradeoff
          : TAO_ORB_Core_instance ()->orb_params ()->cdr_memcpy_tradeoff ())
{ 
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR3_ENTER);
}

TAO_OutputCDR::~TAO_OutputCDR (void)
{
}

// ****************************************************************

TAO_InputCDR::TAO_InputCDR (const char *buf, 
                            size_t bufsiz,
                            int byte_order)
  : ACE_InputCDR (buf,
                  bufsiz,
                  byte_order)
{
}

TAO_InputCDR::TAO_InputCDR (size_t bufsiz,
                            int byte_order)
  : ACE_InputCDR (bufsiz,
                  byte_order)
{
}

TAO_InputCDR::TAO_InputCDR (ACE_Message_Block *data,
                            int byte_order)
  : ACE_InputCDR (data,
                  byte_order)
{
}

TAO_InputCDR::TAO_InputCDR (ACE_Data_Block *data,
                            int byte_order)
  : ACE_InputCDR (data,
                  byte_order)
{
}

TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size,
                            CDR::Long offset)
  : ACE_InputCDR (rhs,
                  size,
                  offset)
{
}

TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size)
  : ACE_InputCDR (rhs,
                  size)
{
}

TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs)
  : ACE_InputCDR (rhs)
{
}

TAO_InputCDR::TAO_InputCDR (const TAO_OutputCDR& rhs,
                            ACE_Allocator* buffer_allocator,
                            ACE_Allocator* data_block_allocator)
  : ACE_InputCDR (rhs,
        buffer_allocator
          ? buffer_allocator 
          : TAO_ORB_Core_instance ()->output_cdr_buffer_allocator (),
        data_block_allocator
          ? data_block_allocator
          : TAO_ORB_Core_instance ()->output_cdr_dblock_allocator ())
{
}

TAO_InputCDR::~TAO_InputCDR (void)
{
}

