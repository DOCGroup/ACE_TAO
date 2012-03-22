// $Id$


#include "tao/On_Demand_Fragmentation_Strategy.h"

#include "tao/Transport.h"
#include "tao/CDR.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/debug.h"

TAO_On_Demand_Fragmentation_Strategy::TAO_On_Demand_Fragmentation_Strategy (
  TAO_Transport * transport,
  CORBA::ULong max_message_size)
  : transport_ (transport)
  , max_message_size_ (max_message_size)
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
  if (this->transport_ == 0)
    return 0;  // No transport.  Can't fragment.

  TAO_GIOP_Message_Version giop_version;

  cdr.get_version (giop_version);

  // GIOP fragments are supported in GIOP 1.1 and better, but TAO only
  // supports them in 1.2 or better since GIOP 1.1 fragments do not
  // have a fragment message header.
  if (giop_version.major == 1 && giop_version.minor < 2)
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

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - On_Demand_Fragmentation_Strategy::fragment, "
                    "sending fragment of size %d\n",
                    cdr.total_length ()));

      // Send the current CDR stream contents through the transport,
      // making sure to switch on the the GIOP flags "more fragments"
      // bit.
      if (this->transport_->send_message (cdr,
                                          cdr.stub (),
                                          0,
                                          cdr.message_semantics (),
                                          cdr.timeout ()) == -1

          // Now generate a fragment header.
          || this->transport_->messaging_object ()->generate_fragment_header (
               cdr,
               cdr.request_id ()) != 0)
        return -1;
    }

  return 0;
}
