// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "SSLIOP_Connector.h"
#include "SSLIOP_Profile.h"
#include "tao/GIOP.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Environment.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(tao, SSLIOP_Connector, "$Id$")

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
int
TAO_SSLIOP_Connector::purge_connections (void)
{
  return this->cached_connect_strategy_->purge_connections ();
}

#define TAO_HANDLER TAO_SSLIOP_Client_Connection_Handler
#define TAO_SVC_TUPLE ACE_Svc_Tuple<TAO_HANDLER>
#define TAO_ADDR TAO_SSLIOP_Connector::TAO_IADDR
#define TAO_HASH_KEY TAO_SSLIOP_Connector::TAO_HASH_KEY
#define TAO_COMPARE_KEYS TAO_SSLIOP_Connector::TAO_COMPARE_KEYS
#define TAO_ATTRIBUTES TAO_SSLIOP_Connector::TAO_ATTRIBUTES
#define TAO_CACHED_HANDLER TAO_SSLIOP_Connector::TAO_CACHED_HANDLER
#define TAO_HASH_MAP TAO_SSLIOP_Connector::TAO_HASH_MAP
#define TAO_HASH_MAP_ITERATOR TAO_SSLIOP_Connector::TAO_HASH_MAP_ITERATOR
#define TAO_HASH_MAP_REVERSE_ITERATOR TAO_SSLIOP_Connector::TAO_HASH_MAP_REVERSE_ITERATOR
#define TAO_CACHING_UTILITY TAO_SSLIOP_Connector::TAO_CACHING_UTILITY
#define TAO_CACHING_STRATEGY TAO_SSLIOP_Connector::TAO_CACHING_STRATEGY
#define TAO_CACHED_CONNECT_STRATEGY TAO_SSLIOP_Connector::TAO_CACHED_CONNECT_STRATEGY

typedef ACE_LRU_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
        TAO_SSLIOP_LRU_CACHING_STRATEGY;

#if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)
typedef TAO_SSLIOP_LRU_CACHING_STRATEGY
        TAO_SSLIOP_CACHING_STRATEGY;
#else
typedef ACE_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
        TAO_SSLIOP_CACHING_STRATEGY;
typedef ACE_LFU_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
        TAO_SSLIOP_LFU_CACHING_STRATEGY;
typedef ACE_FIFO_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
        TAO_SSLIOP_FIFO_CACHING_STRATEGY;
typedef ACE_Null_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
        TAO_SSLIOP_NULL_CACHING_STRATEGY;
typedef ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_LRU_CACHING_STRATEGY>
        TAO_SSLIOP_LRU_CACHING_STRATEGY_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_LFU_CACHING_STRATEGY>
        TAO_SSLIOP_LFU_CACHING_STRATEGY_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_FIFO_CACHING_STRATEGY>
        TAO_SSLIOP_FIFO_CACHING_STRATEGY_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_NULL_CACHING_STRATEGY>
        TAO_SSLIOP_NULL_CACHING_STRATEGY_ADAPTER;
#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

