/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    TLI_Stream.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_TLI_STREAM_H
#define ACE_TLI_STREAM_H
#include "ace/pre.h"

#include "ace/TLI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"

#if defined (ACE_HAS_TLI)

class ACE_Export ACE_TLI_Stream : public ACE_TLI
{
  // = TITLE
  //     Defines the member functions for ACE_TLI_Stream abstraction.
public:
  friend class ACE_TLI_Acceptor;
  friend class ACE_TLI_Connector;

  // = Initialization and termination methods.
  ACE_TLI_Stream (void);
  // Default constructor.

  // = TLI-specific shutdown operations.
  int close (void);
  // Close down and release resources.

  int active_close (void);
  // Send a release and then await the release from the other side.

  int passive_close (void);
  // Acknowledge the release from the other side and then send the
  // release to the other side.

  int get_remote_addr (ACE_Addr &) const;
  // Return address of remotely connected peer.

  // =  timod bindings
  ssize_t send (const void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout = 0) const;
  // Send an n byte buffer to the connected socket (uses t_snd(3)).
  ssize_t recv (void *buf,
                size_t n,
                int *flags,
                const ACE_Time_Value *timeout = 0) const;
  // Recv an n byte buffer from the connected socket (uses t_rcv(3)).

  ssize_t send_n (const void *buf,
                  size_t n,
                  int flags,
                  const ACE_Time_Value *timeout = 0,
                  size_t *bytes_transferred = 0) const;
  // Send exactly n bytes to the connected socket (uses t_snd(3)).
  ssize_t recv_n (void *buf,
                  size_t n,
                  int *flags,
                  const ACE_Time_Value *timeout = 0,
                  size_t *bytes_transferred = 0) const;
  // Recv exactly n bytes from the connected socket (uses t_rcv(3)).

  // = tirdwr bindings
  ssize_t send (const void *buf,
                size_t n,
                const ACE_Time_Value *timeout = 0) const;
  // Send an n byte buffer to the connected socket (uses write(2)).

  ssize_t recv (void *buf,
                size_t n,
                const ACE_Time_Value *timeout = 0) const;
  // Recv an n byte buffer from the connected socket (uses read(2)).

  ssize_t send_n (const void *buf,
                  size_t n,
                  const ACE_Time_Value *timeout = 0,
                  size_t *bytes_transferred = 0) const;
  // Send n bytes, keep trying until n are sent (uses write(2)).

  ssize_t recv_n (void *buf,
                  size_t n,
                  const ACE_Time_Value *timeout = 0,
                  size_t *bytes_transferred = 0) const;
  // Recv n bytes, keep trying until n are received (uses read (2)).

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int rwflag_;
  // Indicates whether the tirdwr module should be pushed

  // = Get/set rwflag
  int  get_rwflag (void);
  void set_rwflag (int);
};

#if defined (__ACE_INLINE__)
#include "ace/TLI_Stream.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_TLI */
#include "ace/post.h"
#endif /* ACE_TLI_STREAM_H */
