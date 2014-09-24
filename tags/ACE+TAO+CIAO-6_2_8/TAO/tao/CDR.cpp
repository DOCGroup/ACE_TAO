// $Id$

#include "tao/CDR.h"
#include "tao/Timeprobe.h"
#include "tao/ORB_Core.h"
#include "tao/SystemException.h"
#include "tao/GIOP_Fragmentation_Strategy.h"

#if !defined (__ACE_INLINE__)
# include "tao/CDR.inl"
#endif /* ! __ACE_INLINE__ */

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_CDR_Timeprobe_Description[] =
{
  "OutputCDR::ctor[1] - enter",
  "OutputCDR::ctor[1] - leave",
  "OutputCDR::ctor[2] - enter",
  "OutputCDR::ctor[2] - leave",
  "OutputCDR::ctor[3] - enter",
  "OutputCDR::ctor[3] - leave",
  "OutputCDR::ctor[4] - enter",
  "OutputCDR::ctor[4] - leave",
  "OutputCDR::ctor[5] - enter",
  "OutputCDR::ctor[5] - leave"
};

enum
{
  TAO_OUTPUT_CDR_CTOR1_ENTER = 2000,
  TAO_OUTPUT_CDR_CTOR1_LEAVE,
  TAO_OUTPUT_CDR_CTOR2_ENTER,
  TAO_OUTPUT_CDR_CTOR2_LEAVE,
  TAO_OUTPUT_CDR_CTOR3_ENTER,
  TAO_OUTPUT_CDR_CTOR3_LEAVE,
  TAO_OUTPUT_CDR_CTOR4_ENTER,
  TAO_OUTPUT_CDR_CTOR4_LEAVE,
  TAO_OUTPUT_CDR_CTOR5_ENTER,
  TAO_OUTPUT_CDR_CTOR5_LEAVE
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_CDR_Timeprobe_Description,
                                  TAO_OUTPUT_CDR_CTOR1_ENTER);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_OutputCDR::TAO_OutputCDR (size_t size,
                              int byte_order,
                              ACE_Allocator *buffer_allocator,
                              ACE_Allocator *data_block_allocator,
                              ACE_Allocator* message_block_allocator,
                              size_t memcpy_tradeoff,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version)
  : ACE_OutputCDR (size,
                   byte_order,
                   buffer_allocator,
                   data_block_allocator,
                   message_block_allocator,
                   memcpy_tradeoff,
                   major_version,
                   minor_version)
  , fragmentation_strategy_ (0)
  , more_fragments_ (false)
  , request_id_ (0)
  , stub_ (0)
  , message_semantics_ (TAO_Message_Semantics::TAO_TWOWAY_REQUEST)
  , timeout_ (0)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR1_ENTER);

#if defined (TAO_ZERO_TAO_OUTPUTCDR_ALLOCATED_BUFFERS)
  // Zero out the buffer if we allocated the buffer.
  if (size == 0)
    (void) ACE_OS::memset (this->current()->wr_ptr(),
                           0,
                           this->current()->space());
#endif /* TAO_ZERO_TAO_OUTPUTCDR_ALLOCATED_BUFFERS */
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
  : ACE_OutputCDR (data,
                   size,
                   byte_order,
                   buffer_allocator,
                   data_block_allocator,
                   message_block_allocator,
                   memcpy_tradeoff,
                   major_version,
                   minor_version)
  , fragmentation_strategy_ (0)
  , more_fragments_ (false)
  , request_id_ (0)
  , stub_ (0)
  , message_semantics_ (TAO_Message_Semantics::TAO_TWOWAY_REQUEST)
  , timeout_ (0)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR2_ENTER);
}

TAO_OutputCDR::TAO_OutputCDR (char *data,
                              size_t size,
                              int byte_order,
                              ACE_Allocator* buffer_allocator,
                              ACE_Allocator* data_block_allocator,
                              ACE_Allocator* message_block_allocator,
                              size_t memcpy_tradeoff,
                              TAO_GIOP_Fragmentation_Strategy * fs,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version)
  : ACE_OutputCDR (data,
                   size,
                   byte_order,
                   buffer_allocator,
                   data_block_allocator,
                   message_block_allocator,
                   memcpy_tradeoff,
                   major_version,
                   minor_version)
  , fragmentation_strategy_ (fs)
  , more_fragments_ (false)
  , request_id_ (0)
  , stub_ (0)
  , message_semantics_ (TAO_Message_Semantics::TAO_TWOWAY_REQUEST)
  , timeout_ (0)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR3_ENTER);
}

TAO_OutputCDR::TAO_OutputCDR (ACE_Message_Block *data,
                              int byte_order,
                              size_t memcpy_tradeoff,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version)
  : ACE_OutputCDR (data,
                   byte_order,
                   memcpy_tradeoff,
                   major_version,
                   minor_version)
  , fragmentation_strategy_ (0)
  , more_fragments_ (false)
  , request_id_ (0)
  , stub_ (0)
  , message_semantics_ (TAO_Message_Semantics::TAO_TWOWAY_REQUEST)
  , timeout_ (0)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR4_ENTER);
}

