/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Channel_Objects_Factory.h
 *
 *  $Id$
 *
 * Factory for all CosNotify objects defined in the idl files and
 * other related objects.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_CO_FACTORY_H
#define TAO_NOTIFY_CO_FACTORY_H

#include "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "notify_export.h"

class TAO_Notify_EventChannelFactory_i;
class TAO_Notify_EventChannel_i;
class TAO_Notify_ConsumerAdmin_i;
class TAO_Notify_SupplierAdmin_i;
class TAO_Notify_FilterFactory_i;
class TAO_Notify_StructuredProxyPushSupplier_i;
class TAO_Notify_SequenceProxyPushSupplier_i;
class TAO_Notify_ProxyPushSupplier_i;
class TAO_Notify_StructuredProxyPushConsumer_i;
class TAO_Notify_SequenceProxyPushConsumer_i;
class TAO_Notify_ProxyPushConsumer_i;
class ACE_Lock;

// @@ Pradeep: I'm not sure if I like the name of this class (and the
// name of the file is mismatched).... In any case, you need a better
// description here. I don't see any destroy methods: what if the
// objects are allocated from a free list, or from shared memory or
// something like that?
 /**
  * @class TAO_Notify_CO_Factory
  *
  * @brief TAO_Notify_CO_Factory
  *
  * Factory for channel objects.
  */
class TAO_Notify_Export TAO_Notify_CO_Factory : public ACE_Service_Object
{
 public:
  // = Creation methods
  /// Create an Event Channel.
  virtual TAO_Notify_EventChannel_i* create_event_channel (TAO_Notify_EventChannelFactory_i* parent ACE_ENV_ARG_DECL)=0;

  /// Create a Consumer Admin.
  virtual TAO_Notify_ConsumerAdmin_i* create_consumer_admin (TAO_Notify_EventChannel_i* channel ACE_ENV_ARG_DECL)=0;

  /// Create a Supplier Admin.
  virtual TAO_Notify_SupplierAdmin_i* create_supplier_admin (TAO_Notify_EventChannel_i* channel ACE_ENV_ARG_DECL)=0;

  /// Create the Filter Factory.
  virtual TAO_Notify_FilterFactory_i* create_filter_factory (ACE_ENV_SINGLE_ARG_DECL)=0;

  // = Proxy Creation methods.
  /// Create a Structured ProxyPushSupplier.
  virtual TAO_Notify_StructuredProxyPushSupplier_i* create_struct_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent ACE_ENV_ARG_DECL)=0;

  /// Create a Sequence ProxyPushSupplier.
  virtual TAO_Notify_SequenceProxyPushSupplier_i* create_seq_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent ACE_ENV_ARG_DECL)=0;

  /// Create a ProxyPushSupplier.
  virtual TAO_Notify_ProxyPushSupplier_i* create_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent ACE_ENV_ARG_DECL)=0;

  /// Create a Structured ProxyPushConsumer.
  virtual TAO_Notify_StructuredProxyPushConsumer_i* create_struct_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent ACE_ENV_ARG_DECL)=0;

  /// Create a Sequence ProxyPushConsumer.
  virtual TAO_Notify_SequenceProxyPushConsumer_i* create_seq_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent ACE_ENV_ARG_DECL)=0;

  /// Create a ProxyPushConsumer.
  virtual TAO_Notify_ProxyPushConsumer_i* create_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent ACE_ENV_ARG_DECL)=0;

  //= Locks
  // = Methods to create locks.
  /// Create lock type used by channel factory.
  virtual ACE_Lock* create_channel_factory_lock (ACE_ENV_SINGLE_ARG_DECL)=0;

  /// Create lock type used by EC's.
  virtual ACE_Lock* create_event_channel_lock (ACE_ENV_SINGLE_ARG_DECL)=0;

  /// Create lock type used by Consumer Admin's.
  virtual ACE_Lock* create_consumer_admin_lock (ACE_ENV_SINGLE_ARG_DECL)=0;

  /// Create lock type used by Supplier Admin's.
  virtual ACE_Lock* create_supplier_admin_lock (ACE_ENV_SINGLE_ARG_DECL)=0;

  /// Create lock type used by proxy supplier.
  virtual ACE_Lock* create_proxy_supplier_lock (ACE_ENV_SINGLE_ARG_DECL)=0;

  /// Create lock type used by proxy consumer.
  virtual ACE_Lock* create_proxy_consumer_lock (ACE_ENV_SINGLE_ARG_DECL)=0;
};

#include "ace/post.h"

#endif /* TAO_NOTIFY_CO_FACTORY_H */
