/* -*- C++ -*- */
// $Id$


// ===========================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SOCK_Dgram.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ===========================================================================

#if !defined (ACE_SOCK_DGRAM_H)
#define ACE_SOCK_DGRAM_H

#include "ace/SOCK.h"
#include "ace/Addr.h"

class ACE_Export ACE_SOCK_Dgram : public ACE_SOCK
  // = TITLE
  //     Defines the member functions for the ACE_SOCK datagram
  //     abstraction. 
{
public:
  // = Initialization routines.
  ACE_SOCK_Dgram (void);
  // Default constructor.

  ACE_SOCK_Dgram (const ACE_Addr &local, 
		  int protocol_family = PF_INET, 
		  int protocol = 0);
  // Initiate a socket dgram.

  int open (const ACE_Addr &local, 
	    int protocol_family = PF_INET, 
	    int protocol = 0);
  // Initiate a socket dgram.

  // = Data transfer routines.
  ssize_t send (const void *buf, 
		size_t n, 
		const ACE_Addr &addr, 
		int flags = 0) const;
  // Send an <n> byte <buf> to the datagram socket (uses sendto(3)).

  ssize_t recv (void *buf, 
		size_t n, 
		ACE_Addr &addr, 
		int flags = 0) const;
  // Receive an <n> byte <buf> from the datagram socket (uses
  // recvfrom(3)).

  ssize_t recv (iovec *io_vec, ACE_Addr &addr, int flags) const;
  // Allows a client to read from a socket without having to provide a
  // buffer to read.  This method determines how much data is in the
  // socket, allocates a buffer of this size, reads in the data, and
  // returns the number of bytes read.  The caller is responsible for
  // deleting the member in the <iov_base> field of <io_vec>.

#if defined (ACE_HAS_MSG)
  ssize_t send (const iovec iov[], 
		size_t n, 
		const ACE_Addr &addr, 
		int flags = 0) const;
  // Send an <iovec> of size <n> to the datagram socket (uses
  // sendmsg(3)).

  ssize_t recv (iovec iov[], 
		size_t n, 
		ACE_Addr &addr, 
		int flags = 0) const;    
  // Recv an <iovec> of size <n> to the datagram socket (uses
  // recvmsg(3)).
#endif /* ACE_HAS_MSG */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:  
  int shared_open (const ACE_Addr &local, 
		   int protocol_family);
  // Open is shared by this and by <LSOCK_Dgram>.

private:
  int  get_remote_addr (ACE_Addr &) const; 
  // Do not allow this function to percolate up to this interface...
};

#include "ace/SOCK_Dgram.i"

#endif /* ACE_SOCK_DGRAM_H */
