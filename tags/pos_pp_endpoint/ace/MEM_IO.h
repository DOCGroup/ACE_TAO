/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    MEM_IO.h
//
// = AUTHOR
//    Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_MEM_IO_H
#define ACE_MEM_IO_H
#include "ace/pre.h"

#include "ace/SOCK.h"
#include "ace/MEM_SAP.h"
#include "ace/Memory_Pool.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_MEM_IO : public ACE_SOCK, public ACE_MEM_SAP
{
  // = TITLE
  //     Defines the methods for the ACE shared memeory wrapper I/O
  //     routines (e.g., send/recv).
  //
  //     The shared memory transport uses ACE_SOCK_* class to
  //     implement the signaling mechanism so we can easily use the
  //     new mechanism with the Reactor pattern (which uses select
  //     under the hood.)
  //
  //     ACE_MEM_Acceptor and ACE_MEM_Connector are used to establish
  //     connections.  When a connection is established,
  //     ACE_MEM_Acceptor creates the MMAP file for data exchange and
  //     sends the location of the file (complete path name) to
  //     ACE_MEM_Connector thru the socket.  ACE_MEM_Connector then
  //     reads the location of the file off the socket and opens up
  //     the same MMAP file.  ACE_MEM_Stream at each side then
  //     contains a reference to the ACE_Mallo object using the same
  //     MMAP file.
  //
  //     When sending information using methods provided in this
  //     class, ACE_MEM_IO requests a chunk of memory from the
  //     MALLOC_TYPE object, copy the data into the shared memory and
  //     send the memory offset (from the start of the ACE_Malloc)
  //     across the socket.  This action also servers as a signal to
  //     the other end.  The receiving side then reverses the
  //     procedures and copies the information into user buffer.
public:
  // = Initialization and termination methods.
  ACE_MEM_IO (void);
  // Constructor.

  ~ACE_MEM_IO (void);
  // Destructor.

  ssize_t send (const void *buf,
                size_t n,
                int flags) ;
  // Send an <n> byte buffer to the other process using shm_malloc_
  // connected thru the socket.

  ssize_t recv (void *buf,
                size_t n,
                int flags) ;
  // Recv an <n> byte buffer from the shm_malloc_ thru connected socket.

  ssize_t send (const void *buf,
                size_t n) ;
  // Send an <n> byte buffer to the other process using shm_malloc_
  // connected thru the socket.

  ssize_t recv (void *buf,
                size_t n) ;
  // Recv an <n> byte buffer from the shm_malloc_ thru connected socket.

  ssize_t send (const void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout);
  // Wait to to <timeout> amount of time to send up to <n> bytes into
  // <buf> from <handle> (uses the <send> call).  If <send> times out
  // a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes sent is returned.

  ssize_t recv (void *buf,
                size_t n,
                int flags,
                const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to receive up to <n> bytes
  // into <buf> from <handle> (uses the <recv> call).  If <recv> times
  // out a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes received is returned.

  ssize_t send (const void *buf,
                size_t n,
                const ACE_Time_Value *timeout);
  // Wait to to <timeout> amount of time to send up to <n> bytes into
  // <buf> from <handle> (uses the <send> call).  If <send> times out
  // a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes sent is returned.

  ssize_t recv (void *buf,
                size_t n,
                const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to receive up to <n> bytes
  // into <buf> from <handle> (uses the <recv> call).  If <recv> times
  // out a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes received is returned.

  ssize_t send (const ACE_Message_Block *message_block,
                const ACE_Time_Value *timeout);
  // Wait to to <timeout> amount of time to send the <message_block>.
  // If <send> times out a -1 is returned with <errno == ETIME>.  If
  // it succeeds the number of bytes sent is returned.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  /*  int get_local_port (u_short &) const;
  // Return the local endpoint port number.  Returns 0 if successful,
  // else -1.

  int get_remote_port (u_short &) const;
  // Return the port number of the remotely connected peer (if there
  // is one). Returns 0 if successful, else -1.
  */

protected:
  ssize_t fetch_recv_buf (int flags, const ACE_Time_Value *timeout = 0);
  // Fetch location of next available data into <recv_buffer_>.
  // As this operation read the address of the data off the socket
  // using ACE::recv, <timeout> only applies to ACE::recv.

private:
  void *recv_buffer_;
  // Internal pointer for support recv/send.

  ssize_t buf_size_;
  // Record the current total buffer size of <recv_buffer_>.

  ssize_t cur_offset_;
  // Record the current read pointer location in <recv_buffer_>.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/MEM_IO.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"
#endif /* ACE_SOCK_IO_H */
