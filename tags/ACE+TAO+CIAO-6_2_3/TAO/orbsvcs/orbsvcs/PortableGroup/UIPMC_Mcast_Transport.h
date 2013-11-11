// -*- C++ -*-

//=============================================================================
/**
 *  @file     UIPMC_Mcast_Transport.h
 *
 *  $Id$
 *
 *  @author Vladimir Zykov <vz@prismtech.com>
 */
//=============================================================================

#ifndef TAO_UIPMC_MCAST_TRANSPORT_H
#define TAO_UIPMC_MCAST_TRANSPORT_H
#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Transport.h"

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Refcountable_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decls.
class TAO_ORB_Core;
class TAO_UIPMC_Mcast_Connection_Handler;
namespace TAO_PG
{
  class UIPMC_Recv_Packet_Cleanup_Guard;
  class UIPMC_Recv_Packet;
}

/**
 * @class TAO_UIPMC_Mcast_Transport
 *
 * @brief Specialization of the base TAO_Transport class to handle the
 *  server side MIOP protocol.
 */
class TAO_PortableGroup_Export TAO_UIPMC_Mcast_Transport : public TAO_Transport
{
  // This is neccessary on some old compilers such as Studio 9 for SunOS
  // to let UIPMC_Recv_Packet_Cleanup_Guard access private cleanup_packets().
  friend class TAO_PG::UIPMC_Recv_Packet_Cleanup_Guard;

public:
  typedef ACE_Hash_Map_Manager<u_long,
                               TAO_PG::UIPMC_Recv_Packet *,
                               ACE_SYNCH_NULL_MUTEX> Packets_Map;

  /// Constructor.
  TAO_UIPMC_Mcast_Transport (TAO_UIPMC_Mcast_Connection_Handler *handler,
                             TAO_ORB_Core *orb_core);

  /// Default destructor.
  ~TAO_UIPMC_Mcast_Transport (void);

  /// Look for the documentation in Transport.h.
  virtual int handle_input (TAO_Resume_Handle &rh,
                            ACE_Time_Value *max_wait_time = 0);

protected:
  /** @name Overridden Template Methods
   *
   * These are implementations of template methods declared by TAO_Transport.
   */
  //@{
  virtual ACE_Event_Handler *event_handler_i (void);
  virtual TAO_Connection_Handler *connection_handler_i (void);

  /// Write the complete Message_Block chain to the connection.
  /// Shouldn't ever be called on the server side.
  virtual ssize_t send (iovec *,
                        int,
                        size_t &,
                        ACE_Time_Value const *);

  /// Shouldn't ever be called. We use recv_all() with different semantics.
  virtual ssize_t recv (char *,
                        size_t,
                        ACE_Time_Value const *);

  virtual int register_handler (void);

public:
  /// @@TODO: These methods IMHO should have more meaningful
  /// names. The names seem to indicate nothing.
  /// Shouldn't ever be called on the server side.
  virtual int send_request (TAO_Stub *,
                            TAO_ORB_Core *,
                            TAO_OutputCDR &,
                            TAO_Message_Semantics,
                            ACE_Time_Value *);

  /// Shouldn't ever be called on the server side.
  virtual int send_message (TAO_OutputCDR &,
                            TAO_Stub * = 0,
                            TAO_ServerRequest * = 0,
                            TAO_Message_Semantics = TAO_Message_Semantics (),
                            ACE_Time_Value * = 0);
  //@}

private:
  /// Receive a UDP message and extract all necessary info from the MIOP
  /// header. If everything is fine return a pointer to the first byte of
  /// the non-MIOP data.
  char *recv_packet (char *buf, size_t len,
                     ACE_INET_Addr &from_addr,
                     CORBA::UShort &packet_length,
                     CORBA::ULong &packet_number,
                     bool &stop_packet,
                     u_long &id_hash) const;

  /// Return the next complete MIOP packet, possiably dequeueing
  /// as many as are available first from the socket.
  TAO_PG::UIPMC_Recv_Packet *recv_all (TAO_Resume_Handle &rh);

  /// Cleanup either all packets or expired only depending the
  /// expired_only flag.
  void cleanup_packets (bool expired_only);

private:
  /// The connection service handler used for accessing lower layer
  /// communication protocols.
  TAO_UIPMC_Mcast_Connection_Handler *connection_handler_;

  /// Incomplete packets.
  Packets_Map incomplete_;

  /// A lock for ensuring that only one thread is doing recv.
  TAO_SYNCH_MUTEX recv_lock_;

  /// Complete packets.
  typedef ACE_Unbounded_Queue<TAO_PG::UIPMC_Recv_Packet *> Packets_Queue;
  Packets_Queue complete_;

  /// A lock for access synchronization to complete queue.
  TAO_SYNCH_MUTEX complete_lock_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif  /* TAO_UIPMC_MCAST_TRANSPORT_H */
