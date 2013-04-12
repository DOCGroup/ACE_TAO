// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/PortableGroup/UIPMC_Transport_Recv_Packet.h"
#include "orbsvcs/PortableGroup/UIPMC_Mcast_Transport.h"

#include "ace/OS_NS_sys_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_PG
{

  UIPMC_Recv_Packet_Cleanup_Guard::UIPMC_Recv_Packet_Cleanup_Guard (
    TAO_UIPMC_Mcast_Transport *transport
  )
    : transport_ (transport)
  {
  }

  UIPMC_Recv_Packet_Cleanup_Guard::~UIPMC_Recv_Packet_Cleanup_Guard (void)
  {
    // Cleanup only expired packets.
    this->transport_->cleanup_packets (true);
  }

  UIPMC_Recv_Packet::UIPMC_Recv_Packet (void)
    : last_fragment_id_ (0)
    , data_length_ (0)
    , started_ (ACE_OS::gettimeofday ())
  {
  }

  UIPMC_Recv_Packet::~UIPMC_Recv_Packet (void)
  {
    for (Fragments_Map::iterator iter = this->fragments_.begin ();
         iter != this->fragments_.end ();
         ++iter)
      {
        delete [] (*iter).item ().buf;
      }
  }

  int
  UIPMC_Recv_Packet::add_fragment (char *data,
                                   CORBA::UShort len,
                                   CORBA::ULong id,
                                   bool is_last)
  {
    Fragment new_data;
    ACE_NEW_RETURN (new_data.buf,
                    char[len],
                    -1);
    ACE_OS::memcpy (new_data.buf, data, len);
    new_data.len = len;

    if (is_last)
      this->last_fragment_id_ = id;

    this->data_length_ += len;

    if (TAO_debug_level >= 10)
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - TAO_PG::UIPMC_Recv_Packet::")
                  ACE_TEXT ("add_fragment, adding fragment %d with %d out ")
                  ACE_TEXT ("of %d bytes\n"),
                  id,
                  len,
                  this->data_length_));

    if (this->fragments_.bind (id, new_data) != 0)
      {
        // We've failed to add a new fragment. It's an error no matter
        // what was the reason. Mark the packet as expired.
        this->started_ = ACE_Time_Value::zero;
        delete [] new_data.buf;
        return -1;
      }

    // We haven't encountered yet the last fragment.
    if (!is_last && this->last_fragment_id_ == 0)
      return 0;

    // We haven't encountered yet all the fragments but the last one is
    // already in.
    if (this->last_fragment_id_ + 1 != this->fragments_.current_size ())
      return 0;

    // Since fragments are enumerated from 0 to last_fragment_id_ this
    // is the heaviest but the most reliable check for packet completeness.
    for (CORBA::ULong id = 0; id <= this->last_fragment_id_; ++id)
      {
        if (this->fragments_.find (id) == -1)
          {
            // Mark the packet as if it timedout.
            this->started_ = ACE_Time_Value::zero;
            return 0;
          }
      }

    return 1;
  }

  ACE_Time_Value const &
  UIPMC_Recv_Packet::started (void) const
  {
    return this->started_;
  }

  CORBA::ULong
  UIPMC_Recv_Packet::data_length (void) const
  {
    return this->data_length_;
  }

  void
  UIPMC_Recv_Packet::copy_data (char *buf) const
  {
    for (CORBA::ULong id = 0; id <= this->last_fragment_id_; ++id)
      {
        Fragment f = { 0, 0 };
        this->fragments_.find (id, f);

        ACE_OS::memcpy (buf, f.buf, f.len);
        buf += f.len;
      }
  }

} // namespace TAO_PG

TAO_END_VERSIONED_NAMESPACE_DECL
