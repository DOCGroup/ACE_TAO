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

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "ace/LSOCK_Acceptor.h"
#include "tao/Pluggable.h"
#include "tao/UIOP_Connect.h"
#include "tao/Acceptor_Impl.h"

// Forward declaration.
class TAO_UIOP_Connector;

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

  virtual ~TAO_UIOP_Acceptor (void);
  // Destructor

  int open (TAO_ORB_Core *orb_core,
            int version_major,
            int version_minor,
            ACE_CString &address);
  // initialize acceptor for this address.

  virtual int close (void);
  // Closes the acceptor

  virtual int open_default (TAO_ORB_Core *orb_core);
  // Open an acceptor on the default endpoint for this protocol

  int create_mprofile (const TAO_ObjectKey &object_key,
                       TAO_MProfile &mprofile);
  // create profile objects for this Acceptor using the SAP
  // (service access point) and object_key.

  // = See TAO_Acceptor
  virtual int is_collocated (const TAO_Profile*);

  virtual ACE_Event_Handler *acceptor (void);
  // Return the underlying acceptor object, ACE_Acceptor

  CORBA::ULong endpoint_count (void);
  // return the number of profiles this will generate

  typedef ACE_Strategy_Acceptor<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR> TAO_UIOP_BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_UIOP_Server_Connection_Handler> TAO_UIOP_CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_UIOP_Server_Connection_Handler> TAO_UIOP_CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR, TAO_UIOP_Connector> TAO_UIOP_ACCEPT_STRATEGY;

private:
  int open_i (TAO_ORB_Core *orb_core, const char *rendezvous);
  // Implement the common part of the open*() methods

  void rendezvous_point (ACE_UNIX_Addr &, const char *rendezvous);
  // Set the rendezvous point and verify that it is
  // valid (e.g. wasn't truncated because it was too long).

private:
  TAO_UIOP_BASE_ACCEPTOR base_acceptor_;
  // the concrete acceptor, as a pointer to its base class.

  TAO_UIOP_CREATION_STRATEGY *creation_strategy_;
  TAO_UIOP_CONCURRENCY_STRATEGY *concurrency_strategy_;
  TAO_UIOP_ACCEPT_STRATEGY *accept_strategy_;
  // Acceptor strategies.

  TAO_GIOP_Version version_;
  // The GIOP version for this endpoint

  TAO_ORB_Core *orb_core_;
  // ORB Core.

  int unlink_on_close_;
  // Flag that determines whether or not the rendezvous point should
  // be unlinked on close.  This is really only used when an error
  // occurs.
};

# endif /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#endif  /* TAO_UIOP_ACCEPTOR_H */
