/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    FIFO_Recv_Msg.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_FIFO_RECV_MSG_H
#define ACE_FIFO_RECV_MSG_H
#include "ace/pre.h"

#include "ace/FIFO_Recv.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_FIFO_Recv_Msg
 *
 * @brief Receiver side for the record oriented C++ wrapper for UNIX FIFOs.
 */
class ACE_Export ACE_FIFO_Recv_Msg : public ACE_FIFO_Recv
{
public:
  // = Initialization methods.
  /// Default constructor.
  ACE_FIFO_Recv_Msg (void);

  /// Open up a record-oriented named pipe for reading.
  ACE_FIFO_Recv_Msg (const ACE_TCHAR *rendezvous,
                     int flags = O_CREAT | O_RDONLY,
                     int perms = ACE_DEFAULT_FILE_PERMS,
                     int persistent = 1,
                     LPSECURITY_ATTRIBUTES sa = 0);

  /// Open up a record-oriented named pipe for reading.
  int open (const ACE_TCHAR *rendezvous,
            int flags = O_CREAT | O_RDONLY,
            int perms = ACE_DEFAULT_FILE_PERMS,
            int persistent = 1,
            LPSECURITY_ATTRIBUTES sa = 0);

  /// Recv <msg> as an ACE_Str_Buf.
  ssize_t recv (ACE_Str_Buf &msg);

  /// Recv <msg> as a buffer.
  ssize_t recv (void *buf, size_t len);

#if defined (ACE_HAS_STREAM_PIPES)
  /// Recv <data> and <cntl> message via Stream pipes.
  ssize_t recv (ACE_Str_Buf *data,
                ACE_Str_Buf *cntl,
                int *flags);

  /// Recv <data> and <cntl> message via Stream pipes in "band" mode.
  ssize_t recv (int *band,
                ACE_Str_Buf *data,
                ACE_Str_Buf *cntl,
                int *flags);
#endif /* ACE_HAS_STREAM_PIPES */

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/FIFO_Recv_Msg.i"
#endif

#include "ace/post.h"
#endif /* ACE_FIFO_RECV_MSG_H */
