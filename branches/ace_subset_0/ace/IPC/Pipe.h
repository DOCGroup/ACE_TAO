/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Pipe.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_PIPE_H
#define ACE_PIPE_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Pipe
 *
 * @brief Provides a bidirectional "pipe" abstraction that is portable
 * to Windows NT, SVR4 UNIX, and BSD UNIX.
 *
 * Uses "name" for lookup in the ACE service repository. Obtains
 * the object and returns it as the appropriate type.
 */
class ACE_Export ACE_Pipe
{
public:
  // = Initialization and termination.
  /// Default constructor (does nothing...).
  ACE_Pipe (void);

  /// Open the pipe and initialize the handles.
  ACE_Pipe (ACE_HANDLE handles[2]);

  /// Initialize the <ACE_Pipe> from the <read> and <write> handles.
  ACE_Pipe (ACE_HANDLE read, ACE_HANDLE write);

  /// Default dtor.  It doesn't close the handles for you.
  ~ACE_Pipe (void);

  /// Open the pipe and initialize the handles.
  int open (ACE_HANDLE handles[2]);

  /// Open the pipe, setting the buffer size to the maximum.
  int open (int buffer_size = ACE_DEFAULT_MAX_SOCKET_BUFSIZ);

  /// Close down the pipe HANDLEs;
  int close (void);

  // = Accessors.

  /**
   * This is the "read" side of the pipe.  Note, however, that
   * processes can also write to this handle as well since pipes are
   * bi-directional.
   */
  ACE_HANDLE read_handle (void) const;

  /**
   * This is the "write" side of the pipe.  Note, however, that
   * processes can also read to this handle as well since pipes are
   * bi-directional.
   */
  ACE_HANDLE write_handle (void) const;

  /// Dump the state of the object.
  void dump (void) const;

private:
  ACE_HANDLE handles_[2];
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/IPC/Pipe.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"
#endif /* ACE_PIPE_H */
