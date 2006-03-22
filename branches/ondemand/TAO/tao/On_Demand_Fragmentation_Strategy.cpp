// $Id$


#include "tao/On_Demand_Fragmentation_Strategy.h"

#include "tao/Transport.h"
#include "tao/CDR.h"
#include "tao/Pluggable_Messaging.h"


TAO_On_Demand_Fragmentation_Strategy::TAO_On_Demand_Fragmentation_Strategy (
  TAO_Transport * transport,
  CORBA::ULong max_message_size)
  : transport_ (transport)
  , max_message_size_ (0)
{
}

TAO_On_Demand_Fragmentation_Strategy::~TAO_On_Demand_Fragmentation_Strategy (
  void)
{
}

int
TAO_On_Demand_Fragmentation_Strategy::fragment (
  TAO_OutputCDR & cdr,
  ACE_CDR::ULong pending_alignment,
  ACE_CDR::ULong pending_length)
{
  CORBA::Octet major = 0;
  CORBA::Octet minor = 0;

  (void) cdr.get_version (major, minor);

  // GIOP fragments are supported in GIOP 1.1 and better, but TAO only
  // supports them in 1.2 or better since GIOP 1.1 fragments do not
  // have a fragment message header.
  if (major == 1 && minor < 2)
    return -1;

  // Determine increase in CDR stream length if pending data is
  // marshaled, taking into account the alignment for the given data
  // type.
  ACE_CDR::ULong const total_pending_length =
    ACE_align_binary (cdr.total_length (), pending_alignment)
    + pending_length;

  // Except for the last fragment, fragmented GIOP messages must
  // always be aligned on an 8-byte boundary.  Padding will be added
  // if necessary.
  ACE_CDR::ULong const aligned_length =
    ACE_align_binary (total_pending_length, ACE_CDR::MAX_ALIGNMENT);

  // this->max_message_size_ must be >= 24 bytes, i.e.:
  //   12 for GIOP protocol header
  //  + 4 for GIOP fragment header
  //  + 8 for payload (including padding)
  // since fragments must be aligned on an 8 byte boundary.
  if (aligned_length > this->max_message_size_)
    {
      // Pad the outgoing fragment if necessary.
      if (cdr.align_write_ptr (ACE_CDR::MAX_ALIGNMENT) != 0)
        return -1;

      // More fragments to come.
      cdr.more_fragments (true);

      // Send the current CDR stream contents through the transport,
      // making sure to switch on the the GIOP flags "more fragments"
      // bit.
      return this->send_message_callback_.send (cdr);

      // Now generate a fragment header.
      this->transport_->messaging_object ()->generate_fragment_header (
        this->request_id_);
    }

  return 0;
}
