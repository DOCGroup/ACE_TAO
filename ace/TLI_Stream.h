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

#if !defined (ACE_TLI_STREAM_H)
#define ACE_TLI_STREAM_H

#include "ace/TLI.h"
#include "ace/INET_Addr.h"

#if defined (ACE_HAS_TLI)

class ACE_Export ACE_TLI_Stream : public ACE_TLI
  // = TITLE
  //     Defines the member functions for ACE_TLI_Stream abstraction. 
{
friend class ACE_TLI_Acceptor;
friend class ACE_TLI_Connector;
public:
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
  ssize_t send (const void *buf, size_t n, int flags) const;
  // Send an n byte buffer to the connected socket (uses t_snd(3)).
  ssize_t recv (void *buf, size_t n, int *flags) const;
  // Recv an n byte buffer from the connected socket (uses t_rcv(3)).

  ssize_t send_n (const void *buf, size_t n, int flags) const;
  // Send exactly n bytes to the connected socket (uses t_snd(3)).
  ssize_t recv_n (void *buf, size_t n, int *flags) const;
  // Recv exactly n bytes from the connected socket (uses t_rcv(3)).

  // = tirdwr bindings 
  ssize_t send (const void *buf, size_t n) const;
  // Send an n byte buffer to the connected socket (uses write(2)).  

  ssize_t recv (void *buf, size_t n) const;
  // Recv an n byte buffer from the connected socket (uses read(2)).

  ssize_t send_n (const void *buf, size_t n) const;
  // Send n bytes, keep trying until n are sent (uses write(2)).

  ssize_t recv_n (void *buf, size_t n) const;
  // Recv n bytes, keep trying until n are received (uses read (2)).

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

#include "ace/TLI_Stream.i"

#endif /* ACE_HAS_TLI */
#endif /* ACE_TLI_STREAM_H */
