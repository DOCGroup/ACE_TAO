// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    LSOCK_Connector.h
//
// = DESCRIPTION
//    LSOCK specific connector processing
//
// = AUTHOR
//    Fred Kuhns <fredk@cs.wustl.edu>
//    Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_LSOCK_CONNECTOR_H
#define TAO_LSOCK_CONNECTOR_H

# if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "ace/Connector.h"
#include "ace/LSOCK_Connector.h"
#include "tao/Pluggable.h"
#include "tao/Connect.h"

typedef ACE_Strategy_Connector<TAO_Client_Connection_Handler,
                               ACE_LSOCK_CONNECTOR> TAO_LSOCK_BASE_CONNECTOR;

class TAO_Export TAO_LSOCK_Connector : public TAO_Connector
{
  // = TITLE
  //   LSOCK-specific Connector bridge for pluggable protocols.
  //
  // = DESCRIPTION
  //   
public:
  // = Initialization and termination methods.
  TAO_LSOCK_Connector (void);
  // Constructor.
  // @@ Do we want to pass in the tag here or should it be statically
  // defined?

  int open (TAO_Resource_Factory *trf, ACE_Reactor *reactor);
  //  Initialize object and register with reactor.

  int close (void);
  // Shutdown Connector bridge and concreate Connector.

  int preconnect (char *preconnections);
  // Initial set of connections to be established.

  CORBA::ULong tag (void);
  // The tag identifying the specific ORB transport layer protocol.
  // For example TAO_IOP_TAG_INTERNET_IOP = 0.  The tag is used in the
  // IOR to identify the type of profile included. IOR -> {{tag0,
  // profile0} {tag1, profile1} ...}  GIOP.h defines typedef
  // CORBA::ULong TAO_IOP_Profile_ID;

  int connect (TAO_Profile *profile,
               TAO_Transport *&transport);
  // Connect will be called from TAO_GIOP_Invocation::start

private:
  CORBA::ULong tag_;
  // LSOCK tag.

  TAO_LSOCK_BASE_CONNECTOR  base_connector_;
  // The connector initiating connection requests for LSOCK.
};

# endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#endif  /* TAO_LSOCK_CONNECTOR_H */
