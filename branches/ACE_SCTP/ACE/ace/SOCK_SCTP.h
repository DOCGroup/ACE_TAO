// -*- C++ -*-

//=============================================================================
/**
 *
 *  @file SOCK_SCTP.h
 *
 *  $Id$
 *
 *  @author  Andrew Stafford <ajstafford@gmail.com>
 *  @author  based on SOCK_Stream
 *            by Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *
 */
//=============================================================================

#ifndef ACE_SOCK_SCTP_H
#define ACE_SOCK_SCTP_H

#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK.h"
#include "ace/Multihomed_INET_Addr.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_Export ACE_SOCK_SCTP : public virtual ACE_SOCK
{
public:

  /**
   * Return local endpoint addresses associated with the specified association 
   * ID.  For one-to-one sockets (SOCK_SCTP_STREAM), assoc_id should 
   * be set to 0.  in the referenced array of
   * ACE_INET_Addr, which should have the specified @a size.  If the
   * number of local endpoint addresses is less than @a size, then
   * @a size will be set to this number.  If successful, the method
   * returns 0, otherwise returns -1.
   */
  int get_local_addrs (ACE_INET_Addr *addrs, size_t &size, int assoc_id) const;

  /**
   * Return remote endpoint addresses in the referenced array of
   * ACE_INET_Addr, which should have the specified @a size.  If the
   * number of remote endpoint addresses is less than @a size, then
   * @a size will be set to this number.  If successful, the method
   * returns 0, otherwise returns -1.
   */
  int get_remote_addrs (ACE_INET_Addr *addrs, size_t &size, int assoc_id) const;

  ssize_t recvmsg(void* msg,
                  size_t msgsz,
                  ACE_Multihomed_INET_Addr& from_addr,
                  struct sctp_sndrcvinfo* sinfo,
                  int* msg_flags);
  
  ssize_t sendmsg(const void* msg,
                  size_t msgsz,
                  ACE_Multihomed_INET_Addr& to_addr,
                  uint32_t ppid = 0,
                  uint32_t flags = 0,
                  uint16_t stream = 0,
                  uint32_t timetolive = 0,
                  uint32_t context = 0);

  // = I/O functions.
  int close (void);

  // = Meta-type info
  typedef ACE_Multihomed_INET_Addr PEER_ADDR;

protected:

  /// Default constructor - protected to prevent direct instantiation
  ACE_SOCK_SCTP();

  /// Constructor (sets the underlying ACE_HANDLE with <h>).
  ACE_SOCK_SCTP (ACE_HANDLE h);

  ~ACE_SOCK_SCTP();
  
  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_SOCK_SCTP_H */
