// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    UIOP_Connector.h
//
// = DESCRIPTION
//    UIOP specific connector processing
//
// = AUTHOR
//    Fred Kuhns <fredk@cs.wustl.edu>
//    Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_UIOP_CONNECTOR_H
#define TAO_UIOP_CONNECTOR_H

# if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "ace/Connector.h"
#include "ace/LSOCK_Connector.h"

#include "tao/Pluggable.h"
#include "tao/UIOP_Connect.h"


// ****************************************************************

#include "tao/Resource_Factory.h"  // @@ Needed for some temporary hacks

typedef ACE_Cached_Connect_Strategy<TAO_UIOP_Client_Connection_Handler,
                                    ACE_LSOCK_CONNECTOR,
                                    TAO_Cached_Connector_Lock>
        TAO_UIOP_CACHED_CONNECT_STRATEGY;

typedef ACE_NOOP_Creation_Strategy<TAO_UIOP_Client_Connection_Handler>
        TAO_UIOP_NULL_CREATION_STRATEGY;

typedef ACE_NOOP_Concurrency_Strategy<TAO_UIOP_Client_Connection_Handler>
        TAO_UIOP_NULL_ACTIVATION_STRATEGY;

// ****************************************************************


typedef ACE_Strategy_Connector<TAO_UIOP_Client_Connection_Handler,
                               ACE_LSOCK_CONNECTOR> TAO_UIOP_BASE_CONNECTOR;

class TAO_Export TAO_UIOP_Connector : public TAO_Connector
{
  // = TITLE
  //   UIOP-specific Connector bridge for pluggable protocols.
  //
  // = DESCRIPTION
  //   
public:
  // = Initialization and termination methods.
  TAO_UIOP_Connector (void);
  // Constructor.
  // @@ Do we want to pass in the tag here or should it be statically
  // defined?

  int open (TAO_Resource_Factory *trf, ACE_Reactor *reactor);
  //  Initialize object and register with reactor.

  int close (void);
  // Shutdown Connector bridge and concreate Connector.

  int preconnect (char *preconnections);
  // Initial set of connections to be established.

  int connect (TAO_Profile *profile,
               TAO_Transport *&transport);
  // Connect will be called from TAO_GIOP_Invocation::start

protected:
  virtual int make_profile (const char *endpoint,
                            TAO_Profile *&,
                            CORBA::Environment &ACE_TRY_ENV);
  // Create a profile with a given endpoint.

  virtual int check_prefix (const char *endpoint);
  // Check that the prefix of the provided endpoint is valid for use
  // with a given pluggable protocol.

private:
  TAO_UIOP_BASE_CONNECTOR base_connector_;
  // The connector initiating connection requests for UIOP.

  static TAO_UIOP_CACHED_CONNECT_STRATEGY UIOP_Cached_Connect_Strategy_;
  static TAO_UIOP_NULL_CREATION_STRATEGY UIOP_Null_Creation_Strategy_;
  static TAO_UIOP_NULL_ACTIVATION_STRATEGY UIOP_Null_Activation_Strategy_;
  // @@ Need to provide our own strategies until the Resource_Factory is
  //    updated for pluggable protocols.
};

# endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#endif  /* TAO_UIOP_CONNECTOR_H */
