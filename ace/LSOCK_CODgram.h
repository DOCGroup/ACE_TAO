/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    LSOCK_CODgram.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_LOCAL_SOCK_CODGRAM_H)
#define ACE_LOCAL_SOCK_CODGRAM_H

#include "ace/LSOCK.h"
#include "ace/SOCK_CODgram.h"
#include "ace/Addr.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

class ACE_Export ACE_LSOCK_CODgram : public ACE_SOCK_CODgram, public ACE_LSOCK
  // = TITLE
  //     Defines the member functions for the ACE_LSOCK 
  //     connected datagram abstraction. 
{
public:
  // = Initialization methods.
  ACE_LSOCK_CODgram (void);
  // Default constructor.

  ACE_LSOCK_CODgram (const ACE_Addr &remote_sap, 
		     const ACE_Addr &local_sap = ACE_Addr::sap_any, 
		     int protocol_family = PF_UNIX, 
		     int protocol = 0);
  // Initiate a connected-datagram.

  int open (const ACE_Addr &remote_sap, 
	    const ACE_Addr &local_sap = ACE_Addr::sap_any, 
	    int protocol_family = PF_UNIX, 
	    int protocol = 0);
  // Initiate a connected-datagram.

  ACE_HANDLE get_handle (void) const;
  // Get underlying handle.

  void set_handle (ACE_HANDLE);
  // Set underlying handle.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#include "ace/LSOCK_CODgram.i"

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#endif /* ACE_LOCAL_SOCK_CODGRAM_H */
