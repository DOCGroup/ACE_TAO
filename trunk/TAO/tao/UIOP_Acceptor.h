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

# if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "ace/LSOCK_Acceptor.h"
#include "tao/Pluggable.h"
#include "tao/UIOP_Connect.h"
#include "tao/Acceptor_Impl.h"

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

  typedef TAO_Acceptor_Impl<TAO_UIOP_Server_Connection_Handler,ACE_LSOCK_ACCEPTOR> TAO_UIOP_BASE_ACCEPTOR;

private:
  int open_i (TAO_ORB_Core *orb_core, const ACE_UNIX_Addr& addr);
  // Implement the common part of the open*() methods

private:
  TAO_UIOP_BASE_ACCEPTOR base_acceptor_;
  // the concrete acceptor, as a pointer to its base class.

  TAO_ORB_Core *orb_core_;
  // ORB Core.
};

# endif /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#endif  /* TAO_UIOP_ACCEPTOR_H */
