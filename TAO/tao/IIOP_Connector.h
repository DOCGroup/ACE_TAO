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

#if defined(__GNUC__) && __GNUC__ == 2 && __GNUC_MINOR__ < 8
#define ACE_HAS_BROKEN_EXTENDED_TEMPLATES
#endif /* __GNUC__ */

#include "ace/Cached_Connect_Strategy_T.h"

typedef ACE_Strategy_Connector<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>
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
  TAO_IIOP_Connector (void);
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

  // = Connection Caching Strategy. Used with TAO_IIOP_Connector.
  typedef size_t ATTRIBUTES;
  typedef ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
          IIOP_ADDR;
  typedef ACE_Pair<TAO_IIOP_Client_Connection_Handler *, ATTRIBUTES> 
          IIOP_CACHED_HANDLER;
  typedef ACE_Hash<IIOP_ADDR> IIOP_HASH_KEY;
  typedef ACE_Equal_To<IIOP_ADDR> IIOP_CMP_KEYS;
  typedef ACE_Hash_Map_Manager_Ex<IIOP_ADDR, IIOP_CACHED_HANDLER, IIOP_HASH_KEY, IIOP_CMP_KEYS, ACE_Null_Mutex> 
          IIOP_HASH_MAP;
  typedef ACE_Hash_Map_Iterator_Ex<IIOP_ADDR, IIOP_CACHED_HANDLER, IIOP_HASH_KEY, IIOP_CMP_KEYS, ACE_Null_Mutex> 
          IIOP_HASH_ITERATOR;
  typedef ACE_Hash_Map_Reverse_Iterator_Ex<IIOP_ADDR, IIOP_CACHED_HANDLER, IIOP_HASH_KEY, IIOP_CMP_KEYS, ACE_Null_Mutex> 
          IIOP_HASH_REVERSE_ITERATOR;
  typedef ACE_Recyclable_Handler_Cleanup_Strategy<IIOP_ADDR, IIOP_CACHED_HANDLER, IIOP_HASH_MAP>
          IIOP_CLEANUP_STRATEGY;
  typedef ACE_Recyclable_Handler_Caching_Utility<IIOP_ADDR, IIOP_CACHED_HANDLER, IIOP_HASH_MAP, IIOP_HASH_ITERATOR, ATTRIBUTES>
          IIOP_CACHING_UTILITY;
#if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)
  typedef ACE_LRU_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>
          IIOP_CACHING_STRATEGY;
#else
  typedef ACE_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>
          IIOP_CACHING_STRATEGY;
#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */
  typedef ACE_Cached_Connect_Strategy_Ex<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR, IIOP_CACHING_STRATEGY, ATTRIBUTES, TAO_Cached_Connector_Lock>
          TAO_CACHED_CONNECT_STRATEGY;

  virtual TAO_CACHED_CONNECT_STRATEGY &cached_connect_strategy (void);
  // Accessor to the connect strategy.

protected:
  // = More TAO_Connector methods, please check the documentation on
  //   Pluggable.h
  virtual void make_profile (const char *endpoint,
                             TAO_Profile *&,
                             CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual int make_connection_caching_strategy (void);  
  // According to the option specified, create the appropriate caching strategy used for purging
  // unused connections from the connection cache.
protected:
  typedef ACE_NOOP_Concurrency_Strategy<TAO_IIOP_Client_Connection_Handler>
        TAO_NULL_ACTIVATION_STRATEGY;

  typedef ACE_NOOP_Creation_Strategy<TAO_IIOP_Client_Connection_Handler>
        TAO_NULL_CREATION_STRATEGY;

  enum
  {
    TAO_LRU, // Use Least Recently Used caching strategy
    TAO_LFU, // Use Least Frequently Used caching strategy
    TAO_FIFO, // Use First In First Out caching strategy
    TAO_NULL // Dont use any strategy.
  };

private:
  TAO_NULL_ACTIVATION_STRATEGY null_activation_strategy_;
  TAO_NULL_CREATION_STRATEGY null_creation_strategy_;

  TAO_CACHED_CONNECT_STRATEGY *cached_connect_strategy_;
  // This is the connection strategy.

  IIOP_CACHING_STRATEGY *caching_strategy_;
  // This is the caching strategy which decides the order of removal
  // of entries from the connection cache. This can be decided at
  // run-time by using the -ORBConnectionCachingStrategy option. The
  // choices are: Least Recently Used, Least Frequently Used, First In
  // First Out, and Null (which does nothing). By default, LRU is used.
  
  TAO_IIOP_BASE_CONNECTOR  base_connector_;
  // The connector initiating connection requests for IIOP.

  TAO_ORB_Core *orb_core_;
  // ORB Core.
};

#endif  /* TAO_IIOP_CONNECTOR_H */
