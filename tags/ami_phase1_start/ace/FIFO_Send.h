/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    FIFO_Send.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_FIFO_SEND_H
#define ACE_FIFO_SEND_H

#include "ace/FIFO.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_FIFO_Send : public ACE_FIFO
{
  // = TITLE
  //     Sender side for the bytestream C++ wrapper for UNIX FIFOs
public:
  // = Initialization methods.
  ACE_FIFO_Send (void);
  // Default constructor.

  ACE_FIFO_Send (const char *rendezvous,
                 int flags = O_WRONLY,
                 int perms = ACE_DEFAULT_FILE_PERMS,
                 LPSECURITY_ATTRIBUTES sa = 0);
  // Open up a bytestream named pipe for writing.

  int open (const char *rendezvous,
            int flags = O_WRONLY,
            int perms = ACE_DEFAULT_FILE_PERMS,
            LPSECURITY_ATTRIBUTES sa = 0);
  // Open up a bytestream named pipe for writing.

  ssize_t send (const void *buf, size_t len);
  // Send <buf> of up to <len> bytes.

  ssize_t send_n (const void *buf, size_t len);
  // Send <buf> of exactly <len> bytes (block until done).

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/FIFO_Send.i"
#endif

#endif /* ACE_FIFO_SEND_H */

