/* -*- C++ -*- */
// $Id$

//============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SOCK.h
//
// = AUTHOR
//    Doug Schmidt
//
//============================================================================

#ifndef ACE_SOCK_H
#define ACE_SOCK_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Addr.h"
#include "ace/IPC_SAP.h"
#include "ace/QoS_Session.h"

class ACE_Export ACE_SOCK : public ACE_IPC_SAP
{
  // = TITLE
  //     An abstract class that forms the basis for more specific
  //     classes, such as <ACE_SOCK_Acceptor> and <ACE_SOCK_Stream>.
  //     Do not instantiate this class.
  //
  // = DESCRIPTION
  //    This class provides functions that are common to all of the
  //    <ACE_SOCK_*> classes. <ACE_SOCK> provides the ability to get
  //    and set socket options, get the local and remote addresses,
  //    and close the socket.
public:
  ~ACE_SOCK (void);
  // Default ctor/dtor.

  int set_option (int level,
                  int option,
                  void *optval,
                  int optlen) const;
  // Wrapper around the <setsockopt> system call.

  int get_option (int level,
                  int option,
                  void *optval,
                  int *optlen) const;
  // Wrapper around the <getsockopt> system call.

  int close (void);
  // Close down the socket.

  int get_local_addr (ACE_Addr &) const;
  // Return the local endpoint address in the referenced <ACE_Addr>.
  // Returns 0 if successful, else -1.

  int get_remote_addr (ACE_Addr &) const;
  // Return the address of the remotely connected peer (if there is
  // one), in the referenced <ACE_Addr>. Returns 0 if successful, else
  // -1.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  int open (int type,
            int protocol_family,
            int protocol,
            int reuse_addr);
  // Wrapper around the BSD-style <socket> system call (no QoS).

  int open (int type,
            int protocol_family,
            int protocol,
            ACE_Protocol_Info *protocolinfo,
            ACE_SOCK_GROUP g,
            u_long flags,
            int reuse_addr);
  // Wrapper around the QoS-enabled <WSASocket> function.

  int join_qos_session (ACE_QoS_Session *qos_session);
  // Join the given QoS session. A socket can join multiple QoS
  // sessions.  This call adds the given QoS session to the list of
  // QoS sessions that the socket has already joined.

  typedef ACE_Unbounded_Set <ACE_QoS_Session *> ACE_QOS_SESSION_SET;

  ACE_QOS_SESSION_SET qos_session_set (void);
  // Get the QoS session set.
  
protected:
  ACE_SOCK (int type,
            int protocol_family,
            int protocol = 0,
            int reuse_addr = 0);
  // Constructor with arguments to call the BSD-style <socket> system
  // call (no QoS).

  ACE_SOCK (int type,
            int protocol_family,
            int protocol,
            ACE_Protocol_Info *protocolinfo,
            ACE_SOCK_GROUP g,
             u_long flags,
            int reuse_addr);
  // Constructor with arguments to call the QoS-enabled <WSASocket>
  // function.

  ACE_SOCK (void);
  // Default constructor is private to prevent instances of this class
  // from being defined.

  ACE_QOS_SESSION_SET qos_session_set_;
  // Set of QoS sessions that this socket has joined.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"
#endif /* ACE_SOCK_H */




