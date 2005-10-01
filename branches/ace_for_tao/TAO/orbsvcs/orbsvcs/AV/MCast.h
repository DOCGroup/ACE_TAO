/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   MCast.h
 *
 *  $Id$
 *
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_AV_MCAST_H
#define TAO_AV_MCAST_H
#include /**/ "ace/pre.h"

#include "FlowSpec_Entry.h"
#include "Protocol_Factory.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"

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

  virtual ~TAO_AV_UDP_MCast_Transport (void);

  virtual int open (ACE_Addr *address);

  virtual int close (void);

  virtual int mtu (void);

  /// Write the complete Message_Block chain to the connection.
  virtual ACE_Addr *get_peer_addr (void);
  virtual ACE_Addr *get_local_addr (void);
  virtual ssize_t send (const ACE_Message_Block *mblk,
                        ACE_Time_Value *s = 0);

  /// Write the contents of the buffer of length len to the connection.
  virtual ssize_t send (const char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0);

  /// Write the contents of iovcnt iovec's to the connection.
  virtual ssize_t send (const iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0);

  /// Read len bytes from into buf.
  virtual ssize_t recv (char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0);

  /// Read len bytes from into buf using flags.
  virtual ssize_t recv (char *buf,
                        size_t len,
                        int flags,
                        ACE_Time_Value *s = 0);

  ///  Read received data into the iovec buffers.
  virtual ssize_t recv (iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0);
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
  /// Ctor
  /// Dtor
  TAO_AV_UDP_MCast_Flow_Handler (void);
  virtual ~TAO_AV_UDP_MCast_Flow_Handler (void);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg = 0);
  ACE_SOCK_Dgram_Mcast *get_mcast_socket (void) const;
  virtual ACE_Event_Handler* event_handler (void){ return this; }
  void set_peer_addr (ACE_INET_Addr *peer_addr);
protected:
  ACE_INET_Addr *peer_addr_;
  ACE_SOCK_Dgram_Mcast *dgram_mcast_;
};

#if defined(__ACE_INLINE__)
#include "MCast.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_AV_MCAST_H */
