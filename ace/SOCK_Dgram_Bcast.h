/* -*- C++ -*- */
// $Id$


// ============================================================================

// = LIBRARY
//    ace
// 
// = FILENAME
//    SOCK_Dgram_Bcast.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SOCK_DGRAM_BCAST_H)
#define ACE_SOCK_DGRAM_BCAST_H

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"

struct ACE_Bcast_Node 
{
  ACE_Bcast_Node (ACE_INET_Addr &, ACE_Bcast_Node *);

  ACE_INET_Addr bcast_addr_;
  // Broadcast address for the interface.

  ACE_Bcast_Node *next_;
  // Pointer to the next interface in the chain.
};

class ACE_Export ACE_SOCK_Dgram_Bcast : public ACE_SOCK_Dgram
  // = TITLE
  //     Defines the member functions for the ACE_SOCK datagram
  //	 abstraction. 
{
public:
  // = Initialization and termination methods.
  ACE_SOCK_Dgram_Bcast (void);
  // Default constructor.

  ACE_SOCK_Dgram_Bcast (const ACE_Addr &local, 
			int protocol_family = PF_INET, 
			int protocol = 0);
  // Initiate a connectionless datagram broadcast endpoint.

  int open (const ACE_Addr &local, 
	    int protocol_family = PF_INET, 
	    int protocol = 0);
  // Initiate a connectionless datagram broadcast endpoint.

  int close (void);
  // Close up and release dynamically allocated resources.

  ssize_t send (const void *buf, 
		size_t n, 
		u_short portnum, 
		int flags = 0) const;
  // Broadcast the datagram to every interface.  Returns the average
  // number of bytes sent.

  ssize_t send (const iovec iov[], 
		size_t n, 
		u_short portnum, 
		int flags = 0) const;
  // Broadcast the <iovec> datagrams to every interface.  Returns the
  // average number of bytes sent.

  ssize_t send (const void *buf, 
		size_t n, 
		const ACE_Addr &addr, 
		int flags = 0) const;
  // Broadcast an N byte datagram to ADDR (note that addr must be
  // preassigned to the broadcast address of the subnet...).

  ssize_t send (const iovec iov[], 
		size_t n, 
		const ACE_Addr &addr, 
		int flags = 0) const;
  // Broadcast an <iovec> of size <n> to <addr> as a datagram (note
  // that addr must be preassigned to the broadcast address of the
  // subnet...) */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int mk_broadcast (void);
  // Make broadcast available for Datagram socket.

  ACE_Bcast_Node *if_list_;
  // Points to the head of the list of broadcast interfaces.

  int get_remote_addr (ACE_Addr &) const; 
  // Do not allow this function to percolate up to this interface...
};

#include "ace/SOCK_Dgram_Bcast.i"

#endif /* ACE_SOCK_DGRAM_BCAST_H */
