/* -*- C++ -*- */
// $Id$

//============================================================================
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
//============================================================================

#if !defined (ACE_SOCK_H)
#define ACE_SOCK_H

#include "ace/ACE.h"
#include "ace/Addr.h"
#include "ace/IPC_SAP.h"

class ACE_Export ACE_SOCK : public ACE_IPC_SAP
  // = TITLE
  //     An abstract class which forms the basis for more specific
  //     classes (such as ACE_SOCK_Acceptor and ACE_SOCK_Connector).
  //     Do not instantiate this class.
  
  // =  This class provides functions that are common to all of the
  //    SOCK-type classes. ACE_SOCK provides the ability to get and set
  //    socket options, get the local and remote addresses, and close 
  //    the socket.
  
{
public:
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
  // Return the local endpoint address in the referenced ACE_Addr.
  // Returns 0 if successful, else -1.

  int get_remote_addr (ACE_Addr &) const;
  // Return the address of the remotely connected peer (if there is
  // one), in the referenced ACE_Addr. Returns 0 if successful, else -1.

  int open (int type, 
            int protocol_family, 
            int protocol,
            int reuse_addr);
  // Wrapper around the <socket> system call.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:

  ACE_SOCK (void);
  // Default constructor.  It's protected to make sure no instance is
  // instantiated.

  ACE_SOCK (int type, int protocol_family, 
            int protocol = 0, int reuse_addr = 0);
  // Constructor with arguments to call the <socket> system call.
};

#include "ace/SOCK.i"

#endif /* ACE_SOCK_H */