int
TAO_SSLIOP_Connector::make_caching_strategy (void)
{
  if (this->TAO_IIOP_Connector::make_caching_strategy () == -1)
    return -1;

  TAO_Resource_Factory *resource_factory =
    this->orb_core_->resource_factory ();

#if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)
  ACE_NEW_RETURN (this->caching_strategy_,
                  TAO_SSLIOP_CACHING_STRATEGY,
                  -1);
#else
  switch (resource_factory->connection_caching_strategy_type ())
    {
    case TAO_Resource_Factory::NOOP:
      ACE_NEW_RETURN (this->caching_strategy_,
                      TAO_SSLIOP_NULL_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;

    default:
    case TAO_Resource_Factory::LRU:
      ACE_NEW_RETURN (this->caching_strategy_,
                      TAO_SSLIOP_LRU_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;

    case TAO_Resource_Factory::LFU:
      ACE_NEW_RETURN (this->caching_strategy_,
                      TAO_SSLIOP_LFU_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;

    case TAO_Resource_Factory::FIFO:
      ACE_NEW_RETURN (this->caching_strategy_,
                      TAO_SSLIOP_FIFO_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;
    }

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

  this->caching_strategy_->purge_percent (resource_factory->purge_percentage ());
  return 0;
}
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

// The TAO_Cached_Connector_Lock template instantiations are in
// Resource_Factory.cpp.

#if !defined (TAO_USES_ROBUST_CONNECTION_MGMT)
#define TAO_SVC_TUPLE ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler>
#define CACHED_CONNECT_STRATEGY ACE_Cached_Connect_Strategy<TAO_SSLIOP_Client_Connection_Handler, ACE_SSL_SOCK_CONNECTOR, TAO_Cached_Connector_Lock>
#define TAO_ADDR ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
#define TAO_HANDLER TAO_SSLIOP_Client_Connection_Handler
#define TAO_HASH_KEY ACE_Hash<TAO_ADDR>
#define TAO_COMPARE_KEYS ACE_Equal_To<TAO_ADDR>
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) || \
    defined (ACE_HAS_GNU_REPO)

template class ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr>;
template class ACE_Auto_Basic_Array_Ptr<TAO_SSLIOP_Client_Connection_Handler*>;
template class auto_ptr<TAO_SSLIOP_Connect_Creation_Strategy>;
template class ACE_Auto_Basic_Ptr<TAO_SSLIOP_Connect_Creation_Strategy>;

template class ACE_Node<ACE_INET_Addr>;
template class ACE_Unbounded_Stack<ACE_INET_Addr>;
template class ACE_Unbounded_Stack_Iterator<ACE_INET_Addr>;

#if !defined (TAO_USES_ROBUST_CONNECTION_MGMT)
template class CACHED_CONNECT_STRATEGY;
template class TAO_ADDR;
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>;
template class ACE_NOOP_Creation_Strategy<TAO_HANDLER>;
template class ACE_Concurrency_Strategy<TAO_HANDLER>;
template class ACE_Connect_Strategy<TAO_HANDLER, ACE_SSL_SOCK_CONNECTOR>;
template class ACE_Connector<TAO_HANDLER, ACE_SSL_SOCK_CONNECTOR>;
template class ACE_Creation_Strategy<TAO_HANDLER>;
template class ACE_Hash_Map_Entry<TAO_ADDR, TAO_HANDLER *>;
template class ACE_Hash<ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr> >;
template class ACE_Equal_To<ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr> >;
template class ACE_Map_Entry<ACE_HANDLE, TAO_SVC_TUPLE *>;
template class ACE_Map_Manager<ACE_HANDLE, TAO_SVC_TUPLE *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<ACE_HANDLE, TAO_SVC_TUPLE *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<ACE_HANDLE, TAO_SVC_TUPLE *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE, TAO_SVC_TUPLE *, ACE_SYNCH_RW_MUTEX>;
template class ACE_NOOP_Concurrency_Strategy<TAO_HANDLER>;
template class ACE_Recycling_Strategy<TAO_HANDLER>;
template class ACE_Strategy_Connector<TAO_HANDLER, ACE_SSL_SOCK_CONNECTOR>;
template class TAO_SVC_TUPLE;

template class ACE_Hash_Map_Manager<TAO_ADDR, TAO_HANDLER *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_ADDR, TAO_HANDLER *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_ADDR, TAO_HANDLER *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Bucket_Iterator<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>;

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
template class ACE_Pair<TAO_HANDLER *, TAO_ATTRIBUTES>;
template class ACE_Reference_Pair<TAO_ADDR, TAO_HANDLER *>;
template class ACE_Hash_Map_Entry<TAO_ADDR, TAO_CACHED_HANDLER>;

template class ACE_Hash_Map_Manager<TAO_ADDR, TAO_CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_ADDR, TAO_CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_ADDR, TAO_CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Bucket_Iterator<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>;

// = Caching_Strategy
template class ACE_Hash_Cache_Map_Manager<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>;

template class ACE_LRU_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>;

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

template class ACE_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>;
template class ACE_LFU_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>;
template class ACE_FIFO_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>;
template class ACE_Null_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>;

template class ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_LRU_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_LFU_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_FIFO_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_NULL_CACHING_STRATEGY>;

template class ACE_Cache_Map_Manager<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP, TAO_HASH_MAP_ITERATOR, TAO_HASH_MAP_REVERSE_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>;
template class ACE_Cache_Map_Iterator<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>;
template class ACE_Cache_Map_Reverse_Iterator<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP_REVERSE_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>;

#else

template class ACE_Cache_Map_Manager<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>;

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

template class ACE_Cached_Connect_Strategy_Ex<TAO_HANDLER, ACE_SSL_SOCK_CONNECTOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES, TAO_Cached_Connector_Lock>;
template class ACE_Cached_Connect_Strategy<TAO_HANDLER, ACE_SSL_SOCK_CONNECTOR, TAO_Cached_Connector_Lock>;

template class ACE_Cleanup_Strategy<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP>;
template class ACE_Refcounted_Recyclable_Handler_Cleanup_Strategy<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP>;
template class ACE_Refcounted_Recyclable_Handler_Caching_Utility<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP, TAO_HASH_MAP_ITERATOR, TAO_ATTRIBUTES>;
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<TAO_SSLIOP_Client_Connection_Handler*>
#pragma instantiate auto_ptr<TAO_SSLIOP_Connect_Creation_Strategy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_SSLIOP_Connect_Creation_Strategy>

#pragma instantiate ACE_Node<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Stack<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Stack_Iterator<ACE_INET_Addr>

#if !defined (TAO_USES_ROBUST_CONNECTION_MGMT)
#pragma instantiate CACHED_CONNECT_STRATEGY;
#pragma instantiate TAO_ADDR;
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
#pragma instantiate ACE_NOOP_Creation_Strategy<TAO_HANDLER>
#pragma instantiate ACE_Concurrency_Strategy<TAO_HANDLER>
#pragma instantiate ACE_Connect_Strategy<TAO_HANDLER, ACE_SSL_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector<TAO_HANDLER, ACE_SSL_SOCK_CONNECTOR>
#pragma instantiate ACE_Creation_Strategy<TAO_HANDLER>
#pragma instantiate ACE_Hash_Map_Entry<TAO_ADDR, TAO_HANDLER *>
#pragma instantiate ACE_Hash<ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr> >
#pragma instantiate ACE_Equal_To<ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr> >
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, TAO_SVC_TUPLE *>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, TAO_SVC_TUPLE *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE, TAO_SVC_TUPLE *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, TAO_SVC_TUPLE *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE, TAO_SVC_TUPLE *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_NOOP_Concurrency_Strategy<TAO_HANDLER>
#pragma instantiate ACE_Recycling_Strategy<TAO_HANDLER>
#pragma instantiate ACE_Strategy_Connector<TAO_HANDLER, ACE_SSL_SOCK_CONNECTOR>
#pragma instantiate TAO_SVC_TUPLE

#pragma instantiate ACE_Hash_Map_Manager<TAO_ADDR, TAO_HANDLER *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_ADDR, TAO_HANDLER *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_ADDR, TAO_HANDLER *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Bucket_Iterator<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
#pragma instantiate ACE_Pair<TAO_HANDLER *, TAO_ATTRIBUTES>
#pragma instantiate ACE_Reference_Pair<TAO_ADDR, TAO_HANDLER *>
#pragma instantiate ACE_Hash_Map_Entry<TAO_ADDR, TAO_CACHED_HANDLER>

#pragma instantiate ACE_Hash_Map_Manager<TAO_ADDR, TAO_CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_ADDR, TAO_CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_ADDR, TAO_CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Bucket_Iterator<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_KEY, TAO_COMPARE_KEYS, ACE_Null_Mutex>

// = Caching_Strategy
#pragma instantiate ACE_Hash_Cache_Map_Manager<TAO_ADDR, TAO_HANDLER *, TAO_HASH_KEY, TAO_COMPARE_KEYS, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>

#pragma instantiate ACE_LRU_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

#pragma instantiate ACE_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
#pragma instantiate ACE_LFU_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
#pragma instantiate ACE_FIFO_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
#pragma instantiate ACE_Null_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>

#pragma instantiate ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_LRU_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_LFU_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_FIFO_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_SSLIOP_NULL_CACHING_STRATEGY>

#pragma instantiate ACE_Cache_Map_Manager<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP, TAO_HASH_MAP_ITERATOR, TAO_HASH_MAP_REVERSE_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>
#pragma instantiate ACE_Cache_Map_Iterator<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>
#pragma instantiate ACE_Cache_Map_Reverse_Iterator<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP_REVERSE_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>

#else

#pragma instantiate ACE_Cache_Map_Manager<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

#pragma instantiate ACE_Cached_Connect_Strategy_Ex<TAO_HANDLER, ACE_SSL_SOCK_CONNECTOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES, TAO_Cached_Connector_Lock>
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_HANDLER, ACE_SSL_SOCK_CONNECTOR, TAO_Cached_Connector_Lock>

#pragma instantiate ACE_Cleanup_Strategy<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP>
#pragma instantiate ACE_Refcounted_Recyclable_Handler_Cleanup_Strategy<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP>
#pragma instantiate ACE_Refcounted_Recyclable_Handler_Caching_Utility<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP, TAO_HASH_MAP_ITERATOR, TAO_ATTRIBUTES>
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

TAO_SSLIOP_Connect_Creation_Strategy::
  TAO_SSLIOP_Connect_Creation_Strategy (ACE_Thread_Manager* t,
                                      TAO_ORB_Core *orb_core)
    :  ACE_Creation_Strategy<TAO_SSLIOP_Client_Connection_Handler> (t),
       orb_core_ (orb_core)
{
}

int
TAO_SSLIOP_Connect_Creation_Strategy::make_svc_handler
  (TAO_SSLIOP_Client_Connection_Handler *&sh)
{
  if (sh == 0)
    ACE_NEW_RETURN (sh,
                    TAO_SSLIOP_Client_Connection_Handler
                    (this->orb_core_->thr_mgr (),
                     this->orb_core_),
                    -1);
  return 0;
}

// ****************************************************************

#if !defined (TAO_USES_ROBUST_CONNECTION_MGMT)
typedef ACE_Cached_Connect_Strategy<TAO_SSLIOP_Client_Connection_Handler,
                                    ACE_SSL_SOCK_CONNECTOR,
                                    TAO_Cached_Connector_Lock>
        TAO_CACHED_CONNECT_STRATEGY;
#endif /* ! TAO_USES_ROBUST_CONNECTION_MGMT */

TAO_SSLIOP_Connector::TAO_SSLIOP_Connector (int default_is_ssl)
  : TAO_IIOP_Connector (),
    default_is_ssl_ (default_is_ssl),
    base_connector_ ()
#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
    ,
    cached_connect_strategy_ (0),
    caching_strategy_ (0)
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */
{
}

int
TAO_SSLIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  if (this->TAO_IIOP_Connector::open (orb_core) == -1)
    return -1;

  TAO_SSLIOP_Connect_Creation_Strategy *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_SSLIOP_Connect_Creation_Strategy
                  (this->orb_core_->thr_mgr (),
                   this->orb_core_),
                  -1);

  auto_ptr<TAO_SSLIOP_Connect_Creation_Strategy>
    new_connect_creation_strategy (connect_creation_strategy);

  TAO_Cached_Connector_Lock *connector_lock = 0;
  ACE_NEW_RETURN (connector_lock,
                  TAO_Cached_Connector_Lock (this->orb_core_),
                  -1);

  auto_ptr<TAO_Cached_Connector_Lock> new_connector_lock (connector_lock);

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  ACE_NEW_RETURN (this->cached_connect_strategy_,
                  TAO_CACHED_CONNECT_STRATEGY (*this->caching_strategy_,
                                               new_connect_creation_strategy.get (),
                                               0,
                                               0,
                                               new_connector_lock.get (),
                                               1),
                  -1);
#else /* TAO_USES_ROBUST_CONNECTION_MGMT */
  TAO_CACHED_CONNECT_STRATEGY *cached_connect_strategy = 0;
  ACE_NEW_RETURN (cached_connect_strategy,
                  TAO_CACHED_CONNECT_STRATEGY
                  (new_connect_creation_strategy.get (),
                   0,
                   0,
                   new_connector_lock.get (),
                   1),
                  -1);
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

  // Finally everything is fine.  Make sure to take ownership away
  // from the auto pointer.
  connect_creation_strategy =
    new_connect_creation_strategy.release ();
  connector_lock =
    new_connector_lock.release ();

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  return this->base_connector_.open (this->orb_core_->reactor (),
                                     &this->null_creation_strategy_,
                                     this->cached_connect_strategy_,
                                     &this->null_activation_strategy_);
#else /* TAO_USES_ROBUST_CONNECTION_MGMT */
  return this->base_connector_.open (this->orb_core_->reactor (),
                                     &this->null_creation_strategy_,
                                     cached_connect_strategy,
                                     &this->null_activation_strategy_);
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */
}

int
TAO_SSLIOP_Connector::close (void)
{
  this->base_connector_.close ();

  // Zap the creation strategy that we created earlier
#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  delete this->cached_connect_strategy_->creation_strategy ();
  delete this->cached_connect_strategy_;
  delete this->caching_strategy_;
#else /* TAO_USES_ROBUST_CONNECTION_MGMT */
  TAO_CACHED_CONNECT_STRATEGY *cached_connect_strategy =
    ACE_dynamic_cast (TAO_CACHED_CONNECT_STRATEGY *,
                      this->base_connector_.connect_strategy ());

  delete cached_connect_strategy->creation_strategy ();
  delete cached_connect_strategy;
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

  return 0;
}

int
TAO_SSLIOP_Connector::connect (TAO_Profile *pfile,
                               TAO_Transport *&transport,
                               ACE_Time_Value *max_wait_time)
{
  // @@ Use the policies to decide if SSL is the right protocol...
  if (!this->default_is_ssl_)
    return this->TAO_IIOP_Connector::connect (pfile,
					      transport,
					      max_wait_time);

  if (pfile->tag () != TAO_TAG_IIOP_PROFILE)
    return -1;

  TAO_SSLIOP_Profile *profile =
    ACE_dynamic_cast (TAO_SSLIOP_Profile *,
                      pfile);
  if (profile == 0)
    return -1;

  if (profile->ssl_port () == 0)
    return this->TAO_IIOP_Connector::connect (pfile,
					      transport,
					      max_wait_time);

  ACE_INET_Addr oa =
    profile->object_addr ();
  oa.set_port_number (profile->ssl_port ());

  ACE_Synch_Options synch_options;
  if (max_wait_time != 0)
    synch_options.set (ACE_Synch_Options::USE_TIMEOUT,
                       *max_wait_time);

  TAO_SSLIOP_Client_Connection_Handler* result;

  // The connect call will set the hint () stored in the Profile
  // object; but we obtain the transport in the <result>
  // variable. Other threads may modify the hint, but we are not
  // affected.
  if (this->base_connector_.connect (profile->ssl_hint (),
                                     result,
                                     oa,
                                     synch_options) == -1)
    { // Give users a clue to the problem.
      if (TAO_orbdebug)
        {
          char buffer [MAXHOSTNAMELEN + 6 + 1];
          profile->addr_to_string (buffer,
                                   sizeof(buffer) - 1);
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) %s:%u, connection to "
                      "%s failed (%p)\n",
                      __FILE__,
                      __LINE__,
                      buffer,
                      "errno"));
        }
      // @@ Without this reset_hint() call the ORB crashes after
      // several attempts to reconnect, apparently because the cached
      // connector already destroyed the object.  Using reset_hint()
      // seems to eliminate the problem, and actually purify is happy
      // with it, but i have some doubts about it: wasn't the hint
      // destroyed already by the connector?  We (Fred and Carlos)
      // thought about just setting the hint to 0, but that would not
      // be thread-safe (other threads may be touching the same
      // profile).  At this point (the day before 1.0) i'm reluctant
      // to change ACE, and this fix passes all the TAO tests
      // (including the new ping/pong test in the tests/Faults
      // directory).
      profile->reset_hint ();
      return -1;
    }

  transport = result->transport ();
  return 0;
}

TAO_Profile *
TAO_SSLIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_SSLIOP_Profile (this->orb_core_),
                  0);

  int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

void
TAO_SSLIOP_Connector::make_profile (const char *endpoint,
                                  TAO_Profile *&profile,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  ACE_NEW_THROW_EX (profile,
                    TAO_SSLIOP_Profile (endpoint,
					this->orb_core_,
                                        0, // @@ ssl_port
					ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());

  ACE_CHECK;
}
