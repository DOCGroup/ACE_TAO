/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Default_CO_Factory.h
//
// = DESCRIPTION
//   Default factory for channel objects.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef NOTIFY_DEFAULT_CO_FACTORY_H
#define NOTIFY_DEFAULT_CO_FACTORY_H

#include "ace/pre.h"
#include "Notify_Channel_Objects_Factory.h"
#include "notify_export.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_Export TAO_Notify_Default_CO_Factory : public TAO_Notify_CO_Factory
{
  // = TITLE
  //   TAO_Notify_Default_CO_Factory
  //
  // = DESCRIPTION
  //   Default Channel Objects factory
  //
public:
  TAO_Notify_Default_CO_Factory (void);
  ~TAO_Notify_Default_CO_Factory ();

  static int init_svc (void);
  // Helper function to register the default factory into the service
  // configurator.

  // = The Service_Object entry points
  virtual int init (int argc, char* argv[]);
  virtual int fini (void);

  // = TAO_Notify_CO_Factory methods
  // = Creation methods
  TAO_Notify_EventChannel_i* create_event_channel (TAO_Notify_EventChannelFactory_i* parent TAO_ENV_ARG_DECL);
  // Create an Event Channel.

  TAO_Notify_ConsumerAdmin_i* create_consumer_admin (TAO_Notify_EventChannel_i* channel TAO_ENV_ARG_DECL);
  // Create a Consumer Admin.

  TAO_Notify_SupplierAdmin_i* create_supplier_admin (TAO_Notify_EventChannel_i* channel TAO_ENV_ARG_DECL);
  // Create a Supplier Admin.

  TAO_Notify_FilterFactory_i* create_filter_factory (TAO_ENV_SINGLE_ARG_DECL);
  // Create the Filter Factory.

  // = Proxy Creation methods.
  TAO_Notify_StructuredProxyPushSupplier_i* create_struct_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent TAO_ENV_ARG_DECL);
  // Create a Structured ProxyPushSupplier.

  TAO_Notify_SequenceProxyPushSupplier_i* create_seq_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent TAO_ENV_ARG_DECL);
  // Create a Sequence ProxyPushSupplier.

  TAO_Notify_ProxyPushSupplier_i* create_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent TAO_ENV_ARG_DECL);
  // Create a ProxyPushSupplier.

  TAO_Notify_StructuredProxyPushConsumer_i* create_struct_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent TAO_ENV_ARG_DECL);
  // Create a Structured ProxyPushConsumer.

  TAO_Notify_SequenceProxyPushConsumer_i* create_seq_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent TAO_ENV_ARG_DECL);
  // Create a Sequence ProxyPushConsumer.

  TAO_Notify_ProxyPushConsumer_i* create_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent TAO_ENV_ARG_DECL);
  // Create a ProxyPushConsumer.

  //= Locks
  // = Methods to create locks.
  ACE_Lock* create_channel_factory_lock (TAO_ENV_SINGLE_ARG_DECL);
  // Create lock type used by channel factory.

  ACE_Lock* create_event_channel_lock (TAO_ENV_SINGLE_ARG_DECL);
  // Create lock type used by EC's.

  ACE_Lock* create_consumer_admin_lock (TAO_ENV_SINGLE_ARG_DECL);
  // Create lock type used by Consumer Admin's.

  ACE_Lock* create_supplier_admin_lock (TAO_ENV_SINGLE_ARG_DECL);
  // Create lock type used by Supplier Admin's.

  ACE_Lock* create_proxy_supplier_lock (TAO_ENV_SINGLE_ARG_DECL);
  // Create lock type used by proxy supplier.

  ACE_Lock* create_proxy_consumer_lock (TAO_ENV_SINGLE_ARG_DECL);
  // Create lock type used by proxy consumer.
};

ACE_STATIC_SVC_DECLARE (TAO_Notify_Default_CO_Factory)
ACE_FACTORY_DECLARE (TAO_Notify,TAO_Notify_Default_CO_Factory)

#include "ace/post.h"

#endif /* NOTIFY_DEFAULT_CO_FACTORY_H */
