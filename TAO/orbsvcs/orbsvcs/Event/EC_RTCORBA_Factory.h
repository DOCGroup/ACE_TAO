// -*- C++ -*-
/**
 *  @file EC_RTCORBA_Factory.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_RTCORBA_FACTORY_H
#define TAO_EC_RTCORBA_FACTORY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/rtcorba_event_export.h"
#include "orbsvcs/Event/EC_Factory.h"
#include "tao/RTCORBA/RTCORBA.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_EC_RTCORBA_Factory
 *
 * @brief Decorate an EC_Factory to use the EC_RTCORBA_Dispatching
 * module.
 */
class TAO_RTCORBAEvent_Export TAO_EC_RTCORBA_Factory : public TAO_EC_Factory
{
public:
  /// Constructor
  /**
   * @param body The decorated factory.
   */
  TAO_EC_RTCORBA_Factory (TAO_EC_Factory *body,
                          const RTCORBA::ThreadpoolLanes &lanes);

  /// destructor...
  ~TAO_EC_RTCORBA_Factory (void) override;

  // Decorated methods...
  int init (int argc, ACE_TCHAR* argv[]) override;
  int fini () override;
  TAO_EC_Dispatching*
      create_dispatching (TAO_EC_Event_Channel_Base*) override;
  void
      destroy_dispatching (TAO_EC_Dispatching*) override ;
  TAO_EC_Filter_Builder*
      create_filter_builder (TAO_EC_Event_Channel_Base*) override  ;
  void
      destroy_filter_builder (TAO_EC_Filter_Builder*) override;
  TAO_EC_Supplier_Filter_Builder*
      create_supplier_filter_builder (TAO_EC_Event_Channel_Base*) override  ;
  void
      destroy_supplier_filter_builder (TAO_EC_Supplier_Filter_Builder*) override;
  TAO_EC_ConsumerAdmin*
      create_consumer_admin (TAO_EC_Event_Channel_Base*) override ;
  void
      destroy_consumer_admin (TAO_EC_ConsumerAdmin*) override;
  TAO_EC_SupplierAdmin*
      create_supplier_admin (TAO_EC_Event_Channel_Base*) override ;
  void
      destroy_supplier_admin (TAO_EC_SupplierAdmin*) override;
  TAO_EC_ProxyPushSupplier*
      create_proxy_push_supplier (TAO_EC_Event_Channel_Base*) override;
  void
      destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier*) override;
  TAO_EC_ProxyPushConsumer*
      create_proxy_push_consumer (TAO_EC_Event_Channel_Base*) override;
  void
      destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer*) override;
  TAO_EC_Timeout_Generator*
      create_timeout_generator (TAO_EC_Event_Channel_Base*) override ;
  void
      destroy_timeout_generator (TAO_EC_Timeout_Generator*) override;
  TAO_EC_ObserverStrategy*
      create_observer_strategy (TAO_EC_Event_Channel_Base*) override;
  void
      destroy_observer_strategy (TAO_EC_ObserverStrategy*) override;
  TAO_EC_Scheduling_Strategy*
      create_scheduling_strategy (TAO_EC_Event_Channel_Base*) override;
  void
      destroy_scheduling_strategy (TAO_EC_Scheduling_Strategy*) override;
  TAO_EC_ProxyPushConsumer_Collection*
      create_proxy_push_consumer_collection (TAO_EC_Event_Channel_Base*) override;
  void
      destroy_proxy_push_consumer_collection (TAO_EC_ProxyPushConsumer_Collection*) override;
  TAO_EC_ProxyPushSupplier_Collection*
      create_proxy_push_supplier_collection (TAO_EC_Event_Channel_Base*) override;
  void
      destroy_proxy_push_supplier_collection (TAO_EC_ProxyPushSupplier_Collection*) override;
  ACE_Lock* create_consumer_lock () override;
  void destroy_consumer_lock (ACE_Lock*) override;
  ACE_Lock* create_supplier_lock () override;
  void destroy_supplier_lock (ACE_Lock*) override;
  TAO_EC_ConsumerControl*
      create_consumer_control (TAO_EC_Event_Channel_Base*) override;
  void
      destroy_consumer_control (TAO_EC_ConsumerControl*) override;
  TAO_EC_SupplierControl*
      create_supplier_control (TAO_EC_Event_Channel_Base*) override;
  void
      destroy_supplier_control (TAO_EC_SupplierControl*) override;

protected:
  /// The decorated factory
  TAO_EC_Factory *body_;

  /// Configure the thread pool lanes.
  RTCORBA::ThreadpoolLanes lanes_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_EC_RTCORBA_FACTORY_H */
