/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SOCK.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_SOCK_H
#define ACE_SOCK_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Addr.h"
#include "ace/IPC_SAP.h"

/**
 * @class ACE_SOCK
 *
 * @brief An abstract class that forms the basis for more specific
 * classes, such as <ACE_SOCK_Acceptor> and <ACE_SOCK_Stream>.
 * Do not instantiate this class.
 *
 * This class provides functions that are common to all of the
 * <ACE_SOCK_*> classes. <ACE_SOCK> provides the ability to get
 * and set socket options, get the local and remote addresses,
 * and close the socket.
 */
class ACE_Export ACE_SOCK : public ACE_IPC_SAP
{
public:
  /// Default ctor/dtor.
  ~ACE_SOCK (void);

  /// Wrapper around the <setsockopt> system call.
  int set_option (int level,
                  int option,
                  void *optval,
                  int optlen) const;

  /// Wrapper around the <getsockopt> system call.
  int get_option (int level,
                  int option,
                  void *optval,
                  int *optlen) const;

  /// Close down the socket.
  int close (void);

  /// Return the local endpoint address in the referenced <ACE_Addr>.
  /// Returns 0 if successful, else -1.
  int get_local_addr (ACE_Addr &) const;

  /**
   * Return the address of the remotely connected peer (if there is
   * one), in the referenced <ACE_Addr>. Returns 0 if successful, else
   * -1.
   */
  int get_remote_addr (ACE_Addr &) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  /// Wrapper around the BSD-style <socket> system call (no QoS).
  int open (int type,
            int protocol_family,
            int protocol,
            int reuse_addr);

  /// Wrapper around the QoS-enabled <WSASocket> function.
  int open (int type,
            int protocol_family,
            int protocol,
            ACE_Protocol_Info *protocolinfo,
            ACE_SOCK_GROUP g,
            u_long flags,
            int reuse_addr);

protected:
  /// Constructor with arguments to call the BSD-style <socket> system
  /// call (no QoS).
  ACE_SOCK (int type,
            int protocol_family,
            int protocol = 0,
            int reuse_addr = 0);

  /// Constructor with arguments to call the QoS-enabled <WSASocket>
  /// function.
  ACE_SOCK (int type,
            int protocol_family,
            int protocol,
            ACE_Protocol_Info *protocolinfo,
            ACE_SOCK_GROUP g,
             u_long flags,
            int reuse_addr);

  /// Default constructor is private to prevent instances of this class
  /// from being defined.
  ACE_SOCK (void);

};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"
#endif /* ACE_SOCK_H */
