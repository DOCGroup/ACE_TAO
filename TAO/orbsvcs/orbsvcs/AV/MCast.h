// -*- C++ -*-

//=============================================================================
/**
 *  @file   MCast.h
 *
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_AV_MCAST_H
#define TAO_AV_MCAST_H
#include /**/ "ace/pre.h"

#include "orbsvcs/AV/FlowSpec_Entry.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_AV_UDP_MCast_Flow_Handler;

/**
 * @class TAO_AV_UDP_MCast_Transport
 * @brief A transport abstraction for Multicast dgram sockets.
 *        Uses the ACE_SOCK_Dgram_Mcast to send data.
 */
class TAO_AV_UDP_MCast_Transport
  :public TAO_AV_Transport
{
public:
  TAO_AV_UDP_MCast_Transport (void);

  TAO_AV_UDP_MCast_Transport (TAO_AV_UDP_MCast_Flow_Handler *handler);

  ~TAO_AV_UDP_MCast_Transport (void) override;

  int open (ACE_Addr *address) override;

  int close (void) override;

  int mtu (void) override;

  /// Write the complete Message_Block chain to the connection.
  ACE_Addr *get_peer_addr (void) override;
  ACE_Addr *get_local_addr (void) override;
  ssize_t send (const ACE_Message_Block *mblk,
                        ACE_Time_Value *s = 0) override;

  /// Write the contents of the buffer of length len to the connection.
  ssize_t send (const char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) override;

  /// Write the contents of iovcnt iovec's to the connection.
  ssize_t send (const iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) override;

  /// Read len bytes from into buf.
  ssize_t recv (char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) override;

  /// Read len bytes from into buf using flags.
  ssize_t recv (char *buf,
                        size_t len,
                        int flags,
                        ACE_Time_Value *s = 0) override;

  ///  Read received data into the iovec buffers.
  ssize_t recv (iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) override;
protected:
  TAO_AV_UDP_MCast_Flow_Handler *handler_;
  ACE_INET_Addr peer_addr_;
  ACE_INET_Addr local_addr_;
};

/**
 * @class TAO_AV_UDP_MCast_Flow_Handler
 * @brief Flow Handler for data sent over multicast sockets.
 */
class TAO_AV_UDP_MCast_Flow_Handler
  :public virtual TAO_AV_Flow_Handler,
   public virtual ACE_Event_Handler
{
public:
  TAO_AV_UDP_MCast_Flow_Handler (void);
  ~TAO_AV_UDP_MCast_Flow_Handler (void) override;
  ACE_HANDLE get_handle () const override;
  int handle_input (ACE_HANDLE fd) override;
  int handle_timeout (const ACE_Time_Value &tv, const void *arg = 0) override;
  ACE_SOCK_Dgram_Mcast *get_mcast_socket () const;
  ACE_Event_Handler* event_handler (void) override{ return this; }
  void set_peer_addr (ACE_INET_Addr *peer_addr);
protected:
  ACE_INET_Addr *peer_addr_;
  ACE_SOCK_Dgram_Mcast *dgram_mcast_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "orbsvcs/AV/MCast.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_AV_MCAST_H */
