// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/IIOP_Connector.h"
#include "tao/IIOP_Profile.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Environment.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(tao, IIOP_Connector, "$Id$")

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
int
TAO_IIOP_Connector::purge_connections (void)
{
  return this->cached_connect_strategy_->purge_connections ();
}

#define TAO_HANDLER TAO_IIOP_Client_Connection_Handler
#define TAO_SVC_TUPLE ACE_Svc_Tuple<TAO_HANDLER>
#define TAO_ADDR TAO_IIOP_Connector::TAO_IADDR
#define TAO_HASH_KEY TAO_IIOP_Connector::TAO_HASH_KEY
#define TAO_COMPARE_KEYS TAO_IIOP_Connector::TAO_COMPARE_KEYS
#define TAO_ATTRIBUTES TAO_IIOP_Connector::TAO_ATTRIBUTES
#define TAO_CACHED_HANDLER TAO_IIOP_Connector::TAO_CACHED_HANDLER
#define TAO_HASH_MAP TAO_IIOP_Connector::TAO_HASH_MAP
#define TAO_HASH_MAP_ITERATOR TAO_IIOP_Connector::TAO_HASH_MAP_ITERATOR
#define TAO_HASH_MAP_REVERSE_ITERATOR TAO_IIOP_Connector::TAO_HASH_MAP_REVERSE_ITERATOR
#define TAO_CACHING_UTILITY TAO_IIOP_Connector::TAO_CACHING_UTILITY
#define TAO_CACHING_STRATEGY TAO_IIOP_Connector::TAO_CACHING_STRATEGY
#define TAO_CACHED_CONNECT_STRATEGY TAO_IIOP_Connector::TAO_CACHED_CONNECT_STRATEGY

typedef ACE_LRU_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
        TAO_IIOP_LRU_CACHING_STRATEGY;

# if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES) || \
(TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY == 1) 

typedef TAO_IIOP_LRU_CACHING_STRATEGY
        TAO_IIOP_CACHING_STRATEGY;
#else
typedef ACE_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
        TAO_IIOP_CACHING_STRATEGY;
typedef ACE_LFU_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
        TAO_IIOP_LFU_CACHING_STRATEGY;
typedef ACE_FIFO_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
        TAO_IIOP_FIFO_CACHING_STRATEGY;
typedef ACE_Null_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
        TAO_IIOP_NULL_CACHING_STRATEGY;
typedef ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_LRU_CACHING_STRATEGY>
        TAO_IIOP_LRU_CACHING_STRATEGY_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_LFU_CACHING_STRATEGY>
        TAO_IIOP_LFU_CACHING_STRATEGY_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_FIFO_CACHING_STRATEGY>
        TAO_IIOP_FIFO_CACHING_STRATEGY_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_NULL_CACHING_STRATEGY>
        TAO_IIOP_NULL_CACHING_STRATEGY_ADAPTER;
#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES or TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY*/

