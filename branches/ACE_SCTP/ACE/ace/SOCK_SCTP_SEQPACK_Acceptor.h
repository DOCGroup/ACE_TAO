// -*- C++ -*-

//=============================================================================
/**
 *  @file    SOCK_SCTP_SEQPACK_Acceptor.h
 *  
 *  $Id$
 *  
 *  @author  Andrew J. Stafford <ajstafford@gmail.com>
 */
//=============================================================================

#ifndef ACE_SOCK_SCTP_SEQPACK_ACCEPTOR_H
#define ACE_SOCK_SCTP_SEQPACK_ACCEPTOR_H

#include /**/ "ace/pre.h"
#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_SCTP_SEQPACK.h"
#include "ace/Multihomed_INET_Addr.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_Time_Value;

/**
 * @class ACE_SOCK_SCTP_SEQPACK_Acceptor
 *
 * @brief Defines a passive mode one-to-many style SCTP endpoint.
 *
 * The <ACE_SOCK_SCTP_SEQPACK_Acceptor> has is its own "passive-mode" and 
 * "data mode" socket.  That is, one-to-many style SCTP endpoints do not use 
 * "accept" to create a new data mode socket, rather the passive-mode socket is 
 * also used to read/write data to/from a client.
 *
 * Note: The current implementation has only been tested with the LKSCTP 
 * implementation of the Sockets API for SCTP.
 */
class ACE_Export ACE_SOCK_SCTP_SEQPACK_Acceptor : public ACE_SOCK_SCTP_SEQPACK
{
public:

  /** 
   * Default constructor.  If this constructer is used then the open 
   * method should be called to initialize the passive socket before calling 
   * accept.
   */
  ACE_SOCK_SCTP_SEQPACK_Acceptor (void);

  /**
   * Multi-homed constructor that calls the open method to initialize the 
   * passive-mode socket.  The underlying protocol type is IPPROTO_SCTP.
   */
  ACE_SOCK_SCTP_SEQPACK_Acceptor (const ACE_Multihomed_INET_Addr &local_sap,
                                  int reuse_addr = 0,
                                  int protocol_family = PF_UNSPEC,
                                  int backlog = ACE_DEFAULT_BACKLOG);

  /**
   * Initializes a passive-mode SCTP acceptor socket.
   * <local_sap> is the set of addresses to listen for
   * connections on.  If <reuse_addr> is 1 then we'll use the
   * <SO_REUSEADDR> to reuse this address.
   */
  int open (const ACE_Multihomed_INET_Addr &local_sap,
            int reuse_addr = 0,
            int protocol_family = PF_UNSPEC,
            int backlog = ACE_DEFAULT_BACKLOG);

  /// Close the socket.  Returns 0 on success and -1 on failure.
  int close (void);

  /// Default destructor.
  ~ACE_SOCK_SCTP_SEQPACK_Acceptor (void);

  // = Meta-type info
  typedef ACE_Multihomed_INET_Addr PEER_ADDR;
  typedef ACE_SOCK_SCTP_SEQPACK PEER_STREAM;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  /// Do not allow this function to percolate up to this interface...
  int get_remote_addr (ACE_Addr &) const;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_SOCK_SEQ_ACCEPTOR_H */
