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
#include "ace/pre.h"

#include "tao/Pluggable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if TAO_HAS_UIOP == 1

#include "ace/Connector.h"
#include "ace/LSOCK_Connector.h"
#include "tao/UIOP_Connect.h"
#include "tao/Resource_Factory.h"



// ****************************************************************
class TAO_Export TAO_UIOP_Connect_Creation_Strategy : public ACE_Creation_Strategy<TAO_UIOP_Client_Connection_Handler>
{
  // = TITLE
  //   Helper creation strategy
  //
  // = DESCRIPTION
  //   Creates UIOP_Client_Connection_Handler objects but satisfies
  //   the interface required by the
  //   ACE_Creation_Strategy<TAO_UIOP_Client_Connection_Handler>
  //
public:
  TAO_UIOP_Connect_Creation_Strategy (ACE_Thread_Manager * = 0,
                                      TAO_ORB_Core* orb_core = 0,
                                      void *arg = 0,
                                      CORBA::Boolean flag = 0);
  // Constructor. <arg> parameter is used to pass any special
  // state/info to the service handler upon creation.  Currently used
  // by IIOP and UIOP to pass protocol configuration properties.

  virtual int make_svc_handler (TAO_UIOP_Client_Connection_Handler *&sh);
  // Makes TAO_UIOP_Client_Connection_Handlers

private:
  TAO_ORB_Core* orb_core_;
  // The ORB

  void *arg_;
  // Some info/state to be passed to the service handler we create.

  CORBA::Boolean lite_flag_;
  // Are we using lite?
};

// ****************************************************************

class TAO_Export TAO_UIOP_Connector : public TAO_Connector
{
  // = TITLE
  //   UIOP-specific Connector bridge for pluggable protocols.
  //
  // = DESCRIPTION
  //
public:
  // = Initialization and termination methods.
  TAO_UIOP_Connector (CORBA::Boolean flag = 0);
  // Constructor.
  // @@ Do we want to pass in the tag here or should it be statically
  // defined?

  // = The TAO_Connector methods, please check the documentation on
  // Pluggable.h
  int open (TAO_ORB_Core *orb_core);
  int close (void);
  int connect (TAO_Base_Connection_Property *prop,
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

  int init_uiop_properties (void);
  // Obtains uiop properties that must be used by this connector, i.e.,
  // initializes <uiop_properties_>.

public:

  typedef ACE_NOOP_Concurrency_Strategy<TAO_UIOP_Client_Connection_Handler>
          TAO_NULL_ACTIVATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_UIOP_Client_Connection_Handler,
                               ACE_LSOCK_CONNECTOR>
          TAO_CONNECT_STRATEGY ;

  typedef ACE_Strategy_Connector<TAO_UIOP_Client_Connection_Handler,
                               ACE_LSOCK_CONNECTOR>
          TAO_UIOP_BASE_CONNECTOR;

private:

  TAO_NULL_ACTIVATION_STRATEGY null_activation_strategy_;
  // Our activation strategy

  TAO_CONNECT_STRATEGY connect_strategy_;
  // Our connect strategy

  TAO_UIOP_BASE_CONNECTOR base_connector_;
  // The connector initiating connection requests for UIOP.

  CORBA::Boolean lite_flag_;
  // Do we need to use a GIOP_Lite for sending messages?

  TAO_UIOP_Properties uiop_properties_;
  // UIOP configuration properties for all connections established by
  // this connector.
};

# endif  /* TAO_HAS_UIOP == 1 */

#include "ace/post.h"
#endif  /* TAO_UIOP_CONNECTOR_H */
