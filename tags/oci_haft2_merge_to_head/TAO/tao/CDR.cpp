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
// the fact that (for example) CORBA::Long is always four bytes long
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
                              ACE_CDR::Octet minor_version)
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
}

TAO_OutputCDR::TAO_OutputCDR (char *data,
                              size_t size,
                              int byte_order,
                              ACE_Allocator *buffer_allocator,
                              ACE_Allocator *data_block_allocator,
                              ACE_Allocator* message_block_allocator,
                              size_t memcpy_tradeoff,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version)
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
}

TAO_OutputCDR::TAO_OutputCDR (ACE_Message_Block *data,
                              int byte_order,
                              size_t memcpy_tradeoff,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version)
  :  ACE_OutputCDR (data,
                    byte_order,
                    memcpy_tradeoff,
                    major_version,
                    minor_version)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR3_ENTER);
}


void
TAO_OutputCDR::throw_stub_exception (int error_num ACE_ENV_ARG_DECL)
{
  switch (error_num)
    {
    case 0 :
      break;
    case EINVAL : // wchar from a GIOP 1.0
      ACE_THROW (CORBA::MARSHAL (CORBA::OMGVMCID | 5, CORBA::COMPLETED_NO));
      ACE_NOTREACHED(break);
    case ERANGE : // untranslatable character
      ACE_THROW (CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 1, CORBA::COMPLETED_NO));
      ACE_NOTREACHED(break);
    case EACCES : // wchar but no codeset
      ACE_THROW(CORBA::INV_OBJREF (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO));
      ACE_NOTREACHED(break);
    default :
      ACE_THROW (CORBA::MARSHAL ());
    }
}

void
TAO_OutputCDR::throw_skel_exception (int error_num ACE_ENV_ARG_DECL)
{
  switch (error_num)
    {
    case 0 :
      break;
    case EINVAL : // wchar from a GIOP 1.0
      ACE_THROW (CORBA::MARSHAL(CORBA::OMGVMCID | 5, CORBA::COMPLETED_YES));
      ACE_NOTREACHED(break);

    case EACCES : // wchar but no codeset
      ACE_THROW (CORBA::BAD_PARAM(CORBA::OMGVMCID | 23, CORBA::COMPLETED_YES));
      ACE_NOTREACHED(break);

    case ERANGE : // untranslatable character
      ACE_THROW (CORBA::DATA_CONVERSION(CORBA::OMGVMCID | 1, CORBA::COMPLETED_YES));
      ACE_NOTREACHED(break);

    default :
      ACE_THROW (CORBA::MARSHAL(TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));

    }
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
}


void
TAO_InputCDR::throw_stub_exception (int error_num ACE_ENV_ARG_DECL)
{
  switch (error_num)
    {
    case 0 :
      break;
    case EINVAL : // wchar from a GIOP 1.0
      ACE_THROW (CORBA::MARSHAL (CORBA::OMGVMCID | 6, CORBA::COMPLETED_YES));
      ACE_NOTREACHED(break);
    case ERANGE : // untranslatable character
      ACE_THROW (CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 1, CORBA::COMPLETED_YES));
      ACE_NOTREACHED(break);
    case EACCES : // wchar but no codeset
      ACE_THROW (CORBA::INV_OBJREF (CORBA::OMGVMCID | 2, CORBA::COMPLETED_YES));
      ACE_NOTREACHED(break);
    default :
      ACE_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
    }
}

void
TAO_InputCDR::throw_skel_exception (int error_num ACE_ENV_ARG_DECL)
{
  switch (error_num)
    {
    case 0 :
      break;
    case EINVAL : // wchar from a GIOP 1.0
      ACE_THROW (CORBA::MARSHAL(CORBA::OMGVMCID | 5, CORBA::COMPLETED_NO));
      ACE_NOTREACHED(break);

    case EACCES : // wchar but no codeset
      ACE_THROW (CORBA::BAD_PARAM(CORBA::OMGVMCID | 23, CORBA::COMPLETED_NO));
      ACE_NOTREACHED(break);

    case ERANGE : // untranslatable character
      ACE_THROW (CORBA::DATA_CONVERSION(CORBA::OMGVMCID | 1, CORBA::COMPLETED_NO));
      ACE_NOTREACHED(break);

    default :
      ACE_THROW (CORBA::MARSHAL());

    }
}