int
TAO_IIOP_Connector::make_caching_strategy (void)
{
  TAO_Resource_Factory *resource_factory =
    this->orb_core_->resource_factory ();

#if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES) || \
(TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY == 1)
  ACE_NEW_RETURN (this->caching_strategy_,
                  TAO_IIOP_CACHING_STRATEGY,
                  -1);
#else
  switch (resource_factory->connection_caching_strategy_type ())
    {
    case TAO_Resource_Factory::NOOP:
      ACE_NEW_RETURN (this->caching_strategy_,
                      TAO_IIOP_NULL_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;

    default:
    case TAO_Resource_Factory::LRU:
      ACE_NEW_RETURN (this->caching_strategy_,
                      TAO_IIOP_LRU_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;

    case TAO_Resource_Factory::LFU:
      ACE_NEW_RETURN (this->caching_strategy_,
                      TAO_IIOP_LFU_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;

    case TAO_Resource_Factory::FIFO:
      ACE_NEW_RETURN (this->caching_strategy_,
                      TAO_IIOP_FIFO_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;
    }

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES or
          TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY == 1*/


  this->caching_strategy_->purge_percent (resource_factory->purge_percentage ());
  return 0;
}
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

// The TAO_Cached_Connector_Lock template instantiations are in
// Resource_Factory.cpp.

#if !defined (TAO_USES_ROBUST_CONNECTION_MGMT)
#define TAO_SVC_TUPLE ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler>
#define CACHED_CONNECT_STRATEGY ACE_Cached_Connect_Strategy<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR, TAO_Cached_Connector_Lock>
#define TAO_ADDR ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
#define TAO_HANDLER TAO_IIOP_Client_Connection_Handler
#define TAO_HASH_KEY ACE_Hash<TAO_ADDR>
#define TAO_COMPARE_KEYS ACE_Equal_To<TAO_ADDR>
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) || \
    defined (ACE_HAS_GNU_REPO)

template class ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr>;
template class ACE_Auto_Basic_Array_Ptr<TAO_IIOP_Client_Connection_Handler*>;
template class auto_ptr<TAO_IIOP_Connect_Creation_Strategy>;
template class ACE_Auto_Basic_Ptr<TAO_IIOP_Connect_Creation_Strategy>;

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
template class ACE_Connect_Strategy<TAO_HANDLER, ACE_SOCK_CONNECTOR>;
template class ACE_Connector<TAO_HANDLER, ACE_SOCK_CONNECTOR>;
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
template class ACE_Strategy_Connector<TAO_HANDLER, ACE_SOCK_CONNECTOR>;
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

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES) || \
(TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY == 0)
template class ACE_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>;
template class ACE_LFU_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>;
template class ACE_FIFO_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>;
template class ACE_Null_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>;

template class ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_LRU_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_LFU_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_FIFO_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_NULL_CACHING_STRATEGY>;

template class ACE_Cache_Map_Manager<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP, TAO_HASH_MAP_ITERATOR, TAO_HASH_MAP_REVERSE_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>;
template class ACE_Cache_Map_Iterator<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>;
template class ACE_Cache_Map_Reverse_Iterator<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP_REVERSE_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>;

#else

template class ACE_Cache_Map_Manager<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>;

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES
          TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY == 0*/

template class ACE_Cached_Connect_Strategy_Ex<TAO_HANDLER, ACE_SOCK_CONNECTOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES, TAO_Cached_Connector_Lock>;
template class ACE_Cached_Connect_Strategy<TAO_HANDLER, ACE_SOCK_CONNECTOR, TAO_Cached_Connector_Lock>;

template class ACE_Cleanup_Strategy<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP>;
template class ACE_Refcounted_Recyclable_Handler_Cleanup_Strategy<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP>;
template class ACE_Refcounted_Recyclable_Handler_Caching_Utility<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP, TAO_HASH_MAP_ITERATOR, TAO_ATTRIBUTES>;
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<TAO_IIOP_Client_Connection_Handler*>
#pragma instantiate auto_ptr<TAO_IIOP_Connect_Creation_Strategy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_IIOP_Connect_Creation_Strategy>

#pragma instantiate ACE_Node<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Stack<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Stack_Iterator<ACE_INET_Addr>

#if !defined (TAO_USES_ROBUST_CONNECTION_MGMT)
#pragma instantiate CACHED_CONNECT_STRATEGY
#pragma instantiate TAO_ADDR
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
#pragma instantiate ACE_NOOP_Creation_Strategy<TAO_HANDLER>
#pragma instantiate ACE_Concurrency_Strategy<TAO_HANDLER>
#pragma instantiate ACE_Connect_Strategy<TAO_HANDLER, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector<TAO_HANDLER, ACE_SOCK_CONNECTOR>
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
#pragma instantiate ACE_Strategy_Connector<TAO_HANDLER, ACE_SOCK_CONNECTOR>
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

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES) || \
(TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY == 0)

#pragma instantiate ACE_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
#pragma instantiate ACE_LFU_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
#pragma instantiate ACE_FIFO_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>
#pragma instantiate ACE_Null_Caching_Strategy<TAO_ATTRIBUTES, TAO_CACHING_UTILITY>

#pragma instantiate ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_LRU_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_LFU_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_FIFO_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<TAO_ATTRIBUTES, TAO_CACHING_UTILITY, TAO_IIOP_NULL_CACHING_STRATEGY>

#pragma instantiate ACE_Cache_Map_Manager<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP, TAO_HASH_MAP_ITERATOR, TAO_HASH_MAP_REVERSE_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>
#pragma instantiate ACE_Cache_Map_Iterator<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>
#pragma instantiate ACE_Cache_Map_Reverse_Iterator<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP_REVERSE_ITERATOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>

#else

#pragma instantiate ACE_Cache_Map_Manager<TAO_ADDR, TAO_HANDLER *, TAO_HASH_MAP, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES>

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES
          TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY == 0*/

#pragma instantiate ACE_Cached_Connect_Strategy_Ex<TAO_HANDLER, ACE_SOCK_CONNECTOR, TAO_CACHING_STRATEGY, TAO_ATTRIBUTES, TAO_Cached_Connector_Lock>
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_HANDLER, ACE_SOCK_CONNECTOR, TAO_Cached_Connector_Lock>

#pragma instantiate ACE_Cleanup_Strategy<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP>
#pragma instantiate ACE_Refcounted_Recyclable_Handler_Cleanup_Strategy<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP>
#pragma instantiate ACE_Refcounted_Recyclable_Handler_Caching_Utility<TAO_ADDR, TAO_CACHED_HANDLER, TAO_HASH_MAP, TAO_HASH_MAP_ITERATOR, TAO_ATTRIBUTES>
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

TAO_IIOP_Connect_Creation_Strategy::
  TAO_IIOP_Connect_Creation_Strategy (ACE_Thread_Manager* t,
                                      TAO_ORB_Core *orb_core,
                                      CORBA::Boolean flag)
    :  ACE_Creation_Strategy<TAO_IIOP_Client_Connection_Handler> (t),
       orb_core_ (orb_core),
       lite_flag_ (flag)
{
}

int
TAO_IIOP_Connect_Creation_Strategy::make_svc_handler
  (TAO_IIOP_Client_Connection_Handler *&sh)
{
  if (sh == 0)
    ACE_NEW_RETURN (sh,
                    TAO_IIOP_Client_Connection_Handler
                    (this->orb_core_->thr_mgr (),
                     this->orb_core_,
                     this->lite_flag_),
                    -1);
  return 0;
}

// ****************************************************************

#if !defined (TAO_USES_ROBUST_CONNECTION_MGMT)
typedef ACE_Cached_Connect_Strategy<TAO_IIOP_Client_Connection_Handler,
                                    ACE_SOCK_CONNECTOR,
                                    TAO_Cached_Connector_Lock>
        TAO_CACHED_CONNECT_STRATEGY;
#endif /* ! TAO_USES_ROBUST_CONNECTION_MGMT */


TAO_IIOP_Connector::TAO_IIOP_Connector (CORBA::Boolean flag)
  : TAO_Connector (TAO_TAG_IIOP_PROFILE),

    orb_core_ (0),
    base_connector_ (),
    lite_flag_ (flag)
#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
    ,
    cached_connect_strategy_ (0),
    caching_strategy_ (0)
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */
{
}

int
TAO_IIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core_ = orb_core;

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  if (this->make_caching_strategy () == -1)
    return -1;
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

  TAO_IIOP_Connect_Creation_Strategy *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_IIOP_Connect_Creation_Strategy
                  (this->orb_core_->thr_mgr (),
                   this->orb_core_,
                   this->lite_flag_),
                  -1);

  auto_ptr<TAO_IIOP_Connect_Creation_Strategy>
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
TAO_IIOP_Connector::close (void)
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
TAO_IIOP_Connector::connect (TAO_Profile *profile,
                             TAO_Transport *&transport,
                             ACE_Time_Value *max_wait_time)
{
  if (profile->tag () != TAO_TAG_IIOP_PROFILE)
    return -1;

  TAO_IIOP_Profile *iiop_profile =
    ACE_dynamic_cast (TAO_IIOP_Profile *,
                      profile);
  if (iiop_profile == 0)
    return -1;

  const ACE_INET_Addr &remote_address =
    iiop_profile->object_addr ();

  TAO_IIOP_Client_Connection_Handler *svc_handler = 0;
  int result = 0;

  if (max_wait_time != 0)
    {
      ACE_Synch_Options synch_options (ACE_Synch_Options::USE_TIMEOUT,
                                       *max_wait_time);

      // The connect call will set the hint () stored in the Profile
      // object; but we obtain the transport in the <svc_handler>
      // variable. Other threads may modify the hint, but we are not
      // affected.
      result = this->base_connector_.connect (iiop_profile->hint (),
                                              svc_handler,
                                              remote_address,
                                              synch_options);
    }
  else
    {
      // The connect call will set the hint () stored in the Profile
      // object; but we obtain the transport in the <svc_handler>
      // variable. Other threads may modify the hint, but we are not
      // affected.
      result = this->base_connector_.connect (iiop_profile->hint (),
                                              svc_handler,
                                              remote_address);
    }

  if (result == -1)
    {
      // Give users a clue to the problem.
      if (TAO_orbdebug)
        {
          char buffer [MAXNAMELEN * 2];
          profile->addr_to_string (buffer,
                                   (MAXNAMELEN * 2) - 1);
          ACE_DEBUG ((LM_ERROR,
                      ASYS_TEXT ("(%P|%t) %s:%u, connection to ")
                      ASYS_TEXT ("%s failed (%p)\n"),
                      __FILE__,
                      __LINE__,
                      buffer,
                      "errno"));
        }
      return -1;
    }

  transport = svc_handler->transport ();

  // Now that we have the client connection handler object we need to
  // set the right messaging protocol for in the client side transport.
  const TAO_GIOP_Version& version = iiop_profile->version ();
  int ret_val = transport->messaging_init (version.major,
                                           version.minor);
  if (ret_val == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%N|%l|%p|%t) init_mesg_protocol () failed \n")));
        }
      return -1;
    }
  
  return 0;
}

