/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    FIFO_Recv.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_FIFO_RECV_H
#define ACE_FIFO_RECV_H
#include "ace/pre.h"

#include "ace/FIFO.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_FIFO_Recv
 *
 * @brief Receiver side of the bytestream C++ wrapper for UNIX
 * FIFOs.
 */
class ACE_Export ACE_FIFO_Recv : public ACE_FIFO
{
public:
  // = Initialization methods.
  /// Default constructor.
  ACE_FIFO_Recv (void);

  /// Open up a bytestream named pipe for reading.
  ACE_FIFO_Recv (const ACE_TCHAR *rendezvous,
                 int flags = O_CREAT | O_RDONLY,
                 int perms = ACE_DEFAULT_FILE_PERMS,
                 int persistent = 1,
                 LPSECURITY_ATTRIBUTES sa = 0);

  /// Open up a bytestream named pipe for reading.
  int open (const ACE_TCHAR *rendezvous,
            int flags = O_CREAT | O_RDONLY,
            int perms = ACE_DEFAULT_FILE_PERMS,
            int persistent = 1,
            LPSECURITY_ATTRIBUTES sa = 0);

  /// Close down the named pipe.
  int close (void);

  /// Recv <buf> of up to <len> bytes.
  ssize_t recv (void *buf, size_t len);

  /// Recv <buf> of exactly <len> bytes (block until done).
  ssize_t recv_n (void *buf, size_t len);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Auxiliary handle that is used to implement persistent FIFOs.
  ACE_HANDLE aux_handle_;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/FIFO_Recv.i"
#endif

#include "ace/post.h"
#endif /* ACE_FIFO_RECV_H */
