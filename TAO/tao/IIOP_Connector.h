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
  //
public:
  // = Initialization and termination methods.
  TAO_IIOP_Connector (void);
  // Constructor.

  // = The TAO_Connector methods, please check the documentation on
  // Pluggable.h
  int open (TAO_ORB_Core *orb_core);
  int close (void);
  int connect (TAO_Profile *profile, TAO_Transport *&transport);
  int preconnect (const char *preconnections);
  TAO_Profile *create_profile (TAO_InputCDR& cdr);

  typedef ACE_NOOP_Concurrency_Strategy<TAO_Client_Connection_Handler>
        TAO_NULL_ACTIVATION_STRATEGY;

  typedef ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>
        TAO_NULL_CREATION_STRATEGY;

protected:
  // = More TAO_Connector methods, please check the documentation on
  // Pluggable.h
  virtual int make_profile (const char *endpoint,
                            TAO_Profile *&,
                            CORBA::Environment &ACE_TRY_ENV);

  virtual int check_prefix (const char *endpoint);

  virtual const char object_key_delimiter (void) const;

private:
  TAO_NULL_ACTIVATION_STRATEGY null_activation_strategy_;
  TAO_NULL_CREATION_STRATEGY null_creation_strategy_;

  TAO_IIOP_BASE_CONNECTOR  base_connector_;
  // The connector initiating connection requests for IIOP.
};

#endif  /* TAO_IIOP_CONNECTOR_H */
