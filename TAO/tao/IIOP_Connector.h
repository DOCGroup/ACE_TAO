// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    IIOP_Connector.h
//
// = DESCRIPTION
//    IIOP specific connector processing
//
// = AUTHOR
//    Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IIOP_CONNECTOR_H
#define TAO_IIOP_CONNECTOR_H

#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"
#include "tao/Pluggable.h"
#include "tao/Connect.h"

typedef ACE_Strategy_Connector<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>
        TAO_IIOP_BASE_CONNECTOR;

class TAO_Export TAO_IIOP_Connector : public TAO_Connector
{
  // = TITLE
  //   TAO_IIOP_Connector
  //
  // = DESCRIPTION
  //   IIOP Specific Connector bridge for pluggable protocols.
public:

  TAO_IIOP_Connector (void);
  // Constructor.  Do we want to pass in the tag here or should it be
  // statically defined?

  int preconnect (char *preconnections);
  // Initial set of connections to be established.

  int open (TAO_Resource_Factory *trf, ACE_Reactor *reactor);
  //  Initialize object and register with reactor. 

  int close (void);
  // Shutdown Connector bridge and concreate Connector.

  CORBA::ULong tag (void);
  // The tag identifying the specific ORB transport layer protocol.
  // For example TAO_IOP_TAG_INTERNET_IOP = 0.  The tag is used in the
  // IOR to identify the type of profile included. IOR -> {{tag0,
  // profile0} {tag1, profole1} ...}  GIOP.h defines typedef
  // CORBA::ULong TAO_IOP_Profile_ID;


  TAO_Transport* connect (TAO_Profile *profile,
                          CORBA::Environment &env);
  // Connect will be called from TAO_GIOP_Invocation::start

private:
  CORBA::ULong tag_;
  // IIOP tag.

  TAO_IIOP_BASE_CONNECTOR  base_connector_;
  // The connector initiating connection requests for IIOP.
};

#endif  /* TAO_IIOP_CONNECTOR_H */
