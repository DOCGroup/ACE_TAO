// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    UIOP_Acceptor.h
//
// = DESCRIPTION
//    Unix Domain Socket (UIOP) specific acceptor processing
//
// = AUTHOR
//    Fred Kuhns <fredk@cs.wustl.edu>
//    Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_UIOP_ACCEPTOR_H
#define TAO_UIOP_ACCEPTOR_H

# if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "ace/Acceptor.h"
#include "ace/LSOCK_Acceptor.h"
#include "tao/Pluggable.h"
#include "tao/UIOP_Connect.h"

// *******************************************************************
//
// @@ More hacks until we get things better strategized.
//                                    -Ossama
// = Server-side factory types.
typedef ACE_Creation_Strategy<TAO_UIOP_Server_Connection_Handler> UIOP_CREATION_STRATEGY;
typedef ACE_Accept_Strategy<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR> UIOP_ACCEPT_STRATEGY;
typedef ACE_Concurrency_Strategy<TAO_UIOP_Server_Connection_Handler> UIOP_CONCURRENCY_STRATEGY;
typedef ACE_Scheduling_Strategy<TAO_UIOP_Server_Connection_Handler> UIOP_SCHEDULING_STRATEGY;
// *******************************************************************

typedef ACE_Strategy_Acceptor<TAO_UIOP_Server_Connection_Handler,
                              ACE_LSOCK_ACCEPTOR>
        TAO_UIOP_BASE_ACCEPTOR;
// was defined in UIOP_Connect.h

// TAO UIOP_Acceptor concrete call defination

class TAO_Export TAO_UIOP_Acceptor : public TAO_Acceptor
{
  // = TITLE
  //   The UIOP-specific bridge class for the concrete acceptor.
  //
  // = DESCRIPTION
  //
public:
  // TAO_UIOP_Acceptor (ACE_UNIX_Addr &addr);
  // Create Acceptor object using addr.

  TAO_UIOP_Acceptor (void);
  // Create Acceptor object using addr.

  int open (TAO_ORB_Core *orb_core, ACE_CString &address);
  // initialize acceptor for this address.

  int create_mprofile (const TAO_ObjectKey &object_key,
                      TAO_MProfile  *&mprofile);
  // create profile objects for this Acceptor using the SAP
  // (service access point) and object_key.

  // = See TAO_Acceptor
  virtual int is_collocated (const TAO_Profile*);

  virtual ACE_Event_Handler *acceptor (void);
  // Return the underlying acceptor object, ACE_Acceptor

private:
  TAO_UIOP_BASE_ACCEPTOR base_acceptor_;
  // the concrete acceptor, as a pointer to its base class.

  // = Server-side factory types.
  static UIOP_CREATION_STRATEGY UIOP_Creation_Strategy_;
  static UIOP_ACCEPT_STRATEGY UIOP_Accept_Strategy_;
  static UIOP_CONCURRENCY_STRATEGY UIOP_Concurrency_Strategy_;
  static UIOP_SCHEDULING_STRATEGY UIOP_Scheduling_Strategy_;
};

# endif /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#endif  /* TAO_UIOP_ACCEPTOR_H */
