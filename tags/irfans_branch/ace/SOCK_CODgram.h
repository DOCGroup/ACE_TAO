/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SOCK_CODgram.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SOCK_CODGRAM_H)
#define ACE_SOCK_CODGRAM_H

#include "ace/SOCK_IO.h"
#include "ace/Addr.h"

class ACE_Export ACE_SOCK_CODgram : public ACE_SOCK_IO
  // = TITLE
  //     Defines the member functions for the ACE_SOCK connected
  // datagram abstraction. 
{
public:
  // = Initialization methods.
  ACE_SOCK_CODgram (void);
  // Default constructor.

  ACE_SOCK_CODgram (const ACE_Addr &remote_sap, 
		    const ACE_Addr &local_sap = ACE_Addr::sap_any, 
		    int protocol_family = PF_INET, 
		    int protocol = 0,
		    int reuse_addr = 0);
  // Initiate a connected dgram.

  int open (const ACE_Addr &remote_sap, 
 	    const ACE_Addr &local_sap = ACE_Addr::sap_any, 
	    int protocol_family = PF_INET, 
	    int protocol = 0,
	    int reuse_addr = 0);
  // Initiate a connected dgram.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#include "ace/SOCK_CODgram.i"

#endif /* ACE_SOCK_CODGRAM_H */
