/* -*- C++ -*- */
/**
 *  @file Types.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_TYPES_H
#define TAO_NS_TYPES_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "tao/orbconf.h"

/**
 *  Forward declare templates
 */
template <class EVENT, class SYNCH> class ACE_Refcounted_Auto_Ptr;
template <class PROXY> class TAO_ESF_Proxy_Collection;
template <class TYPE> class TAO_ESF_RefCount_Guard;
template <class PROXY, class ACE_LOCK> class TAO_NS_Event_Map_T;
template <class T> class ACE_Unbounded_Queue;
template <class PEER> class TAO_NS_Dispatch_Observer_T;
template <class PROXY> class TAO_NS_Event_Map_Observer_T;
template <class PEER> class TAO_NS_Pending_Worker_T;
template <class LOCK, class TYPE> ACE_Atomic_Op;
template <class LOCK, class TYPE> TAO_NS_Signal_Property;
template <class TYPE> TAO_NS_Property_T;

/**
 *  Forward declare classes
 */
class TAO_NS_Supplier;
class TAO_NS_Consumer;
class TAO_NS_Object;
class TAO_NS_EventChannel;
class TAO_NS_Admin;
class TAO_NS_Proxy;
class TAO_NS_Event;
class TAO_NS_ProxySupplier;
class TAO_NS_ProxyConsumer;
class TAO_NS_Peer;
class TAO_NS_EventType;
class TAO_NS_EventTypeSeq;

/**
 *  Define common types
 */
typedef ACE_Reverse_Lock<TAO_SYNCH_MUTEX> TAO_NS_Reverse_Lock;

typedef CORBA::Long TAO_NS_Object_Id;

typedef ACE_Refcounted_Auto_Ptr<TAO_NS_Event, TAO_SYNCH_MUTEX> TAO_NS_Event_var;
typedef ACE_Unbounded_Queue<TAO_NS_Event_var> TAO_NS_Event_Collection;

typedef TAO_ESF_Proxy_Collection<TAO_NS_ProxyConsumer> TAO_NS_ProxyConsumer_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_ProxySupplier> TAO_NS_ProxySupplier_Collection;

typedef TAO_ESF_Proxy_Collection<TAO_NS_EventChannel> TAO_NS_EventChannel_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_Admin> TAO_NS_Admin_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_Proxy> TAO_NS_Proxy_Collection;

typedef TAO_ESF_Proxy_Collection<TAO_NS_Consumer> TAO_NS_Consumer_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_Supplier> TAO_NS_Supplier_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_Peer> TAO_NS_Peer_Collection;

typedef TAO_ESF_RefCount_Guard<CORBA::ULong> TAO_NS_Object_RefCount_Guard;

typedef TAO_NS_Event_Map_T<TAO_NS_ProxySupplier, TAO_SYNCH_RW_MUTEX> TAO_NS_Consumer_Map;
typedef TAO_NS_Event_Map_T<TAO_NS_ProxyConsumer, TAO_SYNCH_RW_MUTEX> TAO_NS_Supplier_Map;

typedef TAO_NS_Event_Map_Observer_T<TAO_NS_ProxyConsumer> TAO_NS_Consumer_Map_Observer;
typedef TAO_NS_Event_Map_Observer_T<TAO_NS_ProxySupplier> TAO_NS_Supplier_Map_Observer;

typedef TAO_NS_Dispatch_Observer_T<TAO_NS_Peer> TAO_NS_Updates_Dispatch_Observer;
typedef TAO_NS_Dispatch_Observer_T<TAO_NS_Consumer> TAO_NS_Event_Dispatch_Observer;

typedef TAO_NS_Pending_Worker_T<TAO_NS_Peer> TAO_NS_Updates_Pending_Worker;
typedef TAO_NS_Pending_Worker_T<TAO_NS_Consumer> TAO_NS_Event_Pending_Worker;

typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX,CORBA::Long> TAO_NS_Property_Atomic_Long;
typedef TAO_NS_Signal_Property <TAO_SYNCH_MUTEX,CORBA::Long> TAO_NS_Signal_Property_Long;

typedef TAO_NS_Property_T<CORBA::Long> TAO_NS_Property_Long;

#include "ace/post.h"
#endif /* TAO_NS_TYPES_H */