TAO_OutputCDR::TAO_OutputCDR (ACE_Data_Block *data_block,
                              int byte_order,
                              ACE_Allocator* message_block_allocator,
                              size_t memcpy_tradeoff,
                              TAO_GIOP_Fragmentation_Strategy * fs,
                              ACE_CDR::Octet major_version,
                              ACE_CDR::Octet minor_version)
  :  ACE_OutputCDR (data_block,
                    byte_order,
                    message_block_allocator,
                    memcpy_tradeoff,
                    major_version,
                    minor_version)
  , fragmentation_strategy_ (fs)
  , more_fragments_ (false)
  , request_id_ (0)
  , stub_ (0)
  , message_semantics_ (TAO_Message_Semantics::TAO_TWOWAY_REQUEST)
  , timeout_ (0)
{
  ACE_FUNCTION_TIMEPROBE (TAO_OUTPUT_CDR_CTOR5_ENTER);
}

void
TAO_OutputCDR::throw_stub_exception (int error_num)
{
  switch (error_num)
    {
    case EINVAL : // wchar from a GIOP 1.0
      throw ::CORBA::MARSHAL (CORBA::OMGVMCID | 5, CORBA::COMPLETED_NO);
      ACE_NOTREACHED (break);
#if (ERANGE != EINVAL)
    case ERANGE : // untranslatable character
      throw ::CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 1,
                                         CORBA::COMPLETED_NO);
      ACE_NOTREACHED (break);
#endif
    case EACCES : // wchar but no codeset
      throw ::CORBA::INV_OBJREF (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
      ACE_NOTREACHED (break);
    default :
      throw ::CORBA::MARSHAL ();
    }
}

void
TAO_OutputCDR::throw_skel_exception (int error_num )
{
  switch (error_num)
    {
    case EINVAL : // wchar from a GIOP 1.0
      throw ::CORBA::MARSHAL (CORBA::OMGVMCID | 5, CORBA::COMPLETED_YES);
      ACE_NOTREACHED (break);

    case EACCES : // wchar but no codeset
      throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 23, CORBA::COMPLETED_YES);
      ACE_NOTREACHED (break);

#if (ERANGE != EINVAL)
    case ERANGE : // untranslatable character
      throw ::CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 1,
                                         CORBA::COMPLETED_YES);
      ACE_NOTREACHED (break);
#endif

    default :
      throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_YES);

    }
}

bool
TAO_OutputCDR::fragment_stream (ACE_CDR::ULong pending_alignment,
                                ACE_CDR::ULong pending_length)
{
  if (this->fragmentation_strategy_)
    {
      return (this->fragmentation_strategy_->fragment (*this,
                                                       pending_alignment,
                                                       pending_length) == 0);
    }

  return true;  // Success.
}



int
TAO_OutputCDR::offset (char* pos)
{
  int offset = 0;
  const ACE_Message_Block * cur_mb = this->begin ();
  char* wr_ptr = this->current ()->wr_ptr ();
  bool found = false;

  while (cur_mb != this->end ())
  {
    if (pos >= cur_mb->rd_ptr () && pos <= cur_mb->wr_ptr ())
    {
      offset += (cur_mb->wr_ptr () - pos);
      found = true;
    }
    else if (found)
    {
      offset += cur_mb->length ();
    }

    if (wr_ptr == cur_mb->wr_ptr ())
      break;

    cur_mb = cur_mb->cont();
  }

  if (!found || wr_ptr != cur_mb->wr_ptr ())
  {
    throw ::CORBA::INTERNAL ();
  }

  return offset;
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
TAO_InputCDR::throw_stub_exception (int error_num )
{
  switch (error_num)
    {
    case EINVAL : // wchar from a GIOP 1.0
      throw ::CORBA::MARSHAL (CORBA::OMGVMCID | 6, CORBA::COMPLETED_YES);
      ACE_NOTREACHED(break);
#if (ERANGE != EINVAL)
    case ERANGE : // untranslatable character
      throw ::CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 1, CORBA::COMPLETED_YES);
      ACE_NOTREACHED(break);
#endif
    case EACCES : // wchar but no codeset
      throw ::CORBA::INV_OBJREF (CORBA::OMGVMCID | 2, CORBA::COMPLETED_YES);
      ACE_NOTREACHED(break);
    default :
      throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_YES);
    }
}

void
TAO_InputCDR::throw_skel_exception (int error_num )
{
  switch (error_num)
    {
    case EINVAL : // wchar from a GIOP 1.0
      throw ::CORBA::MARSHAL(CORBA::OMGVMCID | 5, CORBA::COMPLETED_NO);
      ACE_NOTREACHED(break);

    case EACCES : // wchar but no codeset
      throw ::CORBA::BAD_PARAM(CORBA::OMGVMCID | 23, CORBA::COMPLETED_NO);
      ACE_NOTREACHED(break);

#if (ERANGE != EINVAL)
    case ERANGE : // untranslatable character
      throw ::CORBA::DATA_CONVERSION(CORBA::OMGVMCID | 1, CORBA::COMPLETED_NO);
      ACE_NOTREACHED(break);
#endif

    default :
      throw ::CORBA::MARSHAL();

    }
}

ACE_Message_Block::Message_Flags
TAO_InputCDR::clr_mb_flags( ACE_Message_Block::Message_Flags less_flags )
{
  return start_.clr_self_flags( less_flags );
}


TAO_END_VERSIONED_NAMESPACE_DECL
