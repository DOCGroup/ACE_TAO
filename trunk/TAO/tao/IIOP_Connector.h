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
//    Fred Kuhns
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
  public:

  TAO_IIOP_Connector (void);
  // Constructor.  Do we want to pass in the tag here or
  // should it be statically defined?

  int preconnect(char* preconnections);
  int open(TAO_Resource_Factory *trf, ACE_Reactor *reactor);
  int close(void);
  CORBA::ULong tag (void);

  TAO_Transport* connect(TAO_Profile *profile,
                         CORBA::Environment &env);
  // connect will be called from TAO_GIOP_Invocation::start

private:

  CORBA::ULong tag_;
  // IIOP tag.

  TAO_IIOP_BASE_CONNECTOR  base_connector_;
  // The connector initiating connection requests for IIOP.

};

#endif  /* TAO_IIOP_CONNECTOR_H */
