// $Id$


#include "tao/True_Fragmentation_Strategy.h"

#include "tao/Transport.h"
#include "tao/CDR.h"


TAO_True_Fragmentation_Strategy::TAO_True_Fragmentation_Strategy (
  TAO_Transport * transport)
  : transport_ (transport)
  , max_message_size_ (0)
{
}

TAO_True_Fragmentation_Strategy::~TAO_True_Fragmentation_Strategy (void)
{
}

void
TAO_True_Fragmentation_Strategy::fragment (TAO_OutputCDR & cdr,
                                           ACE_CDR::ULong pending_alignment,
                                           ACE_CDR::ULong pending_length)
{
  // Determine increase in CDR stream length if pending data is
  // marshaled, taking into account the alignment for the given data
  // type.
  ACE_CDR::ULong const total_pending_length =
    ACE_align_binary (this->total_length (), pending_alignment)
    + pending_length;

  // Fragmented GIOP messages must always be aligned on an 8-byte
  // boundary.  Padding will be added if necessary.
  ACE_CDR::ULong const aligned_length =
    ACE_align_binary (total_pending_length % ACE_CDR::MAX_ALIGNMENT);

  // this->max_message_size_ must be >= 16 bytes, i.e.:
  //   12 for GIOP protocol header
  //  + 4 for GIOP fragment header
  //  + 4 for padding
  // since fragments must be aligned on an 8 byte boundary.
  if (aligned_length > this->max_message_size_)
    // DO FRAGMENTATION
}
