/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    FIFO_Recv.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_FIFO_RECV_H)
#define ACE_FIFO_RECV_H

#include "ace/FIFO.h"

class ACE_Export ACE_FIFO_Recv : public ACE_FIFO
  // = TITLE
  //    Receiver side of the bytestream C++ wrapper for UNIX
  //    FIFOs.  
{
public:
  // = Initialization methods.
  ACE_FIFO_Recv (void);
  // Default constructor.

  ACE_FIFO_Recv (const char *rendezvous, 
		 int flags = O_CREAT | O_RDONLY, 
		 int perms = ACE_DEFAULT_FILE_PERMS, 
		 int persistent = 1,
                 LPSECURITY_ATTRIBUTES sa = 0);
  // Open up a bytestream named pipe for reading.

  int open (const char *rendezvous, 
	    int flags = O_CREAT | O_RDONLY, 
	    int perms = ACE_DEFAULT_FILE_PERMS, 
	    int persistent = 1,
            LPSECURITY_ATTRIBUTES sa = 0);
  // Open up a bytestream named pipe for reading.

  int close (void);
  // Close down the named pipe.

  ssize_t recv (void *buf, size_t len);
  // Recv <buf> of up to <len> bytes.

  ssize_t recv_n (void *buf, size_t len);
  // Recv <buf> of exactly <len> bytes (block until done).
  
  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_HANDLE aux_handle_;
  // Auxiliary handle that is used to implement persistent FIFOs.
};

#include "ace/FIFO_Recv.i"

#endif /* ACE_FIFO_RECV_H */
