// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    SHMIOP_Connector.h
//
// = DESCRIPTION
//    IIOP specific connector processing
//
// = AUTHOR
//    Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_SHMIOP_CONNECTOR_H
#define TAO_SHMIOP_CONNECTOR_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "ace/Connector.h"
#include "ace/MEM_Connector.h"
#include "tao/Pluggable.h"
#include "SHMIOP_Connection_Handler.h"
#include "tao/Resource_Factory.h"
#include "tao/Connector_Impl.h"

// ****************************************************************

class TAO_Strategies_Export TAO_SHMIOP_Connector : public TAO_Connector
{
  // = TITLE
  //   SHMIOP-specific Connector bridge for pluggable protocols.
  //
  // = DESCRIPTION
  //   Concrete instance of the TAO_Connector class.  Responsible
  //   for establishing a connection with a server and is called from the
  //   Connector_Registory.
  //
public:
  // = Initialization and termination methods.
  TAO_SHMIOP_Connector (CORBA::Boolean flag = 0);
  // Constructor.

  ~TAO_SHMIOP_Connector (void);
  // Default destructor

  // = The TAO_Connector methods, please check the documentation on
  // Pluggable.h
  int open (TAO_ORB_Core *orb_core);
  int close (void);
  int connect (TAO_Connection_Descriptor_Interface *desc,
               TAO_Transport *&transport,
               ACE_Time_Value *max_wait_time,
               CORBA::Environment &ACE_TRY_ENV);
  int preconnect (const char *preconnections);
  TAO_Profile *create_profile (TAO_InputCDR& cdr);

  virtual int check_prefix (const char *endpoint);

  virtual char object_key_delimiter (void) const;

protected:
  // = More TAO_Connector methods, please check the documentation on
  //   Pluggable.h
  virtual void make_profile (const char *endpoint,
                             TAO_Profile *&,
                             CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

public:

  typedef TAO_Connect_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>
          TAO_SHMIOP_CONNECT_CONCURRENCY_STRATEGY;

  typedef TAO_Connect_Creation_Strategy<TAO_SHMIOP_Connection_Handler>
          TAO_SHMIOP_CONNECT_CREATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_SHMIOP_Connection_Handler,
                               ACE_MEM_CONNECTOR>
          TAO_SHMIOP_CONNECT_STRATEGY ;

  typedef ACE_Strategy_Connector<TAO_SHMIOP_Connection_Handler,
                                 ACE_MEM_CONNECTOR>
          TAO_SHMIOP_BASE_CONNECTOR;
private:

  ACE_MEM_Addr address_;
  // local address

  TAO_SHMIOP_CONNECT_STRATEGY connect_strategy_;
  // Our connect strategy

  TAO_SHMIOP_BASE_CONNECTOR base_connector_;
  // The connector initiating connection requests for SHMIOP.

  CORBA::Boolean lite_flag_;
  // Are we using GIOP lite??
};

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include "ace/post.h"
#endif  /* TAO_SHMIOP_CONNECTOR_H */
