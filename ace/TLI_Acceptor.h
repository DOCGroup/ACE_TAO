/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    TLI_Acceptor.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_TLI_ACCEPTOR_H
#define ACE_TLI_ACCEPTOR_H

#include "ace/TLI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Time_Value.h"
#include "ace/TLI_Stream.h"

#if defined (ACE_HAS_TLI)

// Forward reference...
class ACE_TLI_Request_Queue;

class ACE_Export ACE_TLI_Acceptor : public ACE_TLI
{
  // = TITLE
  //     Defines the member functions for ACE_TLI_Acceptor abstraction.
  //
  // = DESCRIPTION
  //     This class implements the algorithm described in Steve Rago's
  //     book on System V UNIX network programming.  It basically
  //     makes TLI look like the C++ SOCK_SAP socket wrappers with
  //     respect to establishing passive-mode listener endpoints.
public:
  friend class ACE_Request_Queue;

  // = Initialization and termination methods.
  ACE_TLI_Acceptor (void);
  // Default constructor.

  ACE_TLI_Acceptor (const ACE_Addr &remote_sap,
                    int reuse_addr = 0,
                    int oflag = O_RDWR,
                    struct t_info *info = 0,
                    int backlog = 5,
                    const char device[] = ACE_TLI_TCP_DEVICE);
  // Initiate a passive mode socket.

  ACE_HANDLE open (const ACE_Addr &remote_sap,
                   int reuse_addr = 0,
                   int oflag = O_RDWR,
                   struct t_info *info = 0,
                   int backlog = 5,
                   const char device[] = ACE_TLI_TCP_DEVICE);
  // Initiate a passive mode socket.

  int close (void);
  // Close down the acceptor and release resources.

  // = Passive connection acceptance method.

  int accept (ACE_TLI_Stream &new_tli_sap,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0,
              int rwflag = 1,
              netbuf *udata = 0,
              netbuf *opt = 0);
  // Accept a new data transfer connection.  A <timeout> of 0 means
  // block forever, a <timeout> of {0, 0} means poll.  <restart> == 1
  // means "restart if interrupted."

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;
  typedef ACE_TLI_Stream PEER_STREAM;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const char *device_;
  // Network "device" we are using.

  int backlog_;
  // Number of connections to queue.

  int rwflag_;
  // Are we using "tirdwr" mod?

  int handle_async_event (int restart, int rwflag);
  // Handle TLI accept insanity...

  ACE_TLI_Request_Queue *queue_;
  // Used for queueing up pending requests.

  struct t_discon *disp_;
  // Used for handling disconnects
};

#include "ace/TLI_Acceptor.i"

#endif /* ACE_HAS_TLI */
#endif /* ACE_TLI_ACCEPTOR_H */
