// -*- C++ -*-

//=============================================================================
/**
 *  @file     UIPMC_Transport_Recv_Packet.h
 *
 *  $Id$
 *
 *  @author Vladimir Zykov <vz@prismtech.com>
 */
//=============================================================================

#ifndef TAO_UIPMC_TRANSPORT_RECV_PACKET_H
#define TAO_UIPMC_TRANSPORT_RECV_PACKET_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/Versioned_Namespace.h"

#include "tao/corba.h"

class TAO_UIPMC_Mcast_Transport;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_PG
{

  /**
   * @class UIPMC_Recv_Packet_Cleanup_Guard
   *
   * @brief A guard that will cleanup broken/expired packets.
   */
  class UIPMC_Recv_Packet_Cleanup_Guard
  {
  public:
    UIPMC_Recv_Packet_Cleanup_Guard (TAO_UIPMC_Mcast_Transport *transport);

    ~UIPMC_Recv_Packet_Cleanup_Guard (void);

  private:
    TAO_UIPMC_Mcast_Transport *transport_;
  };

  /**
   * @class UIPMC_Recv_Packet
   *
   * @brief A MIOP packet for receiving.
   */
  class UIPMC_Recv_Packet
  {
  public:
    /// Constructs a new recv packet.
    UIPMC_Recv_Packet (void);

    ~UIPMC_Recv_Packet (void);

    /// Adds a new fragment to the packet and if it fails marks the packet
    /// as broken.
    /// Returns 1 if all fragments are in AND if there are no fragments
    /// with unexpected IDs. In case unexpected IDs are encountered the
    /// packet is marked as broken.
    int add_fragment (char *data, CORBA::UShort len,
                      CORBA::ULong id, bool is_last);

    /// Returns the time when the first fragment was received or
    /// ACE_Time_Value::zero if the whole packet was not able to
    /// reconstruct for some reason.
    ACE_Time_Value const &started (void) const;

    CORBA::ULong data_length (void) const;

    /// Copies fragments to buf. Caller ensures that the buf is big enough
    /// for all fragments.
    void copy_data (char *buf) const;

  private:
    /// The id of the last fragment.
    CORBA::ULong last_fragment_id_;

    /// The length of the data stored in all fragments.
    CORBA::ULong data_length_;

    /// The time when the packet will expire.
    mutable ACE_Time_Value started_;

    /// Fragments.
    struct Fragment
    {
      char *buf;
      CORBA::UShort len;
    };

    typedef ACE_Hash_Map_Manager<CORBA::ULong,
                                 Fragment,
                                 ACE_SYNCH_NULL_MUTEX> Fragments_Map;
    Fragments_Map fragments_;
  };

} // namespace TAO_PG

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // TAO_UIPMC_TRANSPORT_RECV_PACKET_H
