/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    UPIPE_Acceptor.h
//
// = AUTHOR
//    Gerhard Lenzer and Douglas C. Schmidt
//
// ============================================================================

#ifndef ACE_UPIPE_ACCEPTOR_H
#define ACE_UPIPE_ACCEPTOR_H

#include "ace/UPIPE_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/SPIPE_Acceptor.h"
#include "ace/Thread_Manager.h"

#if defined (ACE_HAS_THREADS)

class ACE_Export ACE_UPIPE_Acceptor : public ACE_SPIPE_Acceptor
{
  // = TITLE
  //     Defines the format and interface for the listener side of the
  //     ACE_UPIPE_Stream.
public:
  // = Initialization and termination.
  ACE_UPIPE_Acceptor (void);
  // Default constructor.

  ACE_UPIPE_Acceptor (const ACE_UPIPE_Addr &local_sap,
                      int reuse_addr = 0);
  // Initialize passive endpoint.

  int open (const ACE_UPIPE_Addr &local_sap,
            int reuse_addr = 0);
  // Initialize passive endpoint.

  ~ACE_UPIPE_Acceptor (void);
  // Close down and release resources.

  int close (void);
  // Close down and release resources.

  int remove (void);
  // Close down and release resources and remove the underlying SPIPE
  // rendezvous point.

  // = Passive connection acceptance method.
  int accept (ACE_UPIPE_Stream &server_stream,
              ACE_UPIPE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0);
  // Accept a new data transfer connection.  A <timeout> of 0 means
  // block forever, a <timeout> of {0, 0} means poll.  <restart> == 1
  // means "restart if interrupted."

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Thread_Manager tm;
  // Manage threads.

  ACE_Message_Block mb_;
  // To confirm connection establishment.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/UPIPE_Acceptor.i"
#endif

#endif /* ACE_HAS_THREADS */
#endif /* ACE_UPIPE_ACCEPTOR_H */
