// $Id$

// ============================================================================
//
// = DESCRIPTION
//     Encapsulate the I/O layer for a UDP based receiver
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_IO_UDP_H
#define ACE_RMCAST_IO_UDP_H
#include "ace/pre.h"

#include "ace/RMCast/RMCast_Module.h"
#include "ace/Sockets/SOCK_Dgram_Mcast.h"
#include "ace/Utils/Hash_Map_Manager.h"
#include "ace/Threads/Synch.h"
#include "ace/IPC/INET_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_UDP_Proxy;
class ACE_RMCast_Module_Factory;
class ACE_Time_Value;

class ACE_RMCast_Export ACE_RMCast_IO_UDP : public ACE_RMCast_Module
{
public:
  //! Constructor
  /*!
   * The <factory> argument is used to create the modules for each
   * proxy that process incoming messages. The class does *not* assume
   * ownership of <factory>, the caller owns it.  But it does assume
   * ownership of the modules returned by the factory, and it may ask
   * the factory to release them eventually.
   */
  ACE_RMCast_IO_UDP (ACE_RMCast_Module_Factory *factory);

  //! Destructor
  ~ACE_RMCast_IO_UDP (void);

  /// Open the internal socket, but only to send multicast data.
  /**
   * It is not clear to me if this is a valid configuration.  Maybe it
   * would be a problem to expose two separate, incompatible
   * interfaces (check the subscribe() method). However, the
   * alternative would be to implement almost identical class for
   * outgoing and incoming UDP I/O
   */
  int init (const ACE_INET_Addr &mcast_group,
            const ACE_Addr &local,
            int protocol_family = PF_INET,
            int protocol = 0,
            int reuse_addr = 0);

  //! Join a new multicast group
  /*!
   * Start receiving data for the <mcast_addr> multicast group.
   * Please read the documentation of ACE_SOCK_Dgram_Mcast for more
   * details.
   */
  int subscribe (const ACE_INET_Addr &mcast_addr,
                 int reuse_addr = 1,
                 const ACE_TCHAR *net_if = 0,
                 int protocol_family = PF_INET,
                 int protocol = 0);

  // The class can be used with a Reactor or using blocking I/O
  // depending on what method of the following two is called.

  //! Wait for events for the period <tv>. If <tv> is zero it blocks
  //! forever.
  int handle_events (ACE_Time_Value *tv = 0);

  //! There is data to read, read it and process it.
  int handle_input (ACE_HANDLE h);

  //! Obtain the handle for the underlying socket
  ACE_HANDLE get_handle (void) const;

  //@{
  //! Send the message to the ACE_INET_Addr argument.
  /*!
   * These methods are used in the implementation of the
   * ACE_RMCast_UDP_Proxy objects and the implementation of the
   * inherited ACE_RMCast_Module methods in this class.
   */
  int send_data (ACE_RMCast::Data &, const ACE_INET_Addr &);
  int send_poll (ACE_RMCast::Poll &, const ACE_INET_Addr &);
  int send_ack_join (ACE_RMCast::Ack_Join &, const ACE_INET_Addr &);
  int send_ack_leave (ACE_RMCast::Ack_Leave &, const ACE_INET_Addr &);
  int send_ack (ACE_RMCast::Ack &, const ACE_INET_Addr &);
  int send_join (ACE_RMCast::Join &, const ACE_INET_Addr &);
  int send_leave (ACE_RMCast::Leave &, const ACE_INET_Addr &);
  //@}

  // Please read the documentation in ACE_RMCast_Module for more details
  virtual int data (ACE_RMCast::Data &);
  virtual int poll (ACE_RMCast::Poll &);
  virtual int ack_join (ACE_RMCast::Ack_Join &);
  virtual int ack_leave (ACE_RMCast::Ack_Leave &);
  virtual int ack (ACE_RMCast::Ack &);
  virtual int join (ACE_RMCast::Join &);
  virtual int leave (ACE_RMCast::Leave &);

private:

  // This is necessary to satisfy the stupid xgcc for Lynx on Solaris
  // The compiler barfs up lisp code errors
  ACE_RMCast_UDP_Proxy *allocate_and_bind_proxy (ACE_RMCast_Module *module,
                                                 const ACE_INET_Addr &);

  //! The factory used to create the modules attached to each proxy
  ACE_RMCast_Module_Factory *factory_;

  //! The multicast group we subscribe and send to
  ACE_INET_Addr mcast_group_;

  //! The socket used to receive and send data
  ACE_SOCK_Dgram_Mcast dgram_;

  //! Use a Hash_Map to maintain the collection of proxies
  typedef ACE_Hash_Map_Manager<ACE_INET_Addr,ACE_RMCast_UDP_Proxy*,ACE_Null_Mutex> Map;
  //! The collection of proxies
  Map map_;
};

#if defined (__ACE_INLINE__)
#include "ace/RMCast/RMCast_IO_UDP.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_IO_UDP_H */
