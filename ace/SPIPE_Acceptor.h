/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SPIPE_Acceptor.h
//
// = AUTHOR
//    Doug Schmidt and Prashant Jain
// 
// ============================================================================

#if !defined (ACE_SPIPE_ACCEPTOR_H) 
#define ACE_SPIPE_ACCEPTOR_H

#include "ace/SPIPE_Stream.h"

class ACE_Export ACE_SPIPE_Acceptor : public ACE_SPIPE
  // = TITLE
  //     Defines the format and interface for the listener side of the
  //     ACE_SPIPE_Stream. 
{
public:
#if defined (ACE_WIN32)
  // = Maximum number of attempts to accept a connection
  enum 
  {
    MAX_PIPE_INSTANCES = PIPE_UNLIMITED_INSTANCES,
    MAX_ACCEPT_ATTEMPTS = 3 
  };
#endif /* ACE_WIN32 */

  // = Initialization and termination methods.
  ACE_SPIPE_Acceptor (void);
  // Default constructor.

  ACE_SPIPE_Acceptor (const ACE_SPIPE_Addr &local_sap, 
		      int reuse_addr = 1,
		      int perms = ACE_DEFAULT_FILE_PERMS);
  // Initiate a passive-mode STREAM pipe listener.

  int open (const ACE_SPIPE_Addr &local_sap,
	    int reuse_addr = 1,
	    int perms = ACE_DEFAULT_FILE_PERMS);
  // Initiate a passive-mode STREAM pipe listener.

  int close (void);
  // Close down the passive-mode STREAM pipe listener.

  int remove (void);
  // Remove the underlying mounted pipe from the file system.

  // = Passive connection acceptance method.
  int accept (ACE_SPIPE_Stream &ipc_sap_spipe, 
	      ACE_SPIPE_Addr *remote_addr = 0,
	      ACE_Time_Value *timeout = 0,
	      int restart = 1);
  // Accept a new data transfer connection.  A <timeout> of 0 means
  // block forever, a <timeout> of {0, 0} means poll.  <restart> == 1
  // means "restart if interrupted."

  // = Meta-type info
  typedef ACE_SPIPE_Addr PEER_ADDR;
  typedef ACE_SPIPE_Stream PEER_STREAM;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int create_new_instance (int perms = 0);
  // Create a new instance of an SPIPE.
};

#endif /* ACE_SPIPE_ACCEPTOR_H */
