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

#include "ace/Connector.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "ace/LSOCK_Connector.h"
#include "tao/Pluggable.h"
#include "tao/UIOP_Connect.h"
#include "tao/Resource_Factory.h"

#if defined(__GNUC__) && __GNUC__ == 2 && __GNUC_MINOR__ < 8
#define ACE_HAS_BROKEN_EXTENDED_TEMPLATES
#endif /* __GNUC__ */

#include "ace/Cached_Connect_Strategy_T.h"

typedef ACE_Strategy_Connector<TAO_UIOP_Client_Connection_Handler,
                               ACE_LSOCK_CONNECTOR>
        TAO_UIOP_BASE_CONNECTOR;

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
                                      TAO_ORB_Core* orb_core = 0);

  virtual int make_svc_handler (TAO_UIOP_Client_Connection_Handler *&sh);
  // Makes TAO_UIOP_Client_Connection_Handlers

private:
  TAO_ORB_Core* orb_core_;
  // The ORB
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
  TAO_UIOP_Connector (void);
  // Constructor.
  // @@ Do we want to pass in the tag here or should it be statically
  // defined?

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

  virtual int purge_connections (void);
  // Purge "old" connections.

protected:
  // = More TAO_Connector methods, please check the documentation on
  //   Pluggable.h
  virtual void make_profile (const char *endpoint,
                             TAO_Profile *&,
                             CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual int make_caching_strategy (void);
  // According to the option specified, create the appropriate caching
  // strategy used for purging unused connections from the connection
  // cache.

public:

  // = Connection Caching Strategy.
  typedef size_t ATTRIBUTES;
  typedef ACE_Pair<TAO_UIOP_Client_Connection_Handler *,
                   ATTRIBUTES>
          CACHED_HANDLER;
  typedef ACE_Refcounted_Hash_Recyclable<ACE_UNIX_Addr>
          ADDR;
  typedef ACE_Hash<ADDR> HASH_KEY;
  typedef ACE_Equal_To<ADDR> COMPARE_KEYS;

  typedef ACE_Hash_Map_Manager_Ex<ADDR,
                                  CACHED_HANDLER,
                                  HASH_KEY,
                                  COMPARE_KEYS,
                                  ACE_Null_Mutex>
          HASH_MAP;
  typedef ACE_Hash_Map_Iterator_Ex<ADDR,
                                   CACHED_HANDLER,
                                   HASH_KEY,
                                   COMPARE_KEYS,
                                   ACE_Null_Mutex>
          HASH_MAP_ITERATOR;
  typedef ACE_Hash_Map_Reverse_Iterator_Ex<ADDR,
                                           CACHED_HANDLER,
                                           HASH_KEY,
                                           COMPARE_KEYS,
                                           ACE_Null_Mutex>
          HASH_MAP_REVERSE_ITERATOR;

  typedef ACE_Recyclable_Handler_Caching_Utility<ADDR,
                                                 CACHED_HANDLER,
                                                 HASH_MAP,
                                                 HASH_MAP_ITERATOR,
                                                 ATTRIBUTES>
          CACHING_UTILITY;

#if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)
  typedef ACE_LRU_Caching_Strategy<ATTRIBUTES,
                                   CACHING_UTILITY>
          CACHING_STRATEGY;
#else
  typedef ACE_Caching_Strategy<ATTRIBUTES,
                               CACHING_UTILITY>
          CACHING_STRATEGY;
#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

  typedef ACE_Cached_Connect_Strategy_Ex<TAO_UIOP_Client_Connection_Handler,
                                         ACE_LSOCK_CONNECTOR,
                                         CACHING_STRATEGY,
                                         ATTRIBUTES,
                                         TAO_Cached_Connector_Lock>
          CACHED_CONNECT_STRATEGY;

  typedef ACE_NOOP_Creation_Strategy<TAO_UIOP_Client_Connection_Handler>
        TAO_NULL_CREATION_STRATEGY;

  typedef ACE_NOOP_Concurrency_Strategy<TAO_UIOP_Client_Connection_Handler>
        TAO_NULL_ACTIVATION_STRATEGY;

private:
  TAO_NULL_CREATION_STRATEGY null_creation_strategy_;
  TAO_NULL_ACTIVATION_STRATEGY null_activation_strategy_;

  TAO_UIOP_BASE_CONNECTOR base_connector_;
  // The connector initiating connection requests for UIOP.

  TAO_ORB_Core *orb_core_;
  // ORB Core.

  CACHED_CONNECT_STRATEGY *cached_connect_strategy_;
  // Cached connect strategy.

  CACHING_STRATEGY *caching_strategy_;
  // Caching strategy which decides the order of removal of entries
  // from the connection cache.
};

# endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#endif  /* TAO_UIOP_CONNECTOR_H */
