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
#include "tao/Connect.h"

typedef ACE_Strategy_Acceptor<TAO_Server_Connection_Handler,
                              ACE_LSOCK_ACCEPTOR>
        TAO_UIOP_BASE_ACCEPTOR;
// was defined in Connect.h

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

  CORBA::ULong tag (void);
  // The tag, each concrete class will have a specific tag value.

  virtual TAO_Profile *create_profile (TAO_ObjectKey& object_key);
  // create profile object for this Acceptor using the SAP
  // (service access point, Host and Port) and object_key.

  virtual ACE_Event_Handler *acceptor (void);
  // Return the underlying acceptor object, ACE_Acceptor

private:
  TAO_UIOP_BASE_ACCEPTOR base_acceptor_;
  // the concrete acceptor, as a pointer to its base class.

  CORBA::ULong tag_;
  //  the IOP specific tag.
};

# endif /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#endif  /* TAO_UIOP_ACCEPTOR_H */
