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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Connector.h"
#include "tao/Pluggable.h"
#include "tao/IIOP_Connect.h"
#include "tao/Resource_Factory.h"

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
#if defined(__GNUC__) && __GNUC__ == 2 && __GNUC_MINOR__ < 8
#define ACE_HAS_BROKEN_EXTENDED_TEMPLATES
#endif /* __GNUC__ */

#include "ace/Cached_Connect_Strategy_T.h"
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

typedef ACE_Strategy_Connector<TAO_IIOP_Client_Connection_Handler,
                               ACE_SOCK_CONNECTOR>
        TAO_IIOP_BASE_CONNECTOR;

// ****************************************************************

class TAO_Export TAO_IIOP_Connect_Creation_Strategy : public ACE_Creation_Strategy<TAO_IIOP_Client_Connection_Handler>
{
  // = TITLE
  //   Helper creation strategy
  //
  // = DESCRIPTION
  //   Creates UIOP_Client_Connection_Handler objects but satisfies
  //   the interface required by the
  //   ACE_Creation_Strategy<TAO_IIOP_Client_Connection_Handler>
  //
public:
  TAO_IIOP_Connect_Creation_Strategy (ACE_Thread_Manager * = 0,
                                      TAO_ORB_Core* orb_core = 0);

  virtual int make_svc_handler (TAO_IIOP_Client_Connection_Handler *&sh);
  // Makes TAO_IIOP_Client_Connection_Handlers

private:
  TAO_ORB_Core* orb_core_;
  // The ORB
};

// ****************************************************************

class TAO_Export TAO_IIOP_Connector : public TAO_Connector
{
  // = TITLE
  //   IIOP-specific Connector bridge for pluggable protocols.
  //
  // = DESCRIPTION
  //   Concrete instance of the TAO_Connector class.  Responsible
  //   for establishing a connection with a server and is called from the
  //   Connector_Registory.
  //
public:
  // = Initialization and termination methods.
  TAO_IIOP_Connector (CORBA::Boolean flag);
  // Constructor.

  // = The TAO_Connector methods, please check the documentation on
  // Pluggable.h
  int open (TAO_ORB_Core *orb_core);
  int close (void);
  int connect (TAO_Profile *profile,
               TAO_Transport *&transport,
               ACE_Time_Value *max_wait_time);
  int preconnect (const char *preconnections);
  TAO_Profile *create_profile (TAO_InputCDR& cdr);

  virtual int check_prefix (const char *endpoint);

  virtual char object_key_delimiter (void) const;

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  virtual int purge_connections (void);
  // Purge "old" connections.
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

protected:
  // = More TAO_Connector methods, please check the documentation on
  //   Pluggable.h
  virtual void make_profile (const char *endpoint,
                             TAO_Profile *&,
                             CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  virtual int make_caching_strategy (void);
  // According to the option specified, create the appropriate caching
  // strategy used for purging unused connections from the connection
  // cache.
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

public:

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  // = Connection Caching Strategy.
  typedef size_t TAO_ATTRIBUTES;
  typedef ACE_Pair<TAO_IIOP_Client_Connection_Handler *,
                   TAO_ATTRIBUTES>
          TAO_CACHED_HANDLER;
  typedef ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
          TAO_IADDR;
  typedef ACE_Hash<TAO_IADDR> TAO_HASH_KEY;
  typedef ACE_Equal_To<TAO_IADDR> TAO_COMPARE_KEYS;

  typedef ACE_Hash_Map_Manager_Ex<TAO_IADDR,
                                  TAO_CACHED_HANDLER,
                                  TAO_HASH_KEY,
                                  TAO_COMPARE_KEYS,
                                  ACE_Null_Mutex>
          TAO_HASH_MAP;
  typedef ACE_Hash_Map_Iterator_Ex<TAO_IADDR,
                                   TAO_CACHED_HANDLER,
                                   TAO_HASH_KEY,
                                   TAO_COMPARE_KEYS,
                                   ACE_Null_Mutex>
          TAO_HASH_MAP_ITERATOR;
  typedef ACE_Hash_Map_Reverse_Iterator_Ex<TAO_IADDR,
                                           TAO_CACHED_HANDLER,
                                           TAO_HASH_KEY,
                                           TAO_COMPARE_KEYS,
                                           ACE_Null_Mutex>
          TAO_HASH_MAP_REVERSE_ITERATOR;

  typedef ACE_Refcounted_Recyclable_Handler_Caching_Utility<TAO_IADDR,
                                                            TAO_CACHED_HANDLER,
                                                            TAO_HASH_MAP,
                                                            TAO_HASH_MAP_ITERATOR,
                                                            TAO_ATTRIBUTES>
          TAO_CACHING_UTILITY;

#if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)
  typedef ACE_LRU_Caching_Strategy<TAO_ATTRIBUTES,
                                   TAO_CACHING_UTILITY>
          TAO_CACHING_STRATEGY;
#else
  typedef ACE_Caching_Strategy<TAO_ATTRIBUTES,
                               TAO_CACHING_UTILITY>
          TAO_CACHING_STRATEGY;
#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

  typedef ACE_Cached_Connect_Strategy_Ex<TAO_IIOP_Client_Connection_Handler,
                                         ACE_SOCK_CONNECTOR,
                                         TAO_CACHING_STRATEGY,
                                         TAO_ATTRIBUTES,
                                         TAO_Cached_Connector_Lock>
          TAO_CACHED_CONNECT_STRATEGY;
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

  typedef ACE_NOOP_Concurrency_Strategy<TAO_IIOP_Client_Connection_Handler>
        TAO_NULL_ACTIVATION_STRATEGY;

  typedef ACE_NOOP_Creation_Strategy<TAO_IIOP_Client_Connection_Handler>
        TAO_NULL_CREATION_STRATEGY;

protected:
  TAO_ORB_Core *orb_core_;
  // ORB Core.

private:
  TAO_NULL_ACTIVATION_STRATEGY null_activation_strategy_;
  TAO_NULL_CREATION_STRATEGY null_creation_strategy_;

  TAO_IIOP_BASE_CONNECTOR base_connector_;
  // The connector initiating connection requests for IIOP.

  CORBA::Boolean lite_flag_;
  // Do we need to use a GIOP_Lite for sending messages?

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  TAO_CACHED_CONNECT_STRATEGY *cached_connect_strategy_;
  // Cached connect strategy.

  TAO_CACHING_STRATEGY *caching_strategy_;
  // Caching strategy which decides the order of removal of entries
  // from the connection cache.
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */


};

#endif  /* TAO_IIOP_CONNECTOR_H */
