// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO_SSLIOP
//
// = FILENAME
//     SSLIOP_Connector.h
//
// = DESCRIPTION
//     SSLIOP specific connector processing
//
// = AUTHOR
//     Carlos O'Ryan <coryan@ece.uci.edu>
//     Ossama Othman <ossama@ece.uci.edu>
//
// ============================================================================

#ifndef TAO_SSLIOP_CONNECTOR_H
#define TAO_SSLIOP_CONNECTOR_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SSL/SSL_SOCK_Connector.h"
#include "IIOP_SSL_Connector.h"
#include "SSLIOP_Connection_Handler.h"
#include "tao/Resource_Factory.h"


class TAO_Base_Transport_Property;

class TAO_SSLIOP_Export TAO_SSLIOP_Connector : public TAO_IIOP_SSL_Connector
{
  // = TITLE
  //   IIOP-specific Connector bridge for pluggable protocols.
  //
  // = DESCRIPTION
  //   Concrete instance of the TAO_Connector class.  Responsible
  //   for establishing a connection with a server and is called from
  //   the Connector_Registry.
  //
public:
  // = Initialization and termination methods.
  TAO_SSLIOP_Connector (int no_protection);
  // Constructor.

  // = The TAO_Connector methods, please check the documentation on
  // Pluggable.h
  virtual int open (TAO_ORB_Core *orb_core);
  virtual int close (void);
  virtual int connect (TAO_Transport_Descriptor_Interface *desc,
                       TAO_Transport *&transport,
                       ACE_Time_Value *max_wait_time,
                       CORBA::Environment &ACE_TRY_ENV);
  virtual TAO_Profile *create_profile (TAO_InputCDR& cdr);


protected:
  // = More TAO_Connector methods, please check the documentation on
  //   Pluggable.h
  virtual void make_profile (const char *endpoint,
                             TAO_Profile *&,
                             CORBA::Environment &ACE_TRY_ENV =
                               TAO_default_environment ());

public:


  typedef TAO_Connect_Concurrency_Strategy<TAO_SSLIOP_Connection_Handler>
          TAO_SSLIOP_CONNECT_CONCURRENCY_STRATEGY;

  typedef TAO_Connect_Creation_Strategy<TAO_SSLIOP_Connection_Handler>
          TAO_SSLIOP_CONNECT_CREATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_SSLIOP_Connection_Handler,
                               ACE_SSL_SOCK_CONNECTOR>
          TAO_SSLIOP_CONNECT_STRATEGY ;

  typedef ACE_Strategy_Connector<TAO_SSLIOP_Connection_Handler,
                                 ACE_SSL_SOCK_CONNECTOR>
          TAO_SSLIOP_BASE_CONNECTOR;

private:

  int no_protection_;
  // If zero, connect to IIOP over SSL port by default.
  // Otherwise, connect to the insecure IIOP port.

  /// Our connect strategy
  TAO_SSLIOP_CONNECT_STRATEGY connect_strategy_;

  /// The connector initiating connection requests for IIOP.
  TAO_SSLIOP_BASE_CONNECTOR base_connector_;
};


#include "ace/post.h"

#endif  /* TAO_SSLIOP_CONNECTOR_H */
