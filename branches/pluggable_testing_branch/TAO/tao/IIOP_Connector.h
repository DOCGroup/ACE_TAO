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
  //   IIOP-specific Connector bridge for pluggable protocols.
  //
  // = DESCRIPTION
  //   @@ Fred, please fill in here.
public:
  // = Initialization and termination methods.
  TAO_IIOP_Connector (void);
  // Constructor.

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
  TAO_IIOP_BASE_CONNECTOR  base_connector_;
  // The connector initiating connection requests for IIOP.
};

#endif  /* TAO_IIOP_CONNECTOR_H */
