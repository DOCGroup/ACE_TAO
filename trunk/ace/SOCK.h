/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SOCK.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SOCK_H)
#define ACE_SOCK_H

#include "ace/ACE.h"
#include "ace/Addr.h"
#include "ace/IPC_SAP.h"

class ACE_Export ACE_SOCK : public ACE_IPC_SAP
  // = TITLE
  //     Defines the member functions for the base class of the
  //     ACE_SOCK abstraction. 
{
public:
  int open (int type, 
	    int protocol_family, 
	    int protocol);
  // Wrapper around the socket() system call.

  int set_option (int level, 
		  int option, 
		  void *optval, 
		  int optlen) const;
  // Wrapper around the setsockopt() system call.

  int get_option (int level, 
		  int option, 
		  void *optval, 
		  int *optlen) const;
  // Wrapper around the getsockopt() system call.

  int close (void);
  // Close down the socket.

  int get_local_addr (ACE_Addr &) const;
  // Return the local endpoint address.

  int get_remote_addr (ACE_Addr &) const;
  // Return the address of the remotely connected peer (if there is
  // one).

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Make this an abstract class.
  ACE_SOCK (void);
  // Default constructor.

  ACE_SOCK (int type, int protocol_family, int protocol = 0);
  // Wrapper around the socket() system call.

#if defined (ACE_WIN32)
  static ACE_SOCK dummy_;
  // Used to ensure we initialize WinSock!
#endif /* ACE_WIN32 */
};

#include "ace/SOCK.i"

#endif /* ACE_SOCK_H */