int
TAO_IIOP_Connector::preconnect (const char *preconnects)
{
  // Check for the proper protocol prefix.
  if (this->check_prefix (preconnects) != 0)
    return 0; // Failure: zero successful preconnections

  const char *protocol_removed =
    ACE_OS::strstr (preconnects,
                    "://") + 3;
  // "+ 3" since strlen of "://" is 3.

  char *preconnections =
    ACE_OS::strdup (protocol_removed);

  int successes = 0;
  if (preconnections)
    {
      ACE_INET_Addr dest;
      ACE_Unbounded_Stack<ACE_INET_Addr> dests;

      size_t num_connections;

      char *nextptr = 0;
      char *where = 0;
      for (where = ACE::strsplit_r (preconnections, ",", nextptr);
           where != 0;
           where = ACE::strsplit_r (0, ",", nextptr))
        {
          int version_offset = 0;
          // Additional offset to remove version from preconnect, if it exists.

          if (isdigit (where[0]) &&
              where[1] == '.' &&
              isdigit (where[2]) &&
              where[3] == '@')
            version_offset = 4;

          // @@ For now, we just drop the version prefix.  However, at
          //    some point in the future the version may become useful.

          char *tport = 0;
          char *thost = where + version_offset;
          char *sep = ACE_OS::strchr (where, ':');

          if (sep)
            {
              *sep = '\0';
              tport = sep + 1;

              dest.set ((u_short) ACE_OS::atoi (tport), thost);
              dests.push (dest);
            }
          else
            {
              // No port was specified so assume that the port will be the
              // IANA assigned port for IIOP.
              //
              //    IIOP:           683
              //    IIOP over SSL:  684

              dest.set (683, thost);
              dests.push (dest);

              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ASYS_TEXT ("TAO (%P|%t) No port specified for <%s>.  ")
                              ASYS_TEXT ("Using <%d> as default port.\n"),
                              where,
                              dest.get_port_number ()));
                }
            }
        }

      // Create an array of addresses from the stack, as well as an
      // array of eventual handlers.
      num_connections = dests.size ();
      ACE_INET_Addr *remote_addrs = 0;
      TAO_IIOP_Client_Connection_Handler **handlers = 0;
      char *failures = 0;

      ACE_NEW_RETURN (remote_addrs,
                      ACE_INET_Addr[num_connections],
                      -1);

      ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr> safe_remote_addrs (remote_addrs);

      ACE_NEW_RETURN (handlers,
                      TAO_IIOP_Client_Connection_Handler *[num_connections],
                      -1);

      ACE_Auto_Basic_Array_Ptr<TAO_IIOP_Client_Connection_Handler*>
        safe_handlers (handlers);

      ACE_NEW_RETURN (failures,
                      char[num_connections],
                      -1);

      // No longer need to worry about exception safety at this point.
      remote_addrs = safe_remote_addrs.release ();
      handlers = safe_handlers.release ();

      size_t slot = 0;

      // Fill in the remote address array
      while (dests.pop (remote_addrs[slot]) == 0)
        handlers[slot++] = 0;

      // Finally, try to connect.
      this->base_connector_.connect_n (num_connections,
                                       handlers,
                                       remote_addrs,
                                       failures);
      // Loop over all the failures and set the handlers that
      // succeeded to idle state.
      for (slot = 0;
           slot < num_connections;
           slot++)
        {
          if (!failures[slot])
            {
              handlers[slot]->idle ();
              successes++;

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            ASYS_TEXT ("TAO (%P|%t) Preconnection <%s:%d> ")
                            ASYS_TEXT ("succeeded.\n"),
                            remote_addrs[slot].get_host_name (),
                            remote_addrs[slot].get_port_number ()));
            }
          else if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("TAO (%P|%t) Preconnection <%s:%d> failed.\n"),
                        remote_addrs[slot].get_host_name (),
                        remote_addrs[slot].get_port_number ()));
        }

      ACE_OS::free (preconnections);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) IIOP preconnections: %d successes and ")
                    ASYS_TEXT ("%d failures.\n"),
                    successes,
                    num_connections - successes));
    }
  return successes;
}

TAO_Profile *
TAO_IIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_IIOP_Profile (this->orb_core_),
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
TAO_IIOP_Connector::make_profile (const char *endpoint,
                                  TAO_Profile *&profile,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  ACE_NEW_THROW_EX (profile,
                    TAO_IIOP_Profile (endpoint,
                                      this->orb_core_,
                                      ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());

  ACE_CHECK;

  }

int
TAO_IIOP_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  const char *protocol[] = { "iiop", "iioploc" };

  size_t slot = ACE_OS::strchr (endpoint, ':') - endpoint;

  size_t len0 = ACE_OS::strlen (protocol[0]);
  size_t len1 = ACE_OS::strlen (protocol[1]);

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len0
      && ACE_OS::strncasecmp (endpoint, protocol[0], len0) == 0)
    return 0;
  else if (slot == len1
           && ACE_OS::strncasecmp (endpoint, protocol[1], len1) == 0)
    return 0;

  return -1;
  // Failure: not an IIOP IOR
  // DO NOT throw an exception here.
}

char
TAO_IIOP_Connector::object_key_delimiter (void) const
{
  return TAO_IIOP_Profile::object_key_delimiter_;
}
