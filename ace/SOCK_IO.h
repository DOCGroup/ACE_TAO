/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SOCK_IO.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SOCK_IO_H)
#define ACE_SOCK_IO_H

#include "ace/SOCK.h"

class ACE_Export ACE_SOCK_IO : public ACE_SOCK
  // = TITLE
  //     Defines the methods for the ACE_SOCK I/O routines (i.e., send/recv). 
{
public:
  ssize_t send (const void *buf, size_t n, int flags) const;
  // Send an n byte buffer to the connected socket (uses send(3)).

  ssize_t recv (void *buf, size_t n, int flags) const;
  // Recv an n byte buffer from the connected socket (uses recv(3)).

  ssize_t send (const void *buf, size_t n) const;
  // Send an n byte buffer to the connected socket (uses write(2)).  

  ssize_t recv (void *buf, size_t n) const;
  // Recv an n byte buffer from the connected socket (uses read(2)).

  ssize_t send (const iovec iov[], size_t n) const;
  // Send a vector of n byte messages to the connected socket. 

  ssize_t recv (iovec iov[], size_t n) const;
  // Recv a vector of n byte messages to the connected socket. 

  ssize_t send (size_t n, ...) const;
  // Send varargs messages to the connected socket. 

  ssize_t recv (size_t n, ...) const;
  // Recv varargs messages to the connected socket. 

  ssize_t send (const void *buf, size_t n, ACE_OVERLAPPED *overlapped) const;
  // Send <n> bytes via Win32 WriteFile using overlapped I/O.

  ssize_t recv (void *buf, size_t n, ACE_OVERLAPPED *overlapped) const;
  // Recv <n> bytes via Win32 ReadFile using overlapped I/O.

  ssize_t recv (iovec *io_vec);
  // Allows a client to read from a socket without having to provide a
  // buffer to read.  This method determines how much data is in the
  // socket, allocates a buffer of this size, reads in the data, and
  // returns the number of bytes read.  The caller is responsible for
  // deleting the memory.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#include "ace/SOCK_IO.i"

#endif /* ACE_SOCK_IO_H */
