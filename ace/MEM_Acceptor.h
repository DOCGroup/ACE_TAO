/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    MEM_Aceeptor.h
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

#ifndef ACE_MEM_ACCEPTOR_H
#define ACE_MEM_ACCEPTOR_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Acceptor.h"
#include "ace/MEM_Stream.h"

// Forward decl.
class ACE_Reactor;

class ACE_Export ACE_MEM_Acceptor : public ACE_SOCK_Acceptor
{
  // = TITLE
  //     Defines the format and interface for the acceptor side of the
  //     local mmap stream.  I should probably designed the class
  //     to prevent user passing a non-localhost endpoint as the acceptor
  //     listen point because it doesn't make any sense at all to make
  //     the listening endpoint visible (or connectable) anywhere outside
  //     of this machine.  However, I decided to leave the type of endpoint
  //     as <ACE_Addr> so we can later changed to use UNIX sockets with
  //     mmap stream if so desired.  (Currently, using UNIX socket with
  //     this class will not work.)
public:
  // = Initialization methods.
  ACE_MEM_Acceptor (void);
  // Default constructor.

  ACE_MEM_Acceptor (const ACE_Addr &local_sap,
                    int reuse_addr = 0,
                    int protocol_family = PF_INET,
                    int backlog = ACE_DEFAULT_BACKLOG,
                    int protocol = 0);
  // Initiate a passive mode socket.

  int accept (ACE_MEM_Stream &new_ipc_sap,
              ACE_Addr * = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;
  // Accept a new data transfer connection.

  int shared_accept_finish (ACE_MEM_Stream new_stream,
                            int in_blocking_mode,
                            int reset_new_handle) const;
  // Perform operations that must occur after <ACE_OS::accept> is
  // called.

  // = Meta-type info
  typedef ACE_Addr PEER_ADDR;
  typedef ACE_MEM_Stream PEER_STREAM;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if defined (__ACE_INLINE__)
#include "ace/MEM_Acceptor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_MEM_ACCEPTOR_H */
