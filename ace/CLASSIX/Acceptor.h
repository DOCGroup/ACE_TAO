/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    CLASSIX/Acceptor.h
//
// = AUTHOR
//    Wei Chiang
// 
// ============================================================================

#if !defined (ACE_CLASSIX_ACCEPTOR_H)
#define ACE_CLASSIX_ACCEPTOR_H

#include "CLASSIX/Stream.h"
#include "ace/Time_Value.h"

class ACE_Export ACE_CLASSIX_Acceptor
{
  // = TITLE
  //     Defines the format and interface for a connection
  //     oriented <ACE_CLASSIX_Stream>  acceptor.
  //
  // = DESCRIPTION
  //     This class is the counter part of the <ACE_CLASSIX_Connector>
  //     class.  It exchanges SAP information to set up a logical
  //     connection.
  //     
  //     Data format passed between a connector and this class uses
  //     Peer_Union structure.
  //
  // = SEE ALSO
  //     ACE_CLASSIX_Connector
  //     
public:
  struct Peer_Port
  {
    KnUniqueId peer;
  };

  struct Peer_Group
  {
    KnCap  peer;
  };

  struct Peer_Stamp
  {
    int    peer;
  };

  struct Peer_Union
  {
    int type;
    union
    {
      Peer_Port port;
      Peer_Group group;
      Peer_Stamp stamp;
    }choose;
  };

  // = Initialization methods.
  ACE_CLASSIX_Acceptor (ACE_Reactor* = ACE_Reactor::instance());
  // Default constructor.

  ACE_CLASSIX_Acceptor (const ACE_Addr &local_sap, 
                        int reuse_addr = 0, 
                        int protocol_family = ACE_CLASSIX_Addr::AF_CLASSIX, 
                        int backlog = 0, 
                        int protocol = 0,
                        ACE_Reactor* = ACE_Reactor::instance());
  // Initiate a passive mode socket.

  virtual ~ACE_CLASSIX_Acceptor (void);
  // Default dtor.

  int open (const ACE_Addr &local_sap, 
            int reuse_addr = 0, 
            int protocol_family = ACE_CLASSIX_Addr::AF_CLASSIX, 
            int backlog = 5, 
            int protocol = 0,
            ACE_Reactor* = ACE_Reactor::instance());
  // Initiate a passive mode socket.
  int close (void);
  // Closes down the listening port.

  // = Passive connection acceptance method.
  int accept (ACE_CLASSIX_Stream &new_stream, 
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0, 
              int restart = 1,
              int reset_new_handle = 0) const;
  // Accept a new data transfer connection.  A <timeout> of 0 means
  // block forever, a <timeout> of {0, 0} means poll.  <restart> == 1
  // means "restart if interrupted."

  // = ACCESS
  ACE_HANDLE get_handle (void) const;
  // Get the underlying descriptor. 
  int get_local_addr (ACE_Addr &) const;
  // Return the local endpoint address in the referenced ACE_Addr.
  // Returns 0 if successful, else -1.

  void reactor(ACE_Reactor*);
  // reset the reactor

  // = META-TYPE info
  typedef ACE_CLASSIX_Addr PEER_ADDR;
  typedef ACE_CLASSIX_Stream PEER_STREAM;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:

private:
  // int get_remote_addr (ACE_Addr &) const; 
  // Do not allow this function to percolate up to this interface... 

  ACE_CLASSIX_Port_Core *port_;
  // optional port.
  // If 0, default port is used.
  ACE_CLASSIX_SAP       addr_;

  ACE_Reactor           *reactor_;
  // for reference only. Does not own it
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "CLASSIX/Acceptor.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_CLASSIX_ACCEPTOR_H */
