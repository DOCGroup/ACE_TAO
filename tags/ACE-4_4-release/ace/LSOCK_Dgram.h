/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    LSOCK_Dgram.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_LOCAL_SOCK_DGRAM_H)
#define ACE_LOCAL_SOCK_DGRAM_H

#include "ace/SOCK_Dgram.h"
#include "ace/LSOCK.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

class ACE_Export ACE_LSOCK_Dgram : public ACE_SOCK_Dgram, public ACE_LSOCK
  // = TITLE
  //     Create a Local ACE_SOCK datagram. 
{
public:
  // = Initialization methods.
  ACE_LSOCK_Dgram (void);
  // Default constructor.

  ACE_LSOCK_Dgram (const ACE_Addr &local, 
		   int protocol_family = PF_UNIX, 
		   int protocol = 0);
  // Initiate a local dgram.

  int open (const ACE_Addr &local, 
	    int protocol_family = PF_UNIX, 
	    int protocol = 0);
  // Initiate a local dgram.

  ACE_HANDLE get_handle (void) const;
  // Get handle.

  void set_handle (ACE_HANDLE);
  // Set handle.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#include "ace/LSOCK_Dgram.i"

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#endif /* ACE_LOCAL_SOCK_DGRAM_H */
