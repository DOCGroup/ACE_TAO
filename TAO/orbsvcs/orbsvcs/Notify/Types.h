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

/**
 *  Define common types
 */
typedef CORBA::Long TAO_NS_Object_Id;

typedef ACE_Refcounted_Auto_Ptr<TAO_NS_Event, TAO_SYNCH_MUTEX> TAO_NS_Event_var;
typedef TAO_ESF_Proxy_Collection<TAO_NS_ProxyConsumer> TAO_NS_Supplier_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_ProxySupplier> TAO_NS_Consumer_Collection;

typedef TAO_ESF_Proxy_Collection<TAO_NS_EventChannel> TAO_NS_EventChannel_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_Admin> TAO_NS_Admin_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_Proxy> TAO_NS_Proxy_Collection;

typedef TAO_ESF_RefCount_Guard<CORBA::ULong> TAO_NS_Object_RefCount_Guard;

typedef TAO_NS_Event_Map_T<TAO_NS_ProxySupplier, TAO_SYNCH_RW_MUTEX> TAO_NS_Consumer_Map;
typedef TAO_NS_Event_Map_T<TAO_NS_ProxyConsumer, TAO_SYNCH_RW_MUTEX> TAO_NS_Supplier_Map;

#include "ace/post.h"
#endif /* TAO_NS_TYPES_H */
